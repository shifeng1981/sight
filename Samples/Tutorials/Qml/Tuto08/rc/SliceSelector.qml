import QtQuick 2.7
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.0

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
        backend.update();
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
            Layout.preferredWidth: 100
            color: "transparent"


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

                    backend.orientationChanged(currentIndex);
                }
            }

        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true

            color: "transparent"

            Slider {
                id: slider
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                to: 99
                from: 0

                onFromChanged: {
                    value = 0;
                    onValueChanged(value);
                }

                onToChanged: {
                    value = from
                    onValueChanged(value);
                }

                onValueChanged: {
                    textField.text = parseInt(value, 10) + " / " + to
                    backend.sliderChanged(value)
                }
            }
        }
        Rectangle {
            Layout.fillHeight: true
            Layout.preferredWidth: 75

            color: "transparent"

            Text {
                id: textField
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                text: "? / ?"
                enabled: false
                color: Material.color(Material.Green)
                font.bold: true
            }
        }

    }

}
