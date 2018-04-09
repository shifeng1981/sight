/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2015-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "fwServices/IAppConfigManager.hpp"
#include "fwServices/IQmlEngine.hpp"

namespace fwServices
{
    class FWSERVICES_CLASS_API QmlAppConfigManager : public IAppConfigManager
    {
    public:
    	fwCoreClassDefinitionsWithFactoryMacro((QmlAppConfigManager)(::fwServices::IAppConfigManager),
        	                                   (()),
                                           	std::make_shared< QmlAppConfigManager >)
	    fwCoreAllowSharedFromThis()


        QmlAppConfigManager();
        ~QmlAppConfigManager();
        

	   /**
     	* @name Overrides
     	* @{
     	*/
	    FWSERVICES_API void setConfig(const std::string& configId,
	                                          const FieldAdaptorType& replaceFields = FieldAdaptorType());
	    FWSERVICES_API void setConfig(const std::string& configId,
	                                          const ::fwData::Composite::csptr& replaceFields);
	    FWSERVICES_API ::fwData::Object::sptr getConfigRoot() const;
	    FWSERVICES_API void launch();
	    FWSERVICES_API void stopAndDestroy();
	    FWSERVICES_API void create();
	    FWSERVICES_API void start();
	    FWSERVICES_API void update();
	    FWSERVICES_API void stop();
	    FWSERVICES_API void destroy();
	    ///@}

	    /// Setter for `m_isUnitTest`
  	    FWSERVICES_API void	setIsUnitTest(bool isUnitTest);
  	    FWSERVICES_API void	setEngine(SPTR(::fwServices::IQmlEngine) const&);

  	private:
  	   /**
     	* @brief Starts the bundle associated to the config
     	* @note  Does nothing if the bundle is already started or if the config id is not specified (ie. if config is set
     	*        with setConfig(::fwRuntime::ConfigurationElement::csptr cfgElem) ).
     	*/
  		FWSERVICES_API void	startBundle();

  	   /**
  	    * @brief Load QML file
  	    * @note Assert if the file in not founded
  	    */
  	    FWSERVICES_API void	loadQMLFile(::fwRuntime::ConfigurationElement::csptr const&);

  	   /**
  	    * @brief Create QML context if needed
  	    *	i.e create user-needed class and link with QML
  	    */
  	    FWSERVICES_API void createContext(::fwRuntime::ConfigurationElement::csptr const&);

	private:
		/// Is in unit testing mode
		bool	m_isUnitTest;
		std::string	m_configId;
		SPTR(::fwServices::IQmlEngine)	m_qmlEngine;

		/**
		 * Static member for key xml parsing
		 * @{
		 */
		static std::string	QmlEntryPoint;
		static std::string	File;
		static std::string	Context;
		static std::string	Class;
		/**
		 * }@
		 */
	};
}