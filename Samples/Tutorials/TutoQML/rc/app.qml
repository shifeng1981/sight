import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0
import com.wurmel.io 1

ApplicationWindow {
    id: mainWindow
    title: qsTr("QML App")

    Item {
        width: 800
        height: 600
        focus: true
        Keys.onEscapePressed: {
            testId.exiting()
            Qt.quit()
            event.accepted = true
        }
        Rectangle {
            id: container
            width: 300
            height: 400
            color: "#204040"
            Text {
                width: container.width
                text: "Une app en QML"
                color: "#FFFFFF"
                horizontalAlignment: Text.AlignCenter
                clip: true
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    container.color = "#208080"
                    testId.in()
                }
                onExited: {
                    container.color = "#204040"
                    testId.out()
                }
            }            
        }
        Test {
            id: testId
        }
        Component.onCompleted: {
            testId.onSend.connect(function(value) {
                container.width = 300 - value
                container.height = 400 - value
                if (value > 10) {
                    console.log("received from C++ : " + value);                    
                }
            });            
        }
    }
}