#ifndef QTOBJECTHOLDER_H
#define QTOBJECTHOLDER_H

#include "fwServices/config.hpp"
#include "fwServices/QtQmlType.hxx"

#include <fwData/Object.hpp>

#include <QObject>
#include <QSharedPointer>

#include <memory>

namespace fwServices
{

class FWSERVICES_CLASS_QT_API   QtObjectHolder: public QObject
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
    FWSERVICES_QT_API QtObjectHolder(std::shared_ptr<::fwData::Object> const& object, QObject *parent = 0);

    /**
     * @brief QtObjectHolder: constructor copy
     * @param other: class to copy
     */
    FWSERVICES_QT_API QtObjectHolder(QtObjectHolder const& other);

    /**
     * @brief QtObjectHolder: default constructor
     * @param parent: default Qt parent
     *
     * This constructor allow us to instanciate "empty" object
     */
    FWSERVICES_QT_API QtObjectHolder(QObject *parent = 0);

    /**
     * @brief: return stored data in "m_object"
     *
     * If the data is not set and used, the behavior is undefined.
     */
    FWSERVICES_QT_API std::shared_ptr<::fwData::Object>& getObject();

    /**
     *  @brief: same as above but with constness
     */
    FWSERVICES_QT_API std::shared_ptr<::fwData::Object> const& getObject() const;

    /**
     * @brief ~QtObjectHolder: Destructor, do nothing, use RAII to clean objects
     */
    FWSERVICES_QT_API ~QtObjectHolder();

    /**
     * @brief: Compare the data stored inside this object with another.
     */
    FWSERVICES_QT_API bool  operator!=(QtObjectHolder const& other);

    /**
     * @brief: = operator, used to pass value in QML
     */
    FWSERVICES_QT_API QtObjectHolder& operator=(QtObjectHolder const& other);

private:
    std::shared_ptr<::fwData::Object> m_object;

};

} // fwGuiQt

#endif // QTOBJECTHOLDER_H
