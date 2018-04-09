#include "fwGuiQt/QtQmlEngine.hpp"

#include <QCoreApplication>
#include <QQuickWindow>

fwGuiQt::QtQmlEngine::QtQmlEngine(std::string const& scriptFile)
{
	m_scriptFile = scriptFile;
}

void	fwGuiQt::QtQmlEngine::loadFile()
{
	m_component = std::unique_ptr<QQmlComponent>(new QQmlComponent(this));

	QObject::connect(this, SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));

	m_component->loadUrl(QUrl::fromLocalFile(QString::fromStdString(m_scriptFile)));

	SLM_ASSERT(qPrintable(m_component->errorString()), m_component->isReady());
}

void	fwGuiQt::QtQmlEngine::launch()
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

fwGuiQt::QtQmlEngine::~QtQmlEngine()
{
}