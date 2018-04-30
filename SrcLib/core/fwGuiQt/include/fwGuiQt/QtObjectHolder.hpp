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
    /**
     * @brief QtObjectHolder: constructor
     * @param object: object to hold
     * @param parent: default Qt parent
     */
    FWGUIQT_API QtObjectHolder(std::shared_ptr<::fwData::Object> const& object, QObject *parent = 0);

    /**
     * @brief QtObjectHolder: constructor copy
     * @param other: class to copy
     */
    FWGUIQT_API QtObjectHolder(QtObjectHolder const& other);

    /**
     * @brief QtObjectHolder: default constructor
     * @param parent: default Qt parent
     *
     * This constructor allow us to instanciate "empty" object
     */
    FWGUIQT_API QtObjectHolder(QObject *parent = 0);

    /**
     * @brief: return stored data in "m_object"
     *
     * If the data is not set and used, the behavior is undefined.
     */
    std::shared_ptr<::fwData::Object>& FWGUIQT_API getObject();

    /**
     *  @brief: same as above but with constness
     */
    std::shared_ptr<::fwData::Object> const& FWGUIQT_API  getObject() const;

    /**
     * @brief ~QtObjectHolder: Destructor, do nothing, use RAII to clean objects
     */
    FWGUIQT_API ~QtObjectHolder();

    /**
     * @brief: Compare the data stored inside this object with another.
     */
    bool FWGUIQT_API operator!=(QtObjectHolder const& other);

    /**
     * @brief: = operator, used to pass value in QML
     */
    QtObjectHolder& FWGUIQT_API operator=(QtObjectHolder const& other);

private:
    std::shared_ptr<::fwData::Object> m_object;

};

} // fwGuiQt

#endif // QTOBJECTHOLDER_H
