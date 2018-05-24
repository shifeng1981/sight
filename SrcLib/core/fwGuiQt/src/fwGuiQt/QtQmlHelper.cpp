#include "fwGuiQt/QtQmlHelper.hpp"
#include "fwGuiQt/QtQmlEngine.hpp"

#include <QQuickItem>

namespace fwGuiQt
{

QtQmlHelper::QtQmlHelper()
{
}

QObject	*QtQmlHelper::getObjectByName(std::string const& id)
{
    return getRootObject()->findChild<QObject *>(id.c_str());
}

QQuickWindow	*QtQmlHelper::getRootObject()
{
    return QtQmlEngine::getEngine().getWindow();
}

QtQmlHelper::~QtQmlHelper()
{
}

} // fwGuiQt
