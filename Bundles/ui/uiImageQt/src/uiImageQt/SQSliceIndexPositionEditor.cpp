/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "uiImageQt/SQSliceIndexPositionEditor.hpp"

#include <fwCom/Signal.hpp>
#include <fwCom/Signal.hxx>
#include <fwCom/Signals.hpp>
#include <fwCom/Slot.hpp>
#include <fwCom/Slot.hxx>
#include <fwCom/Slots.hpp>
#include <fwCom/Slots.hxx>

#include <fwDataTools/fieldHelper/Image.hpp>
#include <fwDataTools/fieldHelper/MedicalImageHelpers.hpp>

#include <fwServices/QtQmlType.hxx>

namespace uiImageQt
{

const std::string* SQSliceIndexPositionEditor::SLICE_INDEX_FIELDID[ 3 ] =
{
    &fwDataTools::fieldHelper::Image::m_sagittalSliceIndexId,
    &fwDataTools::fieldHelper::Image::m_frontalSliceIndexId,
    &fwDataTools::fieldHelper::Image::m_axialSliceIndexId
};

static const ::fwCom::Slots::SlotKeyType s_UPDATE_SLICE_INDEX_SLOT = "updateSliceIndex";
static const ::fwCom::Slots::SlotKeyType s_UPDATE_SLICE_TYPE_SLOT  = "updateSliceType";

static ::fwServices::QtQmlType<SQSliceIndexPositionEditor>  registar("com.fw4spl", 1, 0, "SSliceIndexPositionEditor");

SQSliceIndexPositionEditor::SQSliceIndexPositionEditor()
{
}

SQSliceIndexPositionEditor::~SQSliceIndexPositionEditor()
{
}

//------------------------------------------------------------------------------

void SQSliceIndexPositionEditor::starting()
{
    ::fwData::Image::sptr image = std::dynamic_pointer_cast<::fwData::Image>(m_image->getObject());
    this->updateImageInfos(image);
    this->updateSliceTypeFromImg(m_orientation);
}

//------------------------------------------------------------------------------

void SQSliceIndexPositionEditor::configuring()
{
    SLM_ASSERT("Missing image.", m_image && m_image->getObject());

    if(m_qOrientation == "axial" )
    {
        m_orientation = Z_AXIS;
    }
    else if(m_qOrientation == "frontal" )
    {
        m_orientation = Y_AXIS;
    }
    else if(m_qOrientation == "sagittal" )
    {
        m_orientation = X_AXIS;
    }
    else
    {
        SLM_FATAL("The value for the qml property \"orientation\" can only be axial, frontal or sagittal.");
    }
}

//------------------------------------------------------------------------------

void SQSliceIndexPositionEditor::stopping()
{
}

//------------------------------------------------------------------------------

void SQSliceIndexPositionEditor::updating()
{
    ::fwData::Image::sptr image = std::dynamic_pointer_cast<::fwData::Image>(m_image->getObject());
    bool imageIsValid = ::fwDataTools::fieldHelper::MedicalImageHelpers::checkImageValidity( image );

    if (imageIsValid)
    {
        this->updateImageInfos(image);
        this->updateSliceIndexFromImg();

    }
}

//------------------------------------------------------------------------------

void SQSliceIndexPositionEditor::updateSliceTypeFromImg(Orientation type )
{
    // Update Type Choice

    ::fwData::Image::sptr image = std::dynamic_pointer_cast<::fwData::Image>(m_image->getObject());
    this->updateSliceIndexFromImg();
}

//------------------------------------------------------------------------------

void SQSliceIndexPositionEditor::updateSliceIndexFromImg()
{
    ::fwData::Image::sptr image = std::dynamic_pointer_cast<::fwData::Image>(m_image->getObject());

    if (::fwDataTools::fieldHelper::MedicalImageHelpers::checkImageValidity(image))
    {
        // Get Index
        std::string fieldID = *SLICE_INDEX_FIELDID[m_orientation];
        OSLM_ASSERT("Field "<<fieldID<<" is missing", image->getField( fieldID ) );
        unsigned int index = image->getField< ::fwData::Integer >( fieldID )->value();

        // Update QSlider
        int max = 0;
        if(image->getNumberOfDimensions() > m_orientation)
        {
            max = static_cast<int>(image->getSize()[m_orientation]-1);
        }
        m_slider->setProperty("minimumValue", 0);
        m_slider->setProperty("maximumValue", max);
        m_slider->setProperty("value", index);
    }
}

//------------------------------------------------------------------------------

void SQSliceIndexPositionEditor::sliderChanged(int newValue)
{
    ::fwData::Image::sptr image = std::dynamic_pointer_cast<::fwData::Image>(m_image->getObject());

    std::string fieldID = *SLICE_INDEX_FIELDID[m_orientation];
    OSLM_ASSERT("Field "<<fieldID<<" is missing", image->getField( fieldID ));
    image->getField< ::fwData::Integer >( fieldID )->value() = newValue;

    auto sig = image->signal< ::fwData::Image::SliceIndexModifiedSignalType >(
        ::fwData::Image::s_SLICE_INDEX_MODIFIED_SIG);
    ::fwCom::Connection::Blocker block(sig->getConnection(this->slot(s_UPDATE_SLICE_INDEX_SLOT)));
    sig->asyncEmit(m_axialIndex->value(), m_frontalIndex->value(), m_sagittalIndex->value());
    updatedSliceIndex(m_axialIndex->value(), m_frontalIndex->value(), m_sagittalIndex->value());
}

//------------------------------------------------------------------------------

void SQSliceIndexPositionEditor::orientationChanged(int orientation)
{
    Orientation type = static_cast< Orientation >( orientation );
    OSLM_ASSERT("Bad slice type "<<type, type == X_AXIS ||
                type == Y_AXIS ||
                type == Z_AXIS );
    int oldType = static_cast< int > ( m_orientation );
    // Change slice type
    m_orientation = type;

    // Fire the signal
    updatedSliceType(oldType, type);
    this->updateSliceIndexFromImg();

}

}
