/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWGUIQUICK_SLICESELECTOR_HPP__
#define __FWGUIQUICK_SLICESELECTOR_HPP__

#include "fwGuiQuick/config.hpp"

#include <boost/function.hpp>

#include <QObject>
#include <QPointer>
#include <QWidget>

// Qt
class QSlider;
class QLineEdit;
class QComboBox;
class QStringList;

namespace fwGuiQuick
{

/**
 * @brief A qt panel used to control a VTK 2D Negatoscope view.
 *
 */
class SliceSelector : public QWidget
{

Q_OBJECT

public:
    /**
     * @brief Constructor.
     * @param parent The parent widget.
     */
    FWGUIQUICK_API SliceSelector( QWidget* const parent) throw();

    /// @brief Destructor.
    FWGUIQUICK_API virtual ~SliceSelector() throw();

    FWGUIQUICK_API void setSliceRange( int min, int max );

    FWGUIQUICK_API void setSliceValue( int index );

    FWGUIQUICK_API void setTypeSelection( int type );

    FWGUIQUICK_API void setEnable(bool enable);

    typedef ::boost::function1<void, int > ChangeIndexCallback;
    FWGUIQUICK_API void setChangeIndexCallback(ChangeIndexCallback fct);

    typedef ::boost::function1<void, int > ChangeTypeCallback;
    FWGUIQUICK_API void setChangeTypeCallback(ChangeTypeCallback fct);

protected Q_SLOTS:

    /**
     * @brief Event handler for a slice type change.
     * @param index index of the selected type item.
     */
    FWGUIQUICK_API void onSliceTypeChange( int index );

    /**
     * @brief Event handler for a slice index change.
     * @param value current value of the slice index slider.
     */
    FWGUIQUICK_API void onSliceIndexChange( int value ) throw();

private:

    QPointer< QComboBox > m_sliceType;

    /// @brief The slice index slider widget.
    QPointer< QSlider > m_sliceIndex;
    QPointer< QLineEdit > m_pSliceIndexText;

    void printIndex(int index);
    void printType(int type);

    ChangeIndexCallback m_fctChangeIndexCallback;
    ChangeTypeCallback m_fctChangeTypeCallback;
};

} // fwGuiQuick

#endif // __FWGUIQUICK_SLICESELECTOR_HPP__
