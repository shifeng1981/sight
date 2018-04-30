#ifndef QTOBJECTHOLDER_H
#define QTOBJECTHOLDER_H

#include "fwGuiQt/config.hpp"
#include "fwGuiQt/QtQmlType.hxx"

#include <fwData/Object.hpp>

#include <QObject>
#include <QSharedPointer>

#include <memory>

namespace fwGuiQt
{

class FWGUIQT_CLASS_API   QtObjectHolder: public QObject
{
    Q_OBJECT
public:
    using sptr = QSharedPointer<QtObjectHolder>;
    using csptr = const QSharedPointer<QtObjectHolder>;

public:

    FWGUIQT_API QtObjectHolder(std::shared_ptr<::fwData::Object> const& object, QObject *parent = 0);

    FWGUIQT_API QtObjectHolder(QtObjectHolder const& other);

    FWGUIQT_API QtObjectHolder(QObject *parent = 0);

    std::shared_ptr<::fwData::Object>& FWGUIQT_API getObject();

    std::shared_ptr<::fwData::Object> const& FWGUIQT_API  getObject() const;

    FWGUIQT_API ~QtObjectHolder();

    bool FWGUIQT_API operator!=(QtObjectHolder const& other);

    QtObjectHolder& FWGUIQT_API operator=(QtObjectHolder const& other);

private:
    std::shared_ptr<::fwData::Object> m_object;

};

} // fwGuiQt

#endif // QTOBJECTHOLDER_H
