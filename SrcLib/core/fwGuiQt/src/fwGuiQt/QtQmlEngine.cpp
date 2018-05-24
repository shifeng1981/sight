#include "fwGuiQt/QtQmlEngine.hpp"
#include "fwGuiQt/QtQmlHelper.hpp"

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
        if (event->type() == QEvent::HideToParent)
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
    m_component = std::unique_ptr<QQmlComponent>(new QQmlComponent(this));

    QObject::connect(this, SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));

    m_component->loadUrl(QUrl::fromLocalFile(QString::fromStdString(m_scriptFile)));

    SLM_ASSERT(qPrintable(m_component->errorString()), m_component->isReady());
}

void	QtQmlEngine::launch()
{
    QObject  *topLevel = m_component->create();
    m_rootWindow = qobject_cast<QQuickWindow *>(topLevel);


    m_rootWindow->installEventFilter(new QmlAppEventFilter);

    // Get window dimension (suggested by QML file)
    QSurfaceFormat surfaceFormat = m_rootWindow->requestedFormat();
    m_rootWindow->setFormat(surfaceFormat);

    //Display window
    m_rootWindow->show();
    runServices();

}

void	QtQmlEngine::addCtx(std::string const& uid, std::string const& type)
{
    QObject *ctxElem = ::fwServices::QtQmlInstancier::instanciate(type);

	SLM_ASSERT("Class not found : " + type, ctxElem != nullptr);
	rootContext()->setContextProperty(QString::fromStdString(uid), ctxElem);
}

QQuickWindow	*QtQmlEngine::getWindow() const
{
	return m_rootWindow;
}

void    QtQmlEngine::stopServices()
{
    auto srvList = QtQmlHelper::getRootObject()->findChildren<::fwServices::IQmlService *>();

    std::reverse(srvList.begin(), srvList.end());
    for (auto& srv : srvList)
    {
        if (srv->isStarted())
        {
            srv->stop();
        }
    }
}

void    QtQmlEngine::runServices()
{
    auto srvList = QtQmlHelper::getRootObject()->findChildren<::fwServices::IQmlService *>();

    std::cout << "SrvList = " << srvList.size() << std::endl;
    for (auto& srv : srvList)
    {
        if (srv->isAutoStart())
        {
            srv->configure();
            srv->start();
        }
    }
}

void    QtQmlEngine::stop(int status)
{
    this->exit(status);
}

QtQmlEngine::~QtQmlEngine()
{
}

} // fwGuiQt

