import QtQuick 2.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0
import com.fw4spl 1

Rectangle {
    objectName: "mainWindow"
    id: mainWindow
    //title: qsTr("TutoGUI")
    visible: true


    width: 800
    height: 600

    SImageReader {
        id: myReader
        filepath: "./patient.vtk"
        onReadDone: {
            console.log("Read done... Image = {" + image + "}");
            myRenderer.start()
            myRenderer.configure();
            myRenderer.update();
        }
        onImageChanged: {
            console.log(myReader.image)
            myRenderer.image = myReader.image
            console.log(myRenderer.image)
        }
    }

    Rectangle {
        id : vtkRenderWindowContainer
        anchors.fill: parent
        border.width: 4
        color: "black"

        FrameBuffer {
            id: vtkRenderWindow
            anchors.fill: parent
            focus:true
            onReady: {
                console.log("pipeline ready");
                myReader.start();
                myReader.configure();
                myReader.update();
            }
        }
    }



    SRenderer {
        id: myRenderer
        target:vtkRenderWindow
    }
}
