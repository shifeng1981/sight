/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#ifndef QTOBJECTHOLDER_H
#define QTOBJECTHOLDER_H

#include "fwServices/config.hpp"
#include "fwServices/macros.hpp"
#include "fwServices/QtQmlType.hxx"

#include <fwData/Object.hpp>

#include <QObject>
#include <QString>

#include <memory>

namespace fwServices
{

class FWSERVICES_CLASS_QT_API QtObjectHolder : public QObject
{
Q_OBJECT

Q_PROPERTY(QString objectType MEMBER m_objectType NOTIFY objectTypeChanged)

public:
    fwQmlTypeMacro(QtObjectHolder);
    /**
     * @brief QtObjectHolder: constructor
     * @param object: object to hold
     * @param parent: default Qt parent
     */
    FWSERVICES_QT_API QtObjectHolder(std::shared_ptr<::fwData::Object> const& object, QObject* parent = 0);

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
    FWSERVICES_QT_API QtObjectHolder(QObject* parent = 0);

    /**
     * @brief: return stored data in "m_object"
     *
     * If the data is not set and used, the behavior is undefined.
     */
    FWSERVICES_QT_API ::fwData::Object::sptr& getObject();

    /**
     *  @brief: same as above but with constness
     */
    FWSERVICES_QT_API const ::fwData::Object::csptr  getObject() const;

    /**
     * @brief ~QtObjectHolder: Destructor, do nothing, use RAII to clean objects
     */
    FWSERVICES_QT_API ~QtObjectHolder();

    /**
     * @brief: Compare the data stored inside this object with another.
     */
    FWSERVICES_QT_API bool operator!=(QtObjectHolder const& other);

    /**
     * @brief: = operator, used to pass value in QML
     */
    FWSERVICES_QT_API QtObjectHolder& operator=(QtObjectHolder const& other);

private Q_SLOTS:
    /**
     *  @brief: create slot, used when type is specified in QML file
     */
    FWSERVICES_QT_API void    create();

Q_SIGNALS:
    void    objectTypeChanged(QString const& newType);

private:
    ::fwData::Object::sptr m_object;

    QString m_objectType;

};

} // fwGuiQt

#endif // QTOBJECTHOLDER_H
