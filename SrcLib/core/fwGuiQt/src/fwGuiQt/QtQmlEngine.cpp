#include "fwGuiQt/QtQmlEngine.hpp"

#include <fwServices/QtQmlType.hxx>
#include <fwServices/QtQmlInstancier.hxx>
#include <fwServices/IQmlService.hpp>

#include <QCoreApplication>
#include <QQuickWindow>
#include <QFileInfo>
#include <QQmlContext>
#include <QQuickItem>

namespace fwGuiQt
{

class   QmlAppEventFilter: public QObject
{
public:
    QmlAppEventFilter() = default;
    ~QmlAppEventFilter() = default;

protected:
    bool    eventFilter(QObject *obj, QEvent *event)
    {
        if (event->type() == QEvent::DeferredDelete)
        {
            QtQmlEngine::getEngine().stopServices();
        }
        return QObject::eventFilter(obj, event);
    }

};

QtQmlEngine	*QtQmlEngine::m_qtQmlEngine = nullptr;

QtQmlEngine::QtQmlEngine()
{
	m_qtQmlEngine = this;
}

QtQmlEngine&	QtQmlEngine::getEngine()
{
	return *m_qtQmlEngine;
}

void	QtQmlEngine::loadFile(std::string const& scriptFile)
{
    m_scriptFile = scriptFile;
    m_rootWindow = new QQuickWidget;

    QObject::connect(this, SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));
    m_rootWindow->setAttribute( Qt::WA_DeleteOnClose );
    m_rootWindow->installEventFilter(new QmlAppEventFilter);
    m_rootWindow->setSource(QFileInfo(QString::fromStdString(m_scriptFile)).filePath());

}

void	QtQmlEngine::launch()
{
    // Get window dimension (suggested by QML file)
    m_rootWindow->setResizeMode(QQuickWidget::SizeRootObjectToView);

    //Display window
    m_rootWindow->show();
}

void	QtQmlEngine::addCtx(std::string const& uid, std::string const& type)
{
    QObject *ctxElem = ::fwServices::QtQmlInstancier::instanciate(type);

	SLM_ASSERT("Class not found : " + type, ctxElem != nullptr);
	rootContext()->setContextProperty(QString::fromStdString(uid), ctxElem);
}

QQuickWidget	*QtQmlEngine::getWindow() const
{
	return m_rootWindow;
}

void    QtQmlEngine::stopServices()
{
    QMetaObject::invokeMethod(m_rootWindow->rootObject(), "cleanUp");
}

QtQmlEngine::~QtQmlEngine()
{
}

} // fwGuiQt

