#include "fwServices/QmlAppConfigManager.hpp"

#include "fwServices/registry/AppConfig.hpp"

#define FW_PROFILING_DISABLED
#include <fwCore/Profiling.hpp>

#include <fwData/Composite.hpp>
#include <fwData/Object.hpp>

#include <fwRuntime/Bundle.hpp>
#include <fwRuntime/operations.hpp>

std::string	fwServices::QmlAppConfigManager::QmlEntryPoint = "qml";
std::string	fwServices::QmlAppConfigManager::File = "file";
std::string	fwServices::QmlAppConfigManager::Context = "ctx";
std::string	fwServices::QmlAppConfigManager::Class = "class";

fwServices::QmlAppConfigManager::QmlAppConfigManager() :
	m_isUnitTest(false)
{
	std::cout << "/!\\ Using QmlAppConfigManager" << std::endl;
}

// -----------------------------------------------------------

fwServices::QmlAppConfigManager::~QmlAppConfigManager()
{
	    SLM_ASSERT("Manager must be stopped before destruction.", m_state == STATE_DESTROYED);
}

// -----------------------------------------------------------

void	fwServices::QmlAppConfigManager::setConfig(const std::string& configId,
	                                          const FieldAdaptorType& replaceFields)
{
	m_configId = configId;
	m_cfgElem  = registry::AppConfig::getDefault()->getAdaptedTemplateConfig( configId, replaceFields, !m_isUnitTest );
}

// -----------------------------------------------------------

void	fwServices::QmlAppConfigManager::setConfig(const std::string& configId,
	                                          const ::fwData::Composite::csptr& replaceFields)
{
	m_configId = configId;
	m_cfgElem  = registry::AppConfig::getDefault()->getAdaptedTemplateConfig( configId, replaceFields, !m_isUnitTest );
}

// -----------------------------------------------------------

::fwData::Object::sptr	fwServices::QmlAppConfigManager::getConfigRoot() const
{
}

// -----------------------------------------------------------

void	fwServices::QmlAppConfigManager::launch()
{
	FW_PROFILE("launch");

	this->startBundle();
	this->create();
	this->start();
	this->update();
}

// -----------------------------------------------------------

void	fwServices::QmlAppConfigManager::stopAndDestroy()
{
	this->stop();
	this->destroy();
}

void	fwServices::QmlAppConfigManager::setEngine(SPTR(::fwServices::IQmlEngine) const& engine)
{
	m_qmlEngine = engine;
}

// -----------------------------------------------------------

void	fwServices::QmlAppConfigManager::create()
{
	SLM_ASSERT("Missing QML engine", m_qmlEngine);
	SLM_ASSERT("Manager already running.", m_state == STATE_DESTROYED);
	bool	qmlFound = false;

	for (const auto& elem : m_cfgElem->getElements())
	{
		if (elem->getName() == ::fwServices::QmlAppConfigManager::QmlEntryPoint)
		{
			this->loadQMLFile(elem);
			this->createContext(elem);
			qmlFound = true;
		}
	}
	SLM_ASSERT("Can't find <qml> tag in xml", qmlFound);
	m_state = STATE_CREATED;
}

// -----------------------------------------------------------

void	fwServices::QmlAppConfigManager::start()
{
	SLM_ASSERT("Manager not created", m_state == STATE_CREATED);
	m_qmlEngine->launch();
	m_state = STATE_STARTED;
}

// -----------------------------------------------------------

void	fwServices::QmlAppConfigManager::update()
{

}

// -----------------------------------------------------------

void	fwServices::QmlAppConfigManager::stop()
{
    SLM_ASSERT("Manager is not started, cannot stop.", m_state == STATE_STARTED);
    m_state = STATE_STOPPED;
}

// -----------------------------------------------------------

void	fwServices::QmlAppConfigManager::destroy()
{
    m_state = STATE_DESTROYED;
}

// -----------------------------------------------------------

void	fwServices::QmlAppConfigManager::setIsUnitTest(bool isUnitTest)
{
	m_isUnitTest = isUnitTest;
}

// -----------------------------------------------------------

void	fwServices::QmlAppConfigManager::startBundle()
{
	SLM_ERROR_IF("Bundle is not specified, it can not be started.", m_configId.empty());
	if (!m_configId.empty() && !m_isUnitTest)
    {
        std::shared_ptr< ::fwRuntime::Bundle > bundle = registry::AppConfig::getDefault()->getBundle(m_configId);
        SLM_INFO_IF("Bundle '" + bundle->getIdentifier() + "' (used for '" + m_configId + "') is already started !",
                    bundle->isStarted());
        if (!bundle->isStarted())
        {
            bundle->start();
        }
    }
}

// -----------------------------------------------------------

void	fwServices::QmlAppConfigManager::loadQMLFile(::fwRuntime::ConfigurationElement::csptr const& qmlElement)
{
	for (const auto& elem : qmlElement->getElements())
	{
		if (elem->getName() == ::fwServices::QmlAppConfigManager::File)
		{
			std::cout << "file found <" << elem->getValue() << ">" << std::endl;
			m_qmlEngine->loadFile(elem->getValue());
			return ;
		}
	}
	SLM_ERROR("Missing QML resource file.");
}

// -----------------------------------------------------------

void	fwServices::QmlAppConfigManager::createContext(::fwRuntime::ConfigurationElement::csptr const& qmlElement)
{
	for (const auto& elem : qmlElement->getElements())
	{
		if (elem->getName() == ::fwServices::QmlAppConfigManager::Context)
		{
			for (const auto& classElem : elem->getElements())
			{
	            SLM_ASSERT("Missing attribute \"uid\".", classElem->hasAttribute("uid"));
   	            SLM_ASSERT("Missing attribute \"type\".", classElem->hasAttribute("type"));
   	            std::string	uid = classElem->getAttributeValue("uid");
   	            std::string	cType = classElem->getAttributeValue("type");

   	            std::cout << uid << " " << cType << std::endl;
			}
		}
	}
}