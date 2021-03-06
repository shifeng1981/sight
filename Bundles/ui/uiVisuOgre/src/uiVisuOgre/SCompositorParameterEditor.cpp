/************************************************************************
 *
 * Copyright (C) 2014-2018 IRCAD France
 * Copyright (C) 2014-2018 IHU Strasbourg
 *
 * This file is part of Sight.
 *
 * Sight is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Sight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Sight. If not, see <https://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#include "uiVisuOgre/SCompositorParameterEditor.hpp"

#include <uiVisuOgre/helper/ParameterEditor.hpp>

#include <fwCom/Slots.hxx>

#include <fwData/Boolean.hpp>
#include <fwData/Float.hpp>
#include <fwData/Integer.hpp>

#include <fwGui/GuiRegistry.hpp>

#include <fwGuiQt/container/QtContainer.hpp>

#include <fwRenderOgre/IAdaptor.hpp>
#include <fwRenderOgre/SRender.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/op/Add.hpp>

#include <QWidget>

namespace uiVisuOgre
{

fwServicesRegisterMacro( ::fwGui::editor::IEditor, ::uiVisuOgre::SCompositorParameterEditor);

const ::fwCom::Slots::SlotKeyType SCompositorParameterEditor::s_UPDATE_COMPOSITOR_SLOT = "updateCompositor";

//------------------------------------------------------------------------------
SCompositorParameterEditor::SCompositorParameterEditor() noexcept
{
    newSlot(s_UPDATE_COMPOSITOR_SLOT, &SCompositorParameterEditor::updateCompositor, this);
}

//------------------------------------------------------------------------------

SCompositorParameterEditor::~SCompositorParameterEditor() noexcept
{
}

//------------------------------------------------------------------------------

void SCompositorParameterEditor::configuring()
{
    this->initialize();

    auto config = this->getConfigTree();

    m_layerID = config.get<std::string>("layer.<xmlattr>.id", "");
}

//------------------------------------------------------------------------------

void SCompositorParameterEditor::starting()
{
    this->create();

    auto qtContainer = ::fwGuiQt::container::QtContainer::dynamicCast(this->getContainer() );
    m_sizer = new QVBoxLayout();
    m_sizer->setContentsMargins(0, 0, 0, 0);

    qtContainer->setLayout(m_sizer);

    this->updating();
}

//------------------------------------------------------------------------------

void SCompositorParameterEditor::stopping()
{
    this->clear();
    this->destroy();
}

//------------------------------------------------------------------------------

void SCompositorParameterEditor::updating()
{
}

//------------------------------------------------------------------------------

void SCompositorParameterEditor::updateCompositor(std::string /*_compositorName*/, bool /*_enabled*/,
                                                  fwRenderOgre::Layer::sptr _layer)
{
    if(_layer->getLayerID() == m_layerID)
    {
        // We will create a new layout so clear everything before
        this->clear();

        bool found = false;

        const auto adaptors = _layer->getRegisteredAdaptors();

        // Is there at least one parameter that we can handle ?
        for (const auto& wAdaptor : adaptors)
        {
            const auto adaptor = wAdaptor.lock();
            if (adaptor->getClassname() == "::visuOgreAdaptor::SCompositorParameter")
            {
                /// Filter object types
                const ::fwData::Object::csptr shaderObj =
                    adaptor->getInOut< ::fwData::Object>(::fwRenderOgre::IParameter::s_PARAMETER_INOUT);
                const auto& objType = shaderObj->getClassname();

                if(objType == "::fwData::Boolean" || objType == "::fwData::Float" || objType == "::fwData::Integer")
                {
                    found = true;
                    break;
                }
            }
        }

        if(!found)
        {
            return;
        }

        /// Getting this widget's container
        auto qtContainer   = ::fwGuiQt::container::QtContainer::dynamicCast( this->getContainer() );
        QWidget* container = qtContainer->getQtContainer();

        QWidget* p2 = new QWidget( container );
        m_editorInfo.editorPanel = ::fwGuiQt::container::QtContainer::New();
        m_editorInfo.editorPanel->setQtContainer(p2);

        const std::string uuid = this->getID();
        m_editorInfo.uuid = uuid + "-editor";

        ::fwGui::GuiRegistry::registerSIDContainer(m_editorInfo.uuid, m_editorInfo.editorPanel);

        auto editorService = ::fwServices::add( "::guiQt::editor::SParameters", m_editorInfo.uuid );
        m_editorInfo.service = editorService;

        ::fwServices::IService::ConfigType editorConfig;

        // Get all ShaderParameter subservices from the corresponding Material adaptor
        for (const auto& wAdaptor : adaptors)
        {
            const auto adaptor = wAdaptor.lock();
            if (adaptor->getClassname() == "::visuOgreAdaptor::SCompositorParameter")
            {
                auto paramAdaptor = ::fwRenderOgre::IParameter::dynamicConstCast(adaptor);
                auto paramConfig  = ::uiVisuOgre::helper::ParameterEditor::createConfig(paramAdaptor,
                                                                                        m_editorInfo.service.lock(),
                                                                                        m_editorInfo.connections);

                if(!paramConfig.empty())
                {
                    editorConfig.add_child("parameters.param", paramConfig);
                }
            }
        }

        editorService->setConfiguration(editorConfig);
        editorService->configure();

        editorService->start();

        this->fillGui();
    }
}

//------------------------------------------------------------------------------

void SCompositorParameterEditor::clear()
{
    ::fwServices::IService::sptr objService = m_editorInfo.service.lock();

    if(objService)
    {
        objService->stop();

        ::fwGui::GuiRegistry::unregisterSIDContainer(m_editorInfo.uuid);

        ::fwServices::OSR::unregisterService(objService);

        m_sizer->removeWidget(m_editorInfo.editorPanel->getQtContainer());
        m_editorInfo.editorPanel->destroyContainer();
        m_editorInfo.editorPanel.reset();
    }
}

//------------------------------------------------------------------------------

void SCompositorParameterEditor::fillGui()
{
    auto editorService = m_editorInfo.service.lock();
    if(editorService)
    {
        m_sizer->addWidget( m_editorInfo.editorPanel->getQtContainer(), 0 );
    }
}

//------------------------------------------------------------------------------

} // namespace uiVisuOgre
