import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0

ApplicationWindow {
    id: mainWindow
    title: qsTr("QML App")

    Item {
        width: 800
        height: 600
        focus: true
        Keys.onEscapePressed: {
            QMLObjectName.exiting()
            Qt.quit()
            event.accepted = true
        }
        Rectangle {
            id: container
            width: 300
            height: 400
            color: "#204040"
            Text {
                width: parent.width
                text: "Une app en QML"
                color: "#FFFFFF"
                horizontalAlignment: Text.AlignCenter
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    container.color = "#208080"
                    QMLObjectName.in()
                }
                onExited: {
                    container.color = "#204040"
                    QMLObjectName.out()
                }
            }            
        }
        Component.onCompleted: {
            QMLObjectName.onSend.connect(function(value) {
                console.log("received from C++ : " + value);
            });            
        }
    }
}