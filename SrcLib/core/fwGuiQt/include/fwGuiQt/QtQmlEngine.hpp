#pragma once

#include "fwGuiQt/config.hpp"

#include <QQmlEngine>
#include <QQmlComponent>

#include <memory>

namespace fwGuiQt
{

	/**
	 *	@brief: The purpose is to load `scriptFile`.qml and display it
	 *
	 */
class FWGUIQT_CLASS_API QtQmlEngine : public QQmlEngine
{
	Q_OBJECT

public:
	FWGUIQT_API QtQmlEngine(std::string const& scriptFile);
	~QtQmlEngine();
	
	/**
	 *	@brief: This function load file specified by scriptFile
	 *		This call can throw if the file contain a error or if it can't be found
	 */
	void	loadFile();
	/**
	 *	@brief: Run script file
	 */
	void	launch();

private:
	std::string	m_scriptFile;
	std::unique_ptr<QQmlComponent>	m_component;
};

} // fwGuiQt
