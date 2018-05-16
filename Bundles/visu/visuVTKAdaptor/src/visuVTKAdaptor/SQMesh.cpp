/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "visuVTKAdaptor/SQMesh.hpp"

#include "visuVTKAdaptor/SQMaterial.hpp"
#include "visuVTKAdaptor/SQMeshNormals.hpp"
#include "visuVTKAdaptor/STexture.hpp"
#include "visuVTKAdaptor/SQTransform.hpp"

#include <fwCom/Signal.hxx>
#include <fwCom/Slot.hpp>
#include <fwCom/Slot.hxx>
#include <fwCom/Slots.hpp>
#include <fwCom/Slots.hxx>

#include <fwData/Material.hpp>
#include <fwData/Mesh.hpp>
#include <fwData/mt/ObjectReadLock.hpp>

#include <fwRuntime/EConfigurationElement.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/op/Add.hpp>
#include <fwServices/QtQmlType.hxx>

#include <fwVtkIO/helper/Mesh.hpp>
#include <fwVtkIO/vtk.hpp>

#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCommand.h>
#include <vtkDepthSortPolyData.h>
#include <vtkMath.h>
#include <vtkMatrix4x4.h>
#include <vtkPicker.h>
#include <vtkPlaneCollection.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataNormals.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkTextureMapToCylinder.h>
#include <vtkTextureMapToPlane.h>
#include <vtkTextureMapToSphere.h>
#include <vtkTransform.h>

//-----------------------------------------------------------------------------

namespace visuVTKAdaptor
{

static ::fwServices::QtQmlType<SQMesh>   registar("com.fw4spl.vtk.adaptors", 1, 0, "SMesh");

//-----------------------------------------------------------------------------

const ::fwCom::Signals::SignalKeyType SQMesh::s_TEXTURE_APPLIED_SIG = "textureApplied";

const ::fwCom::Slots::SlotKeyType SQMesh::s_UPDATE_VISIBILITY_SLOT       = "updateVisibility";
const ::fwCom::Slots::SlotKeyType SQMesh::s_UPDATE_POINT_COLORS_SLOT     = "updatePointColors";
const ::fwCom::Slots::SlotKeyType SQMesh::s_UPDATE_CELL_COLORS_SLOT      = "updateCellColors";
const ::fwCom::Slots::SlotKeyType SQMesh::s_UPDATE_VERTEX_SLOT           = "updateVertex";
const ::fwCom::Slots::SlotKeyType SQMesh::s_UPDATE_POINT_NORMALS_SLOT    = "updatePointNormals";
const ::fwCom::Slots::SlotKeyType SQMesh::s_UPDATE_CELL_NORMALS_SLOT     = "updateCellNormals";
const ::fwCom::Slots::SlotKeyType SQMesh::s_UPDATE_POINT_TEX_COORDS_SLOT = "updatePointTexCoords";
const ::fwCom::Slots::SlotKeyType SQMesh::s_UPDATE_CELL_TEX_COORDS_SLOT  = "updateCellTexCoords";
const ::fwCom::Slots::SlotKeyType SQMesh::s_SHOW_POINT_COLORS_SLOT       = "showPointColors";
const ::fwCom::Slots::SlotKeyType SQMesh::s_SHOW_CELL_COLORS_SLOT        = "showCellColors";
const ::fwCom::Slots::SlotKeyType SQMesh::s_HIDE_COLORS_SLOT             = "hideColors";
const ::fwCom::Slots::SlotKeyType SQMesh::s_UPDATE_COLOR_MODE_SLOT       = "updateColorMode";
const ::fwCom::Slots::SlotKeyType SQMesh::s_UPDATE_NORMAL_MODE_SLOT      = "updateNormalMode";
const ::fwCom::Slots::SlotKeyType SQMesh::s_UPDATE_MATRIX_FIELD_SLOT     = "updateMatrixField";

const ::fwServices::IService::KeyType SQMesh::s_MESH_INPUT = "mesh";

static const std::string s_MATRIX_FIELD_NAME = "TransformMatrix";

//-----------------------------------------------------------------------------

class MeshVtkCommand : public vtkCommand
{
public:
    virtual void Stop() = 0;
};

//------------------------------------------------------------------------------

class PlaneShifterCallback : public MeshVtkCommand
{
public:

    //------------------------------------------------------------------------------

    static PlaneShifterCallback* New( vtkPlane* src,  vtkPlane* dst, double factor = 1.)
    {
        return new PlaneShifterCallback( src, dst, factor );
    }

    //--------------------------------------------------------------------------
    PlaneShifterCallback( vtkPlane* src,  vtkPlane* dst, double factor ) :
        m_planeSrc(src),
        m_planeDst(dst),
        m_factor(factor)
    {
        m_planeSrc->Register(this);
        m_planeDst->Register(this);
        src->AddObserver(vtkCommand::ModifiedEvent, this);
        this->Execute( 0, vtkCommand::ModifiedEvent, 0);
    }

    //------------------------------------------------------------------------------

    void Stop()
    {
        m_planeSrc->RemoveObserver(this);
        m_planeSrc->UnRegister(this);
        m_planeDst->UnRegister(this);
    }

    //------------------------------------------------------------------------------

    virtual void Execute( vtkObject* caller, unsigned long eventId, void* )
    {
        if (eventId == vtkCommand::ModifiedEvent)
        {
            double n[3];
            double o[3];

            m_planeSrc->GetNormal(n);
            m_planeSrc->GetOrigin(o);

            if (m_factor < 0)
            {
                for (int i = 0; i < 3; i++)
                {
                    n[i] = -n[i];
                }
            }

            m_planeDst->SetNormal( n );
            m_planeDst->SetOrigin( o );
            m_planeDst->Push( m_factor );
            m_planeDst->Modified();
        }

    }

protected:
    vtkPlane* m_planeSrc;
    vtkPlane* m_planeDst;
    double m_factor;
};

class PlaneCollectionAdaptorStarter : public MeshVtkCommand
{
public:

    //------------------------------------------------------------------------------

    static PlaneCollectionAdaptorStarter* New(
        ::visuVTKAdaptor::SQMesh *service,
        vtkPlaneCollection* src,
        double factor = 1. )
    {
        return new PlaneCollectionAdaptorStarter( service, src, factor );
    }

    //--------------------------------------------------------------------------
    PlaneCollectionAdaptorStarter( ::visuVTKAdaptor::SQMesh *service,
                                   vtkPlaneCollection* src,
                                   double factor) :
        m_service(service),
        m_planeCollectionSrc(src),
        m_factor(factor)
    {
        m_planeCollectionSrc->Register(this);
        m_planeCollectionSrc->AddObserver(vtkCommand::ModifiedEvent, this);
        this->Execute( 0, vtkCommand::ModifiedEvent, 0);
    }

    //------------------------------------------------------------------------------

    void Stop()
    {
        m_planeCollectionSrc->RemoveObserver(this);
        m_planeCollectionSrc->UnRegister(this);
        this->Clear();
    }

    //------------------------------------------------------------------------------

    void Clear()
    {
        for( std::weak_ptr<::visuVTKAdaptor::SQMesh> adaptor :  m_meshServices )
        {
            if (!adaptor.expired())
            {
                adaptor.lock()->stop();
                //::fwServices::OSR::unregisterService(adaptor.lock());
            }
        }
        m_meshServices.clear();

        for( PlaneShifterCallback* psc :  m_planeCallbacks )
        {
            psc->Stop();
            psc->Delete();
            psc = 0;
        }
        m_planeCallbacks.clear();

        for( vtkPlaneCollection* planeCol :  m_planeCollections )
        {
            planeCol->Delete();
        }
        m_planeCollections.clear();

        if (m_service)
        {
            m_service->setActorPropertyToUnclippedMaterial( false );
        }
    }

    //------------------------------------------------------------------------------

    virtual void Execute( vtkObject* caller, unsigned long eventId, void* data)
    {
/*        ::visuVTKAdaptor::SQMesh::sptr service;

        if (m_service.expired())
        {
            this->Stop();
            this->Delete();
            return;
        }

        if (eventId == vtkCommand::ModifiedEvent)
        {
            service = m_service.lock();

            this->Clear();

            vtkPlane* plane = NULL;
            for (  m_planeCollectionSrc->InitTraversal();
                   (plane = m_planeCollectionSrc->GetNextItem());
                   )
            {
                vtkPlane* newPlane = vtkPlane::New();
                m_planeCallbacks.push_back(PlaneShifterCallback::New(plane, newPlane, m_factor));

                vtkPlaneCollection* newCollection = vtkPlaneCollection::New();
                newCollection->AddItem(newPlane);

                ::fwServices::registry::ServiceFactory::sptr srvFactory =
                    ::fwServices::registry::ServiceFactory::getDefault();

                ::fwServices::IService::sptr srv = srvFactory->create("::visuVTKAdaptor::SQMesh");
                SLM_ASSERT("Service of type '::visuVTKAdaptor::SQMesh' cannot be instantiated.", srv);

                ::fwRenderVTK::IAdaptor::sptr meshService = ::fwRenderVTK::IAdaptor::dynamicCast(srv);
                SLM_ASSERT("Service of type '::visuVTKAdaptor::SQMesh' is not an adaptor", meshService);

                ::fwServices::OSR::registerServiceInput(service->getInput< ::fwData::Mesh >(SQMesh::s_MESH_INPUT),
                                                        SQMesh::s_MESH_INPUT, srv);

                ::visuVTKAdaptor::SQMesh::sptr meshAdaptor = SQMesh::dynamicCast(meshService);

                // Create the config to auto-connect the mesh to the new adaptor
                ::fwServices::IService::Config config;
                config.m_type              = "::visuVTKAdaptor::SQMesh";
                config.m_globalAutoConnect = false;
                config.m_config            = ::fwRuntime::EConfigurationElement::New("service");
                ::fwServices::IService::ObjectServiceConfig objConfig;
                objConfig.m_key         = SQMesh::s_MESH_INPUT;
                objConfig.m_access      = ::fwServices::IService::AccessType::INPUT;
                objConfig.m_autoConnect = true;
                objConfig.m_optional    = false;
                config.m_objects.push_back(objConfig);

                meshAdaptor->setConfiguration(config);
                meshAdaptor->setRenderService(service->getRenderService());
                meshAdaptor->setRendererId(service->getRendererId());
                meshAdaptor->setPickerId(service->getPickerId());
                meshAdaptor->setMaterial(service->getMaterial());
                meshAdaptor->setVtkClippingPlanes(newCollection);
                meshAdaptor->setShowClippedPart(false);
                meshAdaptor->setTransformId(service->getTransformId());

                meshAdaptor->start().wait();
                meshAdaptor->updateVisibility(service->getVisibility());

                m_planeCollections.push_back(newCollection);
                m_meshServices.push_back(meshAdaptor);

                newPlane->Delete();
            }

            bool hasItems = !m_meshServices.empty();
            service->setActorPropertyToUnclippedMaterial( hasItems );
        }
        else if ( eventId == vtkCommand::UserEvent )
        {
            for( ::visuVTKAdaptor::SQMesh::wptr adaptor :  m_meshServices )
            {
                if (!adaptor.expired())
                {
                    adaptor.lock()->updateVisibility(*(bool*)data);
                }
            }
        }*/
    }

protected:

    ::visuVTKAdaptor::SQMesh *m_service;

    vtkPlaneCollection* m_planeCollectionSrc;
    ::fwData::Mesh::sptr m_mesh;

    std::vector< std::weak_ptr<::visuVTKAdaptor::SQMesh> > m_meshServices;

    std::vector< PlaneShifterCallback* > m_planeCallbacks;
    std::vector< vtkPlaneCollection* >   m_planeCollections;

    double m_factor;
};

//------------------------------------------------------------------------------

SQMesh::SQMesh() noexcept :
    m_showClippedPart(true),
    m_autoResetCamera(true),
    m_polyData(nullptr),
    m_mapper(vtkPolyDataMapper::New()),
    m_actor(nullptr),
    m_clippingPlanes(nullptr),
    m_servicesStarterCallback(nullptr),
    m_transform(vtkTransform::New()),
    m_uvgen(NONE)
{
    m_unclippedPartMaterial = ::fwData::Material::New();
    m_material              = ::fwData::Material::New();
    m_unclippedPartMaterial->diffuse()->setRGBA("#aaaaff44");
    m_clippingPlanesId = "";

    m_sigTextureApplied = newSignal<TextureAppliedSignalType>(s_TEXTURE_APPLIED_SIG);

    newSlot(s_UPDATE_VISIBILITY_SLOT, &SQMesh::updateVisibility, this);
    newSlot(s_UPDATE_POINT_COLORS_SLOT, &SQMesh::updatePointColors, this);
    newSlot(s_UPDATE_CELL_COLORS_SLOT, &SQMesh::updateCellColors, this);
    newSlot(s_UPDATE_VERTEX_SLOT, &SQMesh::updateVertex, this);
    newSlot(s_UPDATE_POINT_NORMALS_SLOT, &SQMesh::updatePointNormals, this);
    newSlot(s_UPDATE_CELL_NORMALS_SLOT, &SQMesh::updateCellNormals, this);
    newSlot(s_UPDATE_POINT_TEX_COORDS_SLOT, &SQMesh::updatePointTexCoords, this);
    newSlot(s_UPDATE_CELL_TEX_COORDS_SLOT, &SQMesh::updateCellTexCoords, this);
    newSlot(s_SHOW_POINT_COLORS_SLOT, &SQMesh::showPointColors, this);
    newSlot(s_SHOW_CELL_COLORS_SLOT, &SQMesh::showCellColors, this);
    newSlot(s_HIDE_COLORS_SLOT, &SQMesh::hideColors, this);
    newSlot(s_UPDATE_COLOR_MODE_SLOT, &SQMesh::updateColorMode, this);
    newSlot(s_UPDATE_NORMAL_MODE_SLOT, &SQMesh::updateNormalMode, this);
    newSlot(s_UPDATE_MATRIX_FIELD_SLOT, &SQMesh::updateMatrixField, this);
}

//------------------------------------------------------------------------------

SQMesh::~SQMesh() noexcept
{
    m_clippingPlanes = nullptr;

    m_mapper->Delete();
    m_mapper = nullptr;

    m_transform->Delete();
    m_transform = nullptr;

    if(m_actor)
    {
        m_actor->Delete();
        m_actor = nullptr;
    }

    if (m_polyData)
    {
        m_polyData->Delete();
        m_polyData = nullptr;
    }
}

//------------------------------------------------------------------------------

void SQMesh::configuring()
{
    this->configureParams();

    const std::string color = get<QString>(m_configMap, "color", "#ffffffff").toStdString();
    const std::string unclippedColor = get<QString>(m_configMap, "unclippedcolor", "#aaaaff44").toStdString();

    m_material->diffuse()->setRGBA(color);
    m_unclippedPartMaterial->diffuse()->setRGBA(unclippedColor);


    const std::string autoresetcamera = get<QString>(m_configMap, "autoresetcamera", "yes").toStdString();
    SLM_ASSERT("'autoresetcamera' must be 'yes' or 'no'", autoresetcamera == "yes" || autoresetcamera == "no");
    m_autoResetCamera = (autoresetcamera == "yes");

    const std::string uvGen = get<QString>(m_configMap, "uvgen", "none").toStdString();

    if(uvGen == "none")
    {
        m_uvgen = NONE;
    }
    else if(uvGen == "sphere")
    {
        m_uvgen = SPHERE;
    }
    else if(uvGen == "cylinder")
    {
        m_uvgen = CYLINDER;
    }
    else if(uvGen == "plane")
    {
        m_uvgen = PLANE;
    }
    else
    {
        SLM_FATAL("'uvgen' value must be 'none', 'sphere', 'cylinder' or 'plane', actual: " + uvGen);
    }

    if (m_configMap.count("texture"))
    {
        SLM_FATAL("'texture' is deprecated, you need to connect manually the SQMesh::textureApplied signal to the "
                  "STexture::applyTexture slot.");
    }
    if (m_configMap.count("shadingMode"))
    {
        const std::string shading                         = get<QString>(m_configMap, "shadingMode").toStdString();
        const ::fwData::Material::ShadingType shadingMode = (shading == "ambient") ? ::fwData::Material::AMBIENT :
                                                            (shading == "flat") ? ::fwData::Material::FLAT :
                                                            (shading == "gouraud") ? ::fwData::Material::GOURAUD :
                                                            ::fwData::Material::PHONG;
        m_material->setShadingMode(shadingMode);
    }

    this->setClippingPlanesId(get<QString>(m_configMap, "clippingplane", "").toStdString());
}

//------------------------------------------------------------------------------

void SQMesh::updating()
{
    ::fwData::Mesh::csptr mesh = this->getInOut < ::fwData::Mesh >(s_MESH_INPUT);
    SLM_ASSERT("Missing mesh", mesh);
    this->updateMesh( mesh );
    this->requestRender();
}

//------------------------------------------------------------------------------

void SQMesh::starting()
{
    this->initialize();

    this->buildPipeline();

    this->requestRender();
}

//------------------------------------------------------------------------------

void SQMesh::stopping()
{
    this->removeAllPropFromRenderer();
    if (this->getPicker())
    {
        this->removeFromPicker(m_actor);
    }

    this->removeNormalsService();
    this->removeServicesStarterCommand();

    this->unregisterServices();

    m_connections.disconnect();
    this->requestRender();
}

//------------------------------------------------------------------------------

void SQMesh::createTransformService()
{
    ::fwData::Mesh::csptr mesh = this->getInOut < ::fwData::Mesh >(s_MESH_INPUT);
    SLM_ASSERT("Missing mesh", mesh);

    ::fwData::TransformationMatrix3D::sptr fieldTransform =
        mesh->getField< ::fwData::TransformationMatrix3D >(s_MATRIX_FIELD_NAME);

    if (fieldTransform)
    {
        auto prevTransformService = m_transformService.lock();
        if (prevTransformService)
        {
            auto trf = prevTransformService->getInOut< ::fwData::TransformationMatrix3D >(SQTransform::s_TM3D_INOUT);

            if (trf == fieldTransform)
            {
                return;
            }

            //this->unregisterService(prevTransformService);
            m_transformService.reset();
            m_transform->Pop();
        }

        vtkTransform* vtkFieldTransform = vtkTransform::New();
        vtkFieldTransform->Identity();

        // create the srv configuration for objects auto-connection
        auto transformService = this->registerService< ::visuVTKAdaptor::SQTransform>( "::visuVTKAdaptor::STransform");
        m_transformService = transformService;
        transformService->setProperty(SQTransform::s_TM3D_INOUT, QVariant::fromValue(new ::fwServices::QtObjectHolder(fieldTransform)));

        transformService->setRenderService( this->getRenderService() );
        transformService->setRendererId( this->getRendererId() );
        transformService->setTransform(vtkFieldTransform);
        transformService->start();

        m_transform->Concatenate(vtkFieldTransform);
        m_transform->Modified();
        vtkFieldTransform->Delete();
    }
}

//------------------------------------------------------------------------------

::fwData::Material::sptr SQMesh::getMaterial() const
{
    return m_material;
}

//------------------------------------------------------------------------------

::fwData::Material::sptr SQMesh::getUnclippedMaterial() const
{
    return m_unclippedPartMaterial;
}

//------------------------------------------------------------------------------

void SQMesh::setActorPropertyToUnclippedMaterial(bool opt)
{
    std::shared_ptr<::visuVTKAdaptor::SQMaterial> mat;

    if (opt)
    {
        SLM_ASSERT("Material service expired", !m_unclippedPartMaterialService.expired());
        mat = std::dynamic_pointer_cast<::visuVTKAdaptor::SQMaterial>(m_unclippedPartMaterialService.lock());
    }
    else
    {
        SLM_ASSERT("Material service expired", !m_materialService.expired());
        mat = std::dynamic_pointer_cast<::visuVTKAdaptor::SQMaterial>(m_materialService.lock());
    }

    SLM_ASSERT("Invalid Material Adaptor", mat);

    m_actor->SetProperty( mat->getVtkProperty() );
    this->setVtkPipelineModified();
}

//------------------------------------------------------------------------------

void SQMesh::setShowClippedPart(bool show)
{
    m_showClippedPart = show;
}

//------------------------------------------------------------------------------

void SQMesh::setClippingPlanesId(::fwRenderVTK::SRender::VtkObjectIdType id)
{
    m_clippingPlanesId = id;
}

//------------------------------------------------------------------------------

void SQMesh::setServiceOnMaterial(std::shared_ptr<::fwRenderVTK::IQAdaptor>& srv, ::fwData::Material::sptr material)
{
    if (!srv)
    {
        // create the srv configuration for objects auto-connection
        srv = this->registerService< ::fwRenderVTK::IQAdaptor>("::visuVTKAdaptor::SMaterial");
        srv->setProperty(SQMaterial::s_MATERIAL_INPUT, QVariant::fromValue(new ::fwServices::QtObjectHolder(material)));

        srv->setRenderService(this->getRenderService());
        srv->start();
        srv->update();
    }
}

//------------------------------------------------------------------------------

void SQMesh::setMaterial(::fwData::Material::sptr material)
{
    m_material = material;
}

//------------------------------------------------------------------------------

void SQMesh::setUnclippedPartMaterial(::fwData::Material::sptr material)
{
    m_unclippedPartMaterial = material;
}

//------------------------------------------------------------------------------

void SQMesh::updateOptionsMode()
{
    if (m_material->getOptionsMode() == ::fwData::Material::NORMALS)
    {
        this->createNormalsService();
    }
    else
    {
        this->removeNormalsService();
    }
}

//------------------------------------------------------------------------------

void SQMesh::createNormalsService()
{
    if ( m_normalsService.expired() )
    {
        ::fwData::Mesh::sptr mesh = this->getInOut < ::fwData::Mesh >(s_MESH_INPUT);
        SLM_ASSERT("Missing mesh", mesh);

        // create the srv configuration for objects auto-connection
        auto service = this->registerService< ::visuVTKAdaptor::SQMeshNormals >("::visuVTKAdaptor::SQMeshNormals");
        service->setProperty(SQMeshNormals::s_MESH_INPUT, QVariant::fromValue(new ::fwServices::QtObjectHolder(mesh)));

        service->setRenderService( this->getRenderService() );
        service->setRendererId( this->getRendererId()      );
        service->setPickerId( this->getPickerId()      );
        service->setPolyData( m_polyData );
        service->start();

        m_normalsService = service;
    }
}

//------------------------------------------------------------------------------

void SQMesh::removeNormalsService()
{
    if ( !m_normalsService.expired() )
    {
        this->unregisterService(m_normalsService.lock());
        m_normalsService.reset();
    }
}

//------------------------------------------------------------------------------

void SQMesh::buildPipeline()
{
    ::fwData::Mesh::sptr mesh = this->getInOut < ::fwData::Mesh >(s_MESH_INPUT);
    SLM_ASSERT("Missing mesh", mesh);

    if (!m_clippingPlanesId.empty())
    {
        vtkObject* o               = this->getVtkObject(m_clippingPlanesId);
        vtkPlaneCollection* planes = vtkPlaneCollection::SafeDownCast(o);
        this->setVtkClippingPlanes( planes );
    }

    std::shared_ptr<::fwRenderVTK::IQAdaptor> materialService;
    std::shared_ptr<::fwRenderVTK::IQAdaptor> unclippedPartMaterialService;

    this->setServiceOnMaterial(materialService, m_material);
    this->setServiceOnMaterial(unclippedPartMaterialService, m_unclippedPartMaterial);

    m_materialService              = materialService;
    m_unclippedPartMaterialService = unclippedPartMaterialService;

    if (!m_actor)
    {
        m_actor = this->newActor();

        if(!this->getTransformId().empty())
        {
            vtkTransform* t = m_renderService->getOrAddVtkTransform(m_transformId);
            m_transform->Concatenate(t);
        }
        m_actor->SetUserTransform(m_transform);

        this->createTransformService();

        this->addToRenderer(m_actor);
        if (this->getPicker())
        {
            this->addToPicker(m_actor);
        }
    }

    this->updateMesh( mesh );
    this->updateOptionsMode();

    this->setActorPropertyToUnclippedMaterial(false);

    this->removeServicesStarterCommand();

    if( m_clippingPlanes && m_showClippedPart )
    {
        this->createServicesStarterCommand();
    }
    this->setVtkPipelineModified();
}

//------------------------------------------------------------------------------

void SQMesh::updateMesh( ::fwData::Mesh::csptr mesh )
{
    if (m_polyData)
    {
        m_polyData->Delete();
        m_polyData = 0;
    }
    m_polyData = vtkPolyData::New();

    {
        ::fwData::mt::ObjectReadLock lock(mesh);
        ::fwVtkIO::helper::Mesh::toVTKMesh(mesh, m_polyData);
    }

    if(m_uvgen == SPHERE)
    {
        vtkSmartPointer<vtkTextureMapToSphere> texCoord = vtkSmartPointer<vtkTextureMapToSphere>::New();
        texCoord->SetInputData(m_polyData);
        m_mapper->SetInputConnection(texCoord->GetOutputPort());
    }
    else if(m_uvgen == CYLINDER)
    {
        vtkSmartPointer<vtkTextureMapToCylinder> texCoord = vtkSmartPointer<vtkTextureMapToCylinder>::New();
        texCoord->SetInputData(m_polyData);
        m_mapper->SetInputConnection(texCoord->GetOutputPort());
    }
    else if(m_uvgen == PLANE)
    {
        vtkSmartPointer<vtkTextureMapToPlane> texCoord = vtkSmartPointer<vtkTextureMapToPlane>::New();
        texCoord->SetInputData(m_polyData);
        m_mapper->SetInputConnection(texCoord->GetOutputPort());
    }
    else
    {
        m_mapper->SetInputData(m_polyData);
    }

    ::fwData::Material::sptr material = this->getMaterial();
    SLM_ASSERT("Missing material", material);
    m_sigTextureApplied->asyncEmit(material);
    auto objectHolder = new ::fwServices::QtObjectHolder(material);
    textureApplied(objectHolder);

    if (m_autoResetCamera)
    {
        this->getRenderer()->ResetCamera();
    }
    this->setVtkPipelineModified();
}

//------------------------------------------------------------------------------

vtkActor* SQMesh::newActor()
{
    vtkActor* actor = vtkActor::New();

    m_mapper->SetInputData(m_polyData);

    if (m_clippingPlanes)
    {
        m_mapper->SetClippingPlanes(m_clippingPlanes);
    }

    actor->SetMapper(m_mapper);
    this->setVtkPipelineModified();
    return actor;
}

//------------------------------------------------------------------------------

void SQMesh::updateVisibility( bool isVisible)
{
    if (m_actor)
    {
        m_actor->SetVisibility( isVisible );
    }

    if (m_servicesStarterCallback)
    {
        m_servicesStarterCallback->Execute(0, vtkCommand::UserEvent, &isVisible);
    }
    this->setVtkPipelineModified();
    this->requestRender();
}

//------------------------------------------------------------------------------

bool SQMesh::getVisibility() const
{
    bool visible = false;
    if (m_actor)
    {
        visible = m_actor->GetVisibility() == 1;
    }
    return visible;
}

//------------------------------------------------------------------------------

void SQMesh::setVtkClippingPlanes(vtkPlaneCollection* planes)
{
    m_clippingPlanes = planes;
}

//------------------------------------------------------------------------------

void SQMesh::createServicesStarterCommand()
{
    if(!m_servicesStarterCallback)
    {
        m_servicesStarterCallback         = PlaneCollectionAdaptorStarter::New( this, m_clippingPlanes, -1. );
    }
}

//------------------------------------------------------------------------------

void SQMesh::removeServicesStarterCommand()
{
    if(m_servicesStarterCallback)
    {
        m_servicesStarterCallback->Stop();
        m_servicesStarterCallback->Delete();
        m_servicesStarterCallback = 0;
    }
}

//------------------------------------------------------------------------------

void SQMesh::setAutoResetCamera(bool autoResetCamera)
{
    m_autoResetCamera = autoResetCamera;
}

//------------------------------------------------------------------------------

void SQMesh::updatePointColors()
{
    ::fwData::Mesh::csptr mesh = this->getInOut < ::fwData::Mesh >(s_MESH_INPUT);
    SLM_ASSERT("Missing mesh", mesh);
    SLM_ASSERT("m_polyData not instanced", m_polyData);

    ::fwVtkIO::helper::Mesh::updatePolyDataPointColor(m_polyData, mesh);
    this->setVtkPipelineModified();
    this->requestRender();
}

//------------------------------------------------------------------------------

void SQMesh::updateCellColors()
{
    ::fwData::Mesh::csptr mesh = this->getInOut < ::fwData::Mesh >(s_MESH_INPUT);
    SLM_ASSERT("Missing mesh", mesh);
    SLM_ASSERT("m_polyData not instanced", m_polyData);

    ::fwVtkIO::helper::Mesh::updatePolyDataCellColor(m_polyData, mesh);
    this->setVtkPipelineModified();
    this->requestRender();
}

//------------------------------------------------------------------------------

void SQMesh::updateVertex()
{
    ::fwData::Mesh::csptr mesh = this->getInOut < ::fwData::Mesh >(s_MESH_INPUT);
    SLM_ASSERT("Missing mesh", mesh);
    SLM_ASSERT("m_polyData not instanced", m_polyData);

    ::fwVtkIO::helper::Mesh::updatePolyDataPoints(m_polyData, mesh);

    if (m_autoResetCamera)
    {
        this->getRenderer()->ResetCamera();
    }
    this->setVtkPipelineModified();
    this->requestRender();
}

//------------------------------------------------------------------------------

void SQMesh::updatePointNormals()
{
    ::fwData::Mesh::csptr mesh = this->getInOut < ::fwData::Mesh >(s_MESH_INPUT);
    SLM_ASSERT("Missing mesh", mesh);
    ::fwVtkIO::helper::Mesh::updatePolyDataPointNormals(m_polyData, mesh);
    this->setVtkPipelineModified();
    this->requestRender();
}

//------------------------------------------------------------------------------

void SQMesh::updateCellNormals()
{
    ::fwData::Mesh::csptr mesh = this->getInOut < ::fwData::Mesh >(s_MESH_INPUT);
    SLM_ASSERT("Missing mesh", mesh);
    ::fwVtkIO::helper::Mesh::updatePolyDataCellNormals(m_polyData, mesh);
    this->setVtkPipelineModified();
    this->requestRender();
}

//------------------------------------------------------------------------------

void SQMesh::updatePointTexCoords()
{
    ::fwData::Mesh::csptr mesh = this->getInOut < ::fwData::Mesh >(s_MESH_INPUT);
    SLM_ASSERT("Missing mesh", mesh);
    ::fwVtkIO::helper::Mesh::updatePolyDataPointTexCoords(m_polyData, mesh);
    this->setVtkPipelineModified();
    this->requestRender();
}

//------------------------------------------------------------------------------

void SQMesh::updateCellTexCoords()
{
    ::fwData::Mesh::csptr mesh = this->getInOut < ::fwData::Mesh >(s_MESH_INPUT);
    SLM_ASSERT("Missing mesh", mesh);
    ::fwVtkIO::helper::Mesh::updatePolyDataCellTexCoords(m_polyData, mesh);
    this->setVtkPipelineModified();
    this->requestRender();
}

//------------------------------------------------------------------------------

void SQMesh::showPointColors()
{
    m_mapper->ScalarVisibilityOn();
    m_mapper->SetScalarModeToUsePointData();
    this->setVtkPipelineModified();
    this->requestRender();
}

//------------------------------------------------------------------------------

void SQMesh::showCellColors()
{
    m_mapper->ScalarVisibilityOn();
    m_mapper->SetScalarModeToUseCellData();
    this->setVtkPipelineModified();
    this->requestRender();
}

//------------------------------------------------------------------------------

void SQMesh::hideColors()
{
    m_mapper->ScalarVisibilityOff();
    this->setVtkPipelineModified();
    this->requestRender();
}

//------------------------------------------------------------------------------

void SQMesh::updateColorMode(std::uint8_t mode)
{
    switch (mode)
    {
        case 0:
        {
            this->hideColors();
            break;
        }
        case 1:
        {
            this->showPointColors();
            break;
        }
        case 2:
        {
            this->showCellColors();
            break;
        }
        default:
        {
            OSLM_ERROR("mode " << mode << " is not allowed");
            break;
        }
    }
}

//------------------------------------------------------------------------------

void SQMesh::updateNormalMode(std::uint8_t mode)
{
    this->updateOptionsMode();
    if (!m_normalsService.expired())
    {
        std::shared_ptr<::visuVTKAdaptor::SQMeshNormals> normalsAdaptor =
            std::dynamic_pointer_cast<::visuVTKAdaptor::SQMeshNormals>(m_normalsService.lock());
        normalsAdaptor->updateNormalMode(mode);
    }
}

//------------------------------------------------------------------------------

void SQMesh::updateMatrixField(::fwData::Object::FieldsContainerType fields)
{
    if (fields.find(s_MATRIX_FIELD_NAME) != fields.end())
    {
        this->createTransformService();
    }
}

//------------------------------------------------------------------------------

} //namespace visuVTKAdaptor
