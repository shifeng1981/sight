import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0
import com.fw4spl 1.0

Item {
    id: root
    property var image

    signal updatedSliceIndex(int axial, int frontal, int sagittal)
    signal updatedSliceType(int from, int to);

    onImageChanged: {
        backend.image = image
        backend.configure();
        backend.start();
    }

    SSliceIndexPositionEditor {
        id: backend
        orientation: "axial"
        slider: slider

        onUpdatedSliceIndex: {
            root.updatedSliceIndex(axial, sagittal, frontal)

        }
    }

    RowLayout {
        anchors.fill: parent

        Rectangle {
            Layout.fillHeight: true
            Layout.preferredWidth: 75

            ComboBox {
                id: sliceType
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter

                model: ["Sagittal", "Frontal", "Axial"]

                currentIndex: 2
                onCurrentIndexChanged: {
                    if (!backend.isStarted()) {
                        return ;
                    }

                    console.log("Index == " + currentIndex)
                    backend.orientationChanged(currentIndex);
                }
            }

        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true

            Slider {
                id: slider
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                maximumValue: 250
                minimumValue: 0

                onMinimumValueChanged: {
                    value = minimumValue
                }

                onMaximumValueChanged: {
                    value = minimumValue
                }
                onValueChanged: {
                     textField.text = parseInt(value, 10) + " / " + maximumValue
                    backend.sliderChanged(value)
                }
            }
        }
        Rectangle {
            Layout.fillHeight: true
            Layout.preferredWidth: 75

            Text {
                id: textField
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                text: "0 / 99"
                enabled: false
            }
        }

    }

}
