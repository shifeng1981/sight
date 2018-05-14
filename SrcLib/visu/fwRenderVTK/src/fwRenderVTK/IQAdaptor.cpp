/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwRenderVTK/IQAdaptor.hpp"

#include "fwRenderVTK/registry/adaptors.hpp"

#include <fwData/String.hpp>

#include <fwRuntime/EConfigurationElement.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/registry/ObjectService.hpp>

#include <fwTools/fwID.hpp>

#include <vtkAbstractPropPicker.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkTransform.h>

namespace fwRenderVTK
{

IQAdaptor::IQAdaptor() noexcept :
    m_vtkPipelineModified(true),
    m_rendererId("default"),
    m_pickerId(""),
    // by default no Picker
    m_transformId(""),
    m_renderService(nullptr),
    // by default no Transform
    m_propCollection( vtkPropCollection::New() )
{
}

IQAdaptor::~IQAdaptor() noexcept
{
    std::cout << "Adaptor deleted" << std::endl;
    m_propCollection->Delete();
}

//------------------------------------------------------------------------------

void IQAdaptor::configureParams()
{
    this->setPickerId(get<QString>(m_configMap, "picker", "").toStdString());
    this->setRendererId(get<QString>(m_configMap, "renderer", "").toStdString());
    this->setTransformId(get<QString>(m_configMap, "transform", "").toStdString());
}

//------------------------------------------------------------------------------

void IQAdaptor::initialize()
{
    SLM_ASSERT("Render service not set", m_renderService);
}

//------------------------------------------------------------------------------

void IQAdaptor::setRenderService( SQRender* service)
{
    /// Preconditions
    SLM_ASSERT("service not instanced", service);

    m_renderService = service;
}

//------------------------------------------------------------------------------

void IQAdaptor::setRendererId(SQRender::RendererIdType newID)
{
    m_rendererId = newID;
}

//------------------------------------------------------------------------------

void IQAdaptor::setVtkPipelineModified()
{
    m_vtkPipelineModified = true;
}

//------------------------------------------------------------------------------

bool IQAdaptor::getAutoRender() const
{
    return (m_renderService->getRenderMode() == SQRender::RenderMode::AUTO);
}

//------------------------------------------------------------------------------

void IQAdaptor::requestRender()
{
    if ( (this->getRenderService()->getStatus() == ::fwServices::IService::STARTED ||
          this->getRenderService()->getStatus() == ::fwServices::IService::SWAPPING)
         && this->getRenderService()->isShownOnScreen()
         && m_vtkPipelineModified && this->getAutoRender() )
    {
        if ( !this->getRenderService()->getPendingRenderRequest())
        {
            std::cout << "Worker : " << m_associatedWorker << std::endl;
            this->getRenderService()->setPendingRenderRequest(true);
            this->getRenderService()->slot(SQRender::s_RENDER_SLOT)->run();
        }
        m_vtkPipelineModified = false;
    }
}

//------------------------------------------------------------------------------

SQRender::RendererIdType IQAdaptor::getRendererId() const
{
    return m_rendererId;
}

//------------------------------------------------------------------------------

SQRender* IQAdaptor::getRenderService() const
{
    SLM_ASSERT("SQRender service is not initialized, initialize() method must be called first",
               m_renderService);
    return m_renderService;
}

//------------------------------------------------------------------------------

vtkRenderer* IQAdaptor::getRenderer()
{
    return m_renderService->getRenderer(m_rendererId);
}

//------------------------------------------------------------------------------

void IQAdaptor::setPickerId(SQRender::PickerIdType newID)
{
    m_pickerId = newID;
}

//------------------------------------------------------------------------------

SQRender::PickerIdType IQAdaptor::getPickerId() const
{
    return m_pickerId;
}

//------------------------------------------------------------------------------

vtkAbstractPropPicker* IQAdaptor::getPicker(std::string pickerId)
{
    if (pickerId.empty())
    {
        pickerId = m_pickerId;
    }
    return m_renderService->getPicker(pickerId);
}

//------------------------------------------------------------------------------

void IQAdaptor::setTransformId(SQRender::VtkObjectIdType newID)
{
    m_transformId = newID;
}

//------------------------------------------------------------------------------

SQRender::VtkObjectIdType IQAdaptor::getTransformId() const
{
    return m_transformId;
}

//------------------------------------------------------------------------------

vtkTransform* IQAdaptor::getTransform()
{
    SLM_ASSERT("Transform id must be defined", !m_transformId.empty());
    return m_renderService->getOrAddVtkTransform(m_transformId);
}

//------------------------------------------------------------------------------

vtkObject* IQAdaptor::getVtkObject(const SQRender::VtkObjectIdType& objectId) const
{
    if (!objectId.empty())
    {
        return m_renderService->getVtkObject(objectId);
    }
    return nullptr;
}

//------------------------------------------------------------------------------

vtkRenderWindowInteractor* IQAdaptor::getInteractor()
{
    return this->getRenderer()->GetRenderWindow()->GetInteractor();
}

//------------------------------------------------------------------------------

IQAdaptor *IQAdaptor::getAssociatedAdaptor(vtkProp* prop, int depth)
{
    IQAdaptor *srv;

    if (prop)
    {
        if ( m_propCollection->IsItemPresent(prop) )
        {
            srv = this;
        }
        else
        {
            IQAdaptor *res;
            for( auto& service : m_serviceList)
            {

                auto adaptor = service.value<::fwRenderVTK::IQAdaptor *>();
                if(adaptor)
                {
                    res = adaptor->getAssociatedAdaptor(prop, depth - 1 );
                    if (res)
                    {
                        break;
                    }
                }
            }
            srv = ( res && depth == 0 ) ? this : res;
        }
    }
    return srv;
}

//------------------------------------------------------------------------------

void IQAdaptor::registerProp(vtkProp* prop)
{
    getProps(m_propCollection, prop);
}

//------------------------------------------------------------------------------

void IQAdaptor::getProps(vtkPropCollection* propc, vtkProp* prop)
{
    int initSize = propc->GetNumberOfItems();

    prop->GetActors(propc);
    prop->GetActors2D(propc);
    prop->GetVolumes(propc);

    if (initSize == propc->GetNumberOfItems())
    {
        propc->AddItem(prop);
    }
}

//------------------------------------------------------------------------------

void IQAdaptor::getAllSubProps(vtkPropCollection* propc, int depth)
{
    vtkProp* prop;

    m_propCollection->InitTraversal();
    while ( (prop = m_propCollection->GetNextProp()) )
    {
        getProps(propc, prop);
    }

    if(depth != 0)
    {
        for( const auto& service : m_serviceList)
        {
            auto adaptor = service.value<::fwRenderVTK::IQAdaptor *>();
            if(adaptor)
            {
                adaptor->getAllSubProps( propc, depth - 1 );
            }
        }
    }
}

//------------------------------------------------------------------------------

void IQAdaptor::unregisterProps()
{
    m_propCollection->RemoveAllItems();
}

//------------------------------------------------------------------------------

void IQAdaptor::addToRenderer(vtkProp* prop)
{
    this->registerProp(prop);
    this->getRenderer()->AddViewProp(prop);
    this->setVtkPipelineModified();
}

//------------------------------------------------------------------------------

void IQAdaptor::addToPicker(vtkProp* prop, std::string pickerId)
{
    OSLM_ASSERT("Picker '"<< pickerId << "' undefined.", this->getPicker(pickerId));
    this->getPicker(pickerId)->AddPickList(prop);
    this->setVtkPipelineModified();
}

//------------------------------------------------------------------------------

void IQAdaptor::removeFromPicker(vtkProp* prop, std::string pickerId)
{
    OSLM_ASSERT("Picker '"<< pickerId << "' undefined.", this->getPicker(pickerId));
    this->getPicker(pickerId)->DeletePickList(prop);
    this->setVtkPipelineModified();
}

//------------------------------------------------------------------------------

void IQAdaptor::removeAllPropFromRenderer()
{
    vtkPropCollection* propc = m_propCollection;
    vtkProp* prop;

    propc->InitTraversal();
    while ( (prop = propc->GetNextProp()) )
    {
        this->getRenderer()->RemoveViewProp(prop);
    }
    this->unregisterProps();
    this->setVtkPipelineModified();
}

} // namespace fwRenderVTK
