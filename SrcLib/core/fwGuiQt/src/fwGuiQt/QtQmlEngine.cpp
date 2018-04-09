#include "fwGuiQt/QtQmlEngine.hpp"
#include "fwGuiQt/QtQmlInstancier.hpp"

#include <QCoreApplication>
#include <QQuickWindow>
#include <QFileInfo>
#include <QQmlContext>

namespace fwGuiQt
{

QtQmlEngine::QtQmlEngine()
{
}

void	QtQmlEngine::loadFile(std::string const& scriptFile)
{
	m_scriptFile = scriptFile;
	m_component = std::unique_ptr<QQmlComponent>(new QQmlComponent(this));

	QObject::connect(this, SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));

	m_component->loadUrl(QFileInfo(QString::fromStdString(m_scriptFile)).filePath());

	SLM_ASSERT(qPrintable(m_component->errorString()), m_component->isReady());
}

void	QtQmlEngine::launch()
{
	// Create QQuickWindow
	QObject	*topLevel = m_component->create();
	QQuickWindow	*window = qobject_cast<QQuickWindow *>(topLevel);

	// Get window dimension (suggested by QML file)
	QSurfaceFormat surfaceFormat = window->requestedFormat();
	window->setFormat(surfaceFormat);

	//Display window
	window->show();
}

void	QtQmlEngine::addCtx(std::string const& uid, std::string const& type)
{
	std::cout << "Ctx : " << type << std::endl;
	QObject *ctxElem = QtQmlInstancier::instanciate(type);

	SLM_ASSERT("Class not found : " + type, ctxElem != nullptr);
	std::cout << uid << ":" << ctxElem << std::endl;
	rootContext()->setContextProperty(QString::fromStdString(uid), ctxElem);
}

QtQmlEngine::~QtQmlEngine()
{
}

} // fwGuiQt

