/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "uiIO/editor/SQIOSelector.hpp"

#include <fwCom/Signal.hxx>
#include <fwCom/Slots.hpp>
#include <fwCom/Slots.hxx>

#include <fwCore/base.hpp>

#include <fwData/Composite.hpp>

#include <fwDataTools/helper/Composite.hpp>

#include <fwGui/Cursor.hpp>
#include <fwGui/dialog/MessageDialog.hpp>
#include <fwGui/dialog/SelectorDialog.hpp>

#include <fwIO/IReader.hpp>
#include <fwIO/IWriter.hpp>

#include <fwRuntime/ConfigurationElement.hpp>
#include <fwRuntime/helper.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/op/Add.hpp>
#include <fwServices/QtQmlType.hxx>
#include <fwServices/registry/ServiceConfig.hpp>
#include <fwServices/registry/ServiceFactory.hpp>

#include <boost/foreach.hpp>

#include <sstream>
#include <string>

namespace uiIO
{

namespace editor
{

// Qml expose
static ::fwServices::QtQmlType<SQIOSelector>   registar("com.fw4spl.uiIO", 1, 0, "SIOSelector");

static const ::fwCom::Signals::SignalKeyType JOB_CREATED_SIGNAL = "jobCreated";
static const ::fwCom::Slots::SlotKeyType FORWARD_JOB_SLOT       = "forwardJob";

//------------------------------------------------------------------------------

SQIOSelector::SQIOSelector() :
    m_mode( READER_MODE ),
    m_servicesAreExcluded( true ),
    m_qInOut(nullptr)
{
    m_sigJobCreated  = newSignal< JobCreatedSignalType >( JOB_CREATED_SIGNAL );
    m_slotForwardJob = newSlot( FORWARD_JOB_SLOT, &SQIOSelector::forwardJob, this );
}

//------------------------------------------------------------------------------

SQIOSelector::~SQIOSelector()  noexcept
{
}

//------------------------------------------------------------------------------

void SQIOSelector::configuring()
{
    SLM_ASSERT("The qml property <mode> must be 'reader' (to open file) or 'writer' (to write a new file).",
               m_qMode == "writer" || m_qMode == "reader" );
    m_mode = ( m_qMode == "writer" ) ? WRITER_MODE : READER_MODE;
    SLM_DEBUG( "mode => " + m_qMode );

    const std::string selectionMode = m_qSelectionMode.toStdString();
    SLM_ASSERT( "The qml attribute <selectionMode> must be 'include' (to add the selection to selector list ) or "
                "'exclude' (to exclude the selection of the selector list).",
                selectionMode == "exclude" || selectionMode == "include" );
    m_servicesAreExcluded = ( selectionMode == "exclude" );
    SLM_DEBUG( "selection mode => " + selectionMode );
}

//------------------------------------------------------------------------------

void SQIOSelector::starting()
{
}

//------------------------------------------------------------------------------

void SQIOSelector::stopping()
{
}

//------------------------------------------------------------------------------

void SQIOSelector::updating()
{
    bool createOutput = false;
    ::fwData::Object::sptr obj;// = this->getInOut< ::fwData::Object>(::fwIO::s_DATA_KEY);

    if (m_qInOut)
    {
        obj = m_qInOut->getObject();
    }
    // Retrieve implementation of type ::fwIO::IReader for this object
    std::vector< std::string > availableExtensionsId;
    if ( m_mode == READER_MODE )
    {
        std::string classname = m_qDataClassname.toStdString();

        // FIXME: support for old version using getObject(): all the 'in' or 'inout' keys were possible
        if (!obj && classname.empty())
        {
            SLM_FATAL("You must provide either an inout object or a dataClassName attribute");
        }
        if (obj)
        {
            SLM_WARN_IF("The 'class' attribute is defined, but the object is set as 'inout', only the object classname "
                        "is used", !classname.isEmpty());
            classname = obj->getClassname();
        }
        createOutput          = (!obj && !m_qDataClassname.isEmpty());
        availableExtensionsId =
            ::fwServices::registry::ServiceFactory::getDefault()->getImplementationIdFromObjectAndType(
                classname, "::fwIO::IReader");
    }
    else
    {
        if (!obj)
        {
            SLM_FATAL("You must provide an object in order to write.");
        }
        availableExtensionsId =
            ::fwServices::registry::ServiceFactory::getDefault()->getImplementationIdFromObjectAndType(
                obj->getClassname(), "::fwIO::IWriter");
    }

    // Filter available extensions and replace id by service description
    std::vector< std::pair < std::string, std::string > > availableExtensionsMap;
    std::vector< std::string > availableExtensionsSelector;

    for( const std::string& serviceId :  availableExtensionsId )
    {
        bool serviceIsSelectedByUser =
            std::find( m_selectedServices.begin(), m_selectedServices.end(),
                       serviceId ) != m_selectedServices.end();

        // Test if the service is considered here as available by users, if yes push in availableExtensionsSelector
        // excluded mode => add services that are not selected by users
        // included mode => add services selected by users
        if( (m_servicesAreExcluded && !serviceIsSelectedByUser) ||
            (!m_servicesAreExcluded && serviceIsSelectedByUser) )
        {
            // Add this service
            std::string infoUser =
                ::fwServices::registry::ServiceFactory::getDefault()->getServiceDescription(serviceId);

            std::map< std::string, std::string >::const_iterator iter = m_serviceToConfig.find( serviceId );
            if ( iter != m_serviceToConfig.end() )
            {
                infoUser = ::fwServices::registry::ServiceConfig::getDefault()->getConfigDesc(iter->second);
            }

            if (infoUser != "")
            {
                availableExtensionsMap.push_back( std::pair < std::string, std::string > (serviceId, infoUser) );
                availableExtensionsSelector.push_back( infoUser );
            }
            else
            {
                availableExtensionsMap.push_back( std::pair < std::string, std::string > (serviceId, serviceId) );
                availableExtensionsSelector.push_back( serviceId );
            }
        }
    }

    // Sort available services (lexical string sort)
    std::sort( availableExtensionsSelector.begin(), availableExtensionsSelector.end() );

    // Test if we have an extension
    if ( !availableExtensionsMap.empty() )
    {
        std::string extensionId           = availableExtensionsMap[0].first;
        bool extensionSelectionIsCanceled = false;

        // Selection of extension when availableExtensions.size() > 1
        if ( availableExtensionsSelector.size() > 1 )
        {
            ::fwGui::dialog::SelectorDialog::sptr selector = ::fwGui::dialog::SelectorDialog::New();

            if ( m_mode != READER_MODE )
            {
                selector->setTitle("Writer to use");
            }
            else
            {
                selector->setTitle("Reader to use");
            }
            selector->setSelections(availableExtensionsSelector);
            std::string selection = selector->show();
            if( !selection.empty() )
            {
                bool extensionIdFound = false;

                typedef std::pair < std::string, std::string > PairType;
                for(PairType pair :  availableExtensionsMap)
                {
                    if (pair.second == selection )
                    {
                        extensionId      = pair.first;
                        extensionIdFound = true;
                    }
                }
                OSLM_ASSERT("Problem to find the selected string.", extensionIdFound );
            }
            else
            {
                extensionSelectionIsCanceled = true;
            }
        }

        if ( !extensionSelectionIsCanceled )
        {

            // Get Config
            bool hasConfigForService = false;
            ::fwRuntime::ConfigurationElement::csptr srvCfg;
            if ( m_serviceToConfig.find( extensionId ) != m_serviceToConfig.end() )
            {
                hasConfigForService = true;
                srvCfg              = ::fwServices::registry::ServiceConfig::getDefault()->getServiceConfig(
                    m_serviceToConfig[extensionId], extensionId );
                SLM_ASSERT(
                    "No service configuration of type ::fwServices::registry::ServiceConfig was found",
                    srvCfg );
            }

            // Configure and start service
            if ( m_mode == READER_MODE )
            {
                if(createOutput)
                {
                    obj = ::fwData::factory::New(m_qDataClassname.toStdString());
                    SLM_ASSERT("Cannot create object with classname='" + m_qDataClassname.toStdString() + "'", obj);
                }
                ::fwIO::IReader::sptr reader = ::fwServices::add< ::fwIO::IReader >( extensionId );
                reader->registerInOut(obj, ::fwIO::s_DATA_KEY);
                reader->setWorker(m_associatedWorker);

                if ( hasConfigForService )
                {
                    reader->setConfiguration( ::fwRuntime::ConfigurationElement::constCast(srvCfg) );
                    reader->configure();
                }

                auto jobCreatedSignal = reader->signal("jobCreated");
                if(jobCreatedSignal)
                {
                    jobCreatedSignal->connect(m_slotForwardJob);
                }

                try
                {
                    reader->setWorker(::fwThread::Worker::sptr());
                    reader->start();
                    reader->configureWithIHM();

                    ::fwGui::Cursor cursor;
                    cursor.setCursor(::fwGui::ICursor::BUSY);
                    reader->update();
                    cursor.setDefaultCursor();

                    reader->stop();
                    ::fwServices::OSR::unregisterService(reader);

                    if (createOutput)
                    {
                        m_qInOut = new ::fwServices::QtObjectHolder(obj);
                    }
                    done();
                }
                catch (std::exception& e)
                {
                    std::string msg = "Failed to read : \n" + std::string(e.what());
                    ::fwGui::dialog::MessageDialog::showMessageDialog("Reader Error", msg);
                    failure(msg.c_str());
                }
            }
            else
            {
                // When all writers make use of getObject(), we can use the following code instead:
                //      ::fwIO::IWriter::sptr writer = ::fwServices::add< ::fwIO::IWriter >( extensionId );
                //      writer->registerInput(this->getObject(), ::fwIO::s_DATA_KEY);

                auto factory = ::fwServices::registry::ServiceFactory::getDefault();
                ::fwIO::IWriter::sptr writer =
                    ::fwIO::IWriter::dynamicCast(factory->create( "::fwIO::IWriter", extensionId));
                ::fwServices::OSR::registerService(obj, ::fwIO::s_DATA_KEY,
                                                   ::fwServices::IService::AccessType::INPUT, writer);

                writer->setWorker(m_associatedWorker);

                if ( hasConfigForService )
                {
                    writer->setConfiguration( ::fwRuntime::ConfigurationElement::constCast(srvCfg) );
                    writer->configure();
                }

                auto jobCreatedSignal = writer->signal("jobCreated");
                if(jobCreatedSignal)
                {
                    jobCreatedSignal->connect(m_slotForwardJob);
                }

                try
                {
                    writer->start();
                    writer->configureWithIHM();

                    ::fwGui::Cursor cursor;
                    cursor.setCursor(::fwGui::ICursor::BUSY);
                    writer->update();
                    cursor.setDefaultCursor();

                    writer->stop();
                    ::fwServices::OSR::unregisterService(writer);
                }
                catch (std::exception& e)
                {
                    std::string msg = "Failed to write : \n" +  std::string(e.what());
                    ::fwGui::dialog::MessageDialog::showMessageDialog("Writer Error", msg);
                }
            }
        }
    }
    else
    {
        SLM_WARN("SQIOSelector::load : availableExtensions is empty.");
        if ( m_mode == READER_MODE )
        {
            ::fwGui::dialog::MessageDialog messageBox;
            messageBox.setTitle("Reader not found");
            messageBox.setMessage( "There are no available readers for this data type." );
            messageBox.setIcon(::fwGui::dialog::IMessageDialog::WARNING);
            messageBox.addButton(::fwGui::dialog::IMessageDialog::OK);
            messageBox.show();
        }
        else // m_mode == WRITER_MODE
        {
            ::fwGui::dialog::MessageDialog messageBox;
            messageBox.setTitle("Writer not found");
            messageBox.setMessage( "There are no available writers for this data type." );
            messageBox.setIcon(::fwGui::dialog::IMessageDialog::WARNING);
            messageBox.addButton(::fwGui::dialog::IMessageDialog::OK);
            messageBox.show();
        }
    }
}

//------------------------------------------------------------------------------

void SQIOSelector::info( std::ostream& _sstream )
{
    // Update message
    _sstream << "SQIOSelector";
}

//------------------------------------------------------------------------------

void SQIOSelector::setIOMode( IOMode _mode )
{
    m_mode = _mode;
}

//------------------------------------------------------------------------------

void SQIOSelector::forwardJob(::fwJobs::IJob::sptr iJob)
{
    jobCreated();
}

//------------------------------------------------------------------------------
} // namespace editor

} // namespace gui
