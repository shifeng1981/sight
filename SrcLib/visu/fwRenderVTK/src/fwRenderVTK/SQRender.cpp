/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwRenderVTK/SQRender.hpp"

#include "fwRenderVTK/IAdaptor.hpp"
#include "fwRenderVTK/IVtkRenderWindowInteractorManager.hpp"
#include "fwRenderVTK/OffScreenInteractorManager.hpp"
#include "fwRenderVTK/registry/adaptors.hpp"
#include "fwRenderVTK/vtk/InteractorStyle3DForNegato.hpp"

#include <fwCom/Signal.hxx>
#include <fwCom/Slot.hxx>
#include <fwCom/Slots.hxx>

#include <fwData/Color.hpp>
#include <fwData/mt/ObjectWriteLock.hpp>

#include <fwRuntime/ConfigurationElementContainer.hpp>
#include <fwRuntime/utils/GenericExecutableFactoryRegistrar.hpp>

#include <fwServices/helper/Config.hpp>
#include <fwServices/macros.hpp>
#include <fwServices/op/Add.hpp>
#include <fwServices/QtQmlType.hxx>

#include <fwThread/Timer.hpp>

#include <fwTools/fwID.hpp>

#include <fwVtkIO/vtk.hpp>

#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/optional.hpp>

#include <vtkCellPicker.h>
#include <vtkImageFlip.h>
#include <vtkInstantiator.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkWindowToImageFilter.h>

#include <functional>

using namespace fwServices;

namespace fwRenderVTK
{
const ::fwCom::Signals::SignalKeyType SQRender::s_DROPPED_SIG         = "dropped";
const ::fwCom::Slots::SlotKeyType SQRender::s_RENDER_SLOT             = "render";
const ::fwCom::Slots::SlotKeyType SQRender::s_REQUEST_RENDER_SLOT     = "requestRender";
const ::fwCom::Slots::SlotKeyType SQRender::s_TOGGLE_AUTO_RENDER_SLOT = "toggleAutoRender";

static const ::fwServices::IService::KeyType s_OFFSCREEN_INOUT = "offScreen";

static ::fwServices::QtQmlType<SQRender>   registar("com.fw4spl.vtk", 1, 0, "SRender");

//-----------------------------------------------------------------------------

SQRender::SQRender() noexcept :
    m_pendingRenderRequest(false),
    m_renderMode(RenderMode::AUTO),
    m_width(160),
    m_height(90),
    m_targetFrameRate(30),
    m_offScreen(false),
    m_flip(false),
    m_inout(nullptr),
    m_target(nullptr)
{
    newSignal<DroppedSignalType>(s_DROPPED_SIG);

    newSlot(s_RENDER_SLOT, &SQRender::render, this);
    newSlot(s_REQUEST_RENDER_SLOT, &SQRender::requestRender, this);
    newSlot(s_TOGGLE_AUTO_RENDER_SLOT, &SQRender::toggleAutoRender, this);
}

//-----------------------------------------------------------------------------

SQRender::~SQRender() noexcept
{
}

//-----------------------------------------------------------------------------

void SQRender::configureRenderer( const ConfigType& rendererConf )
{
    const std::string id         = get<QString>(rendererConf, "id").toStdString();
    const std::string background = get<QString>(rendererConf, "background", "").toStdString();

    if(m_renderers.count(id) == 0)
    {
        m_renderers[id] = vtkRenderer::New();

//vtk depth peeling not available on android (Offscreen rendering issues)
#ifndef ANDROID
        m_renderers[id]->SetUseDepthPeeling( 1 );
        m_renderers[id]->SetMaximumNumberOfPeels( 8  );
#endif

        const int layer = get<int>(rendererConf, "layer", m_renderers[id]->GetLayer());
        m_renderers[id]->SetLayer(layer);
    }

    if ( !background.empty() )
    {
        if(background[0] == '#')
        {
            ::fwData::Color::sptr color = ::fwData::Color::New();
            color->setRGBA(background);
            m_renderers[id]->SetBackground(color->getRGBA()[0], color->getRGBA()[1], color->getRGBA()[2]);
        }
        else
        {
            // compatibility with "old" color
            double color = std::stod(background);
            m_renderers[id]->SetBackground(color, color, color);
        }
    }
}

//-----------------------------------------------------------------------------

void SQRender::configurePicker( const ConfigType& pickerConf )
{
    const std::string id       = get<QString>(pickerConf, "id").toStdString();
    const std::string vtkclass = get<QString>(pickerConf, "vtkclass", "vtkCellPicker").toStdString();
    const double tolerance     = get<double>(pickerConf, "tolerance", 0.0);

    if(m_pickers.count(id) == 0)
    {
        m_pickers[id] = vtkAbstractPropPicker::SafeDownCast(vtkInstantiator::CreateInstance(vtkclass.c_str()));
        OSLM_ASSERT("'" << vtkclass.c_str() << "' instantiation failled.", m_pickers[id]);
        m_pickers[id]->InitializePickList();
        m_pickers[id]->PickFromListOn();
        vtkPicker* picker = vtkPicker::SafeDownCast(m_pickers[id]);
        if (picker)
        {
            picker->SetTolerance(tolerance);
        }
    }
}

//-----------------------------------------------------------------------------

void SQRender::configureVtkObject( const ConfigType& vtkObjectConf )
{
    const std::string id       = get<QString>(vtkObjectConf, "id").toStdString();
    const std::string vtkClass = get<QString>(vtkObjectConf, "class").toStdString();

    SLM_ASSERT("Empty 'id'.", !id.empty() );
    SLM_ASSERT("Empty 'class'.", !vtkClass.empty() );

    if( m_vtkObjects.count(id) == 0 )
    {
        if ( vtkClass == "vtkTransform" )
        {
            m_vtkObjects[id] = createVtkTransform(vtkObjectConf);
        }
        else
        {
            m_vtkObjects[id] = vtkInstantiator::CreateInstance(vtkClass.c_str());
        }
    }
}

//-----------------------------------------------------------------------------

vtkTransform* SQRender::createVtkTransform( const ConfigType& vtkObjectConf )
{
    vtkTransform* newMat = vtkTransform::New();

/*    auto vtkTransform = get<QVariant>(vtkObjectConf, "vtkTransform");

    if (!vtkTransform.isNull())
    {
        const std::string& transformId = get<QString>(vtkTransform, "data");

        vtkTransform* mat = vtkTransform::SafeDownCast( getVtkObject(transformId) );

        SLM_ASSERT("No transform named '" + transformId + "'.", mat != nullptr);

        const std::string inverse = v.second.get<std::string>("<xmlattr>.inverse", "no");

        SLM_ASSERT("Inverse must be 'yes' or 'no'.", inverse == "yes" || inverse == "no");

        if(inverse == "yes")
        {
            newMat->Concatenate( mat->GetLinearInverse() );
        }
        else
        {
            newMat->Concatenate( mat );
        }

    }*/
/*
    const ::boost::optional<const ConfigType&> vtkTransformConf = vtkObjectConf.get_child_optional("vtkTransform");
    if(vtkTransformConf.is_initialized())
    {
        BOOST_FOREACH(const ::boost::property_tree::ptree::value_type& v, vtkTransformConf.get())
        {
            SLM_ASSERT("Invalid markup '" + v.first + "', 'concatenate' must be used here.", v.first == "concatenate");

            const std::string& transformId = v.second.data();

            vtkTransform* mat = vtkTransform::SafeDownCast( getVtkObject(transformId) );

            SLM_ASSERT("No transform named '" + transformId + "'.", mat != nullptr);

            const std::string inverse = v.second.get<std::string>("<xmlattr>.inverse", "no");

            SLM_ASSERT("Inverse must be 'yes' or 'no'.", inverse == "yes" || inverse == "no");

            if(inverse == "yes")
            {
                newMat->Concatenate( mat->GetLinearInverse() );
            }
            else
            {
                newMat->Concatenate( mat );
            }
        }
    }*/

    return newMat;
}

//-----------------------------------------------------------------------------

void SQRender::addVtkObject( const VtkObjectIdType& _id, vtkObject* _vtkObj )
{
    SLM_ASSERT( "vtkObject id is empty", !_id.empty() );
    SLM_ASSERT( "vtkObject is NULL", _vtkObj );

    if( m_vtkObjects.count(_id) == 0 )
    {
        m_vtkObjects[_id] = _vtkObj;
    }
}

//-----------------------------------------------------------------------------

void SQRender::configuring()
{
    const std::string renderMode = get<QString>(m_sceneConf, "renderMode", "auto").toStdString();

    if (renderMode == "auto")
    {
        m_renderMode = RenderMode::AUTO;
    }
    else if (renderMode == "timer")
    {
        m_renderMode = RenderMode::TIMER;
    }
    else if (renderMode == "none")
    {
        m_renderMode = RenderMode::NONE;
    }
    else
    {
        SLM_WARN_IF("renderMode '" + renderMode + " is unknown, setting renderMode to 'auto'.",
                    !renderMode.empty());
    }
    /// Target frame rate (default 30Hz)
    if(m_renderMode == RenderMode::TIMER)
    {
        SLM_ASSERT("Prevent division by 0.", m_targetFrameRate != 0);
        unsigned int timeStep = static_cast<unsigned int>( 1000.f / m_targetFrameRate);
        m_timer = m_associatedWorker->createTimer();

        ::fwThread::Timer::TimeDurationType duration = std::chrono::milliseconds(timeStep);
        m_timer->setFunction( std::bind( &SQRender::requestRender, this)  );
        m_timer->setDuration(duration);
    }
}

//-----------------------------------------------------------------------------

void SQRender::starting()
{
    this->startContext();

    auto rendererList = get<QVariantList>(m_sceneConf, "rendererList");
    for (QVariant const& renderer : rendererList)
    {
        SLM_ASSERT("Renderer configuration must be a dictionnary", renderer.canConvert<QVariantMap>());
        auto rendererConf = renderer.value<QVariantMap>();

        this->configureRenderer(rendererConf);
    }
    auto pickerList = get<QVariantList>(m_sceneConf, "pickerList");
    for (QVariant const& picker : pickerList)
    {
        SLM_ASSERT("Picker configuration must be a dictionnary", picker.canConvert<QVariantMap>());
        auto pickerConf = picker.value<QVariantMap>();

        this->configurePicker(pickerConf);
    }
    auto vtkObjectList = get<QVariantList>(m_sceneConf, "vtkObjectList");
    for (QVariant const& vtkObject : vtkObjectList)
    {
        SLM_ASSERT("vtkObject configuration must be a dictionnary", vtkObject.canConvert<QVariantMap>());
        auto vtkObjectConf = vtkObject.value<QVariantMap>();

        this->configureVtkObject(vtkObjectConf);
    }

    m_target->getRenderWindow()->SetNumberOfLayers(static_cast<int>(m_renderers.size()));
    for( RenderersMapType::iterator iter = m_renderers.begin(); iter != m_renderers.end(); ++iter )
    {
        vtkRenderer* renderer = (*iter).second;
        m_target->getRenderWindow()->AddRenderer(renderer);
    }

    if(m_timer)
    {
        m_timer->start();
    }
}

//-----------------------------------------------------------------------------

void SQRender::stopping()
{
    if(m_timer)
    {
        m_timer->stop();
    }

    this->stopContext();

    if (!m_offScreen)
    {
        this->destroy();
    }
}

//-----------------------------------------------------------------------------

void SQRender::updating()
{
}

//-----------------------------------------------------------------------------

void SQRender::render()
{
    m_target->getRenderWindow()->GetInteractor()->Render();
    this->setPendingRenderRequest(false);
}

//-----------------------------------------------------------------------------

bool SQRender::isShownOnScreen()
{
    return true;
/*    if (!m_offScreen)
    {
        return this->getContainer()->isShownOnScreen();
    }
    else
    {
        return true;
    }
    return false;*/
}

//-----------------------------------------------------------------------------

void SQRender::requestRender()
{
    if ( this->isShownOnScreen() && !this->getPendingRenderRequest())
    {
        this->setPendingRenderRequest(true);
        this->slot(SQRender::s_RENDER_SLOT)->run();
    }
}

//-----------------------------------------------------------------------------

void SQRender::toggleAutoRender()
{
    if(m_renderMode == RenderMode::AUTO)
    {
        m_renderMode = RenderMode::NONE;
    }
    else if(m_renderMode == RenderMode::NONE)
    {
        m_renderMode = RenderMode::AUTO;
    }

    auto interactor      = m_target->getRenderWindow()->GetInteractor()->GetInteractorStyle();
    auto interactorStyle = dynamic_cast< IInteractorStyle* >(interactor);
    interactorStyle->setAutoRender(m_renderMode == RenderMode::AUTO);
}

//-----------------------------------------------------------------------------

void SQRender::startContext()
{
    auto interactor = InteractorStyle3DForNegato::New();
    SLM_ASSERT("Can't instantiate interactor", interactor);
    interactor->setAutoRender(m_renderMode == RenderMode::AUTO);
    m_target->getRenderWindow()->GetInteractor()->SetInteractorStyle( interactor );
}

//-----------------------------------------------------------------------------

void SQRender::stopContext()
{
    for( RenderersMapType::iterator iter = m_renderers.begin(); iter != m_renderers.end(); ++iter )
    {
        vtkRenderer* renderer = iter->second;
        renderer->InteractiveOff();
        m_target->getRenderWindow()->RemoveRenderer(renderer);
        renderer->Delete();
    }

    m_renderers.clear();
}

//-----------------------------------------------------------------------------

vtkRenderer* SQRender::getRenderer(RendererIdType rendererId)
{
    OSLM_ASSERT("Renderer not found : '" << rendererId << "'", m_renderers.count(rendererId) == 1);

    return m_renderers[rendererId];
}

//-----------------------------------------------------------------------------

vtkAbstractPropPicker* SQRender::getPicker(PickerIdType pickerId)
{
    PickersMapType::const_iterator iter = m_pickers.find(pickerId);
    if ( iter == m_pickers.end())
    {
        SLM_DEBUG("Picker '" + pickerId + "' not found");
        return nullptr;
    }
    return iter->second;
}

//-----------------------------------------------------------------------------

vtkObject* SQRender::getVtkObject(const VtkObjectIdType& objectId) const
{
    VtkObjectMapType::const_iterator iter = m_vtkObjects.find(objectId);
    if ( iter == m_vtkObjects.end())
    {
        SLM_DEBUG("vtkObject '" + objectId + "' not found");
        return nullptr;
    }
    return iter->second;
}

//-----------------------------------------------------------------------------

vtkTransform* SQRender::getOrAddVtkTransform( const VtkObjectIdType& _id )
{
    vtkTransform* t = vtkTransform::SafeDownCast(getVtkObject(_id));
    if(t == 0)
    {
        t = vtkTransform::New();
        this->addVtkObject(_id, t);
    }
    return t;
}

//-----------------------------------------------------------------------------

bool SQRender::isOffScreen() const
{
    return m_offScreen;
}

//-----------------------------------------------------------------------------

void SQRender::setOffScreenRenderSize(unsigned int _width, unsigned int _height)
{
/*    if(m_offScreen)
    {

        // The MakeCurrent() is really essential otherwise this breaks the rendering of other rendering windows
        m_interactorManager->getInteractor()->GetRenderWindow()->MakeCurrent();
        m_interactorManager->getInteractor()->GetRenderWindow()->SetSize(static_cast<int>(m_width),
                                                                         static_cast<int>(m_height));
    }*/
}

//------------------------------------------------------------------------------

QVariantMap const& SQRender::scene() const
{
    return m_sceneConf;
}

//------------------------------------------------------------------------------

void SQRender::setScene(QVariantMap const& scene)
{
    m_sceneConf = scene;
}

//------------------------------------------------------------------------------

FrameBufferItem* SQRender::getTarget() const
{
    return m_target;
}

//-----------------------------------------------------------------------------

} //namespace fwRenderVTK
