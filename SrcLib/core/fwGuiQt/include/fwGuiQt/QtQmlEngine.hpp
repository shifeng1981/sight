#pragma once

#include "fwGuiQt/config.hpp"

#include <fwServices/IQmlEngine.hpp>

#include <QQmlEngine>
#include <QQmlComponent>

#include <memory>

namespace fwGuiQt
{

	/**
	 *	@brief: The purpose is to load `scriptFile`.qml and display it
	 *
	 */
class FWGUIQT_CLASS_API QtQmlEngine : public ::fwServices::IQmlEngine, public QQmlEngine
{

public:
	FWGUIQT_API QtQmlEngine();
	~QtQmlEngine();
	
	/**
	 *	@brief: This function load file specified by scriptFile
	 *		This call can throw if the file contain a error or if it can't be found
	 */
	void	loadFile(std::string const& scriptFile);
	/**
	 *	@brief: Run script file
	 */
	void	launch();

private:
	std::string	m_scriptFile;
	std::unique_ptr<QQmlComponent>	m_component;
};

} // fwGuiQt
