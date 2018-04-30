#pragma once

#include "fwGuiQt/config.hpp"

#include <fwServices/IQmlEngine.hpp>

#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QObject>
#include <QQuickWidget>

#include <memory>
#include <vector>

namespace fwGuiQt
{

	/**
	 *	@brief: The purpose is to load `scriptFile`.qml and display it
	 *
	 */
class FWGUIQT_CLASS_API QtQmlEngine : public ::fwServices::IQmlEngine, public QQmlApplicationEngine
{
private:
	static QtQmlEngine	*m_qtQmlEngine;

public:
	FWGUIQT_API QtQmlEngine();
	~QtQmlEngine();

	static QtQmlEngine&	getEngine();
	
	/**
	 *	@brief: This function load file specified by scriptFile
	 *		This call can throw if the file contain a error or if it can't be found
	 */
	void	loadFile(std::string const& scriptFile);
	/**
	 *	@brief: Run script file
	 */
	void	launch();

	/**
	 *	@brief: return root window
	 */
    QQuickWidget	*getWindow() const;

	/**
	 * @brief: This function will instanciate a class derived of QObject
	 * @uid: Name of the context (to be used in QML)
	 * @type: Class name
	 */
	virtual void FWSERVICES_API 	addCtx(std::string const& uid, std::string const& type);

private:
	std::string	m_scriptFile;
	std::unique_ptr<QQmlComponent>	m_component;
	std::vector<std::unique_ptr<QObject> >	m_context;
    QQuickWidget	*m_rootWindow;
};

} // fwGuiQt
