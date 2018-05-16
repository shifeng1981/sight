#ifndef SQSLICEINDEXPOSITIONEDITOR_HPP
#define SQSLICEINDEXPOSITIONEDITOR_HPP

#include "uiImageQt/config.hpp"

#include <fwDataTools/helper/MedicalImageAdaptor.hpp>

#include <fwServices/IQmlService.hpp>
#include <fwServices/QtObjectHolder.hpp>

#include <QQuickItem>

namespace uiImageQt
{

class UIIMAGEQT_CLASS_API   SQSliceIndexPositionEditor : public ::fwServices::IQmlService,
                                                         public ::fwDataTools::helper::MedicalImageAdaptor
{
    Q_OBJECT

    Q_PROPERTY(fwServices::QtObjectHolder *image MEMBER m_image)
    Q_PROPERTY(QString  orientation MEMBER m_qOrientation)
    Q_PROPERTY(QQuickItem *slider MEMBER m_slider)

public:
    UIIMAGEQT_API   SQSliceIndexPositionEditor();
    UIIMAGEQT_API   ~SQSliceIndexPositionEditor();

    UIIMAGEQT_API void    starting() override;
    UIIMAGEQT_API void    stopping() override;
    UIIMAGEQT_API void    configuring() override;
    UIIMAGEQT_API void    updating() override;

private:
    using ::fwDataTools::helper::MedicalImageAdaptor::Orientation;

protected:
    /// Update the editor slice type choice from the image slice type.
    UIIMAGEQT_API void updateSliceTypeFromImg(Orientation type );
    /// This method is called when the slider is move. Notify the slice index is modified.
    UIIMAGEQT_API void sliceIndexNotification(unsigned int index);
    /// Update the editor slider from the image slice index.
    UIIMAGEQT_API void updateSliceIndexFromImg();



public Q_SLOTS:
    UIIMAGEQT_API void  orientationChanged(int);
    UIIMAGEQT_API void  sliderChanged(int);

Q_SIGNALS:
    void updatedSliceIndex(int axial, int sagittal, int frontal);
    void updatedSliceType(int from, int to);



private:
    ::fwServices::QtObjectHolder    *m_image = nullptr;
    QString m_qOrientation = "axial";
    QQuickItem  *m_slider = nullptr;

    static const std::string* SLICE_INDEX_FIELDID[ 3 ];

};

}

#endif // SQSLICEINDEXPOSITIONEDITOR_HPP
