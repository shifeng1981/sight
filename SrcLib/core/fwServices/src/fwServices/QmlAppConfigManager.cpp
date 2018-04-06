#include "fwServices/QmlAppConfigManager.hpp"

fwServices::QmlAppConfigManager::QmlAppConfigManager() :
	m_isUnitTest(false)
{
}

// -----------------------------------------------------------

fwServices::QmlAppConfigManager::~QmlAppConfigManager()
{
}

// -----------------------------------------------------------

void	fwServices::QmlAppConfigManager::setConfig(const std::string& configId,
	                                          const FieldAdaptorType& replaceFields)
{
	m_configId = configId;
}

// -----------------------------------------------------------

void	fwServices::QmlAppConfigManager::setConfig(const std::string& configId,
	                                          const ::fwData::Composite::csptr& replaceFields)
{
	m_configId = configId;
}

// -----------------------------------------------------------

::fwData::Object::sptr	fwServices::QmlAppConfigManager::getConfigRoot() const
{

}

// -----------------------------------------------------------

void	fwServices::QmlAppConfigManager::launch()
{

}

// -----------------------------------------------------------

void	fwServices::QmlAppConfigManager::stopAndDestroy()
{
	this->stop();
	this->destroy();
}

// -----------------------------------------------------------

void	fwServices::QmlAppConfigManager::create()
{

}

// -----------------------------------------------------------

void	fwServices::QmlAppConfigManager::start()
{

}

// -----------------------------------------------------------

void	fwServices::QmlAppConfigManager::update()
{

}

// -----------------------------------------------------------

void	fwServices::QmlAppConfigManager::stop()
{

}

// -----------------------------------------------------------

void	fwServices::QmlAppConfigManager::destroy()
{

}

// -----------------------------------------------------------

void	fwServices::QmlAppConfigManager::setIsUnitTest(bool isUnitTest)
{
	m_isUnitTest = isUnitTest;
}

// -----------------------------------------------------------

void	fwServices::QmlAppConfigManager::startBundle()
{
	
}

// -----------------------------------------------------------
