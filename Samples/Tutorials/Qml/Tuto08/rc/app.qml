import QtQuick 2.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0
import com.fw4spl.uiIO 1.0
import com.fw4spl.vtk 1.0
import com.fw4spl.vtk.adaptors 1.0

Rectangle {
    id: rootWindow
    width: 800
    height: 600

    /**
     *  TopBar wrapper
     */
    Item {
        id: menuWrapper
        anchors.fill: parent

        TopBar {
          id: myTopMenu
        }

        states: State {
          name: "hasMenuBar"
          when: myTopMenu && !myTopMenu.__isNative

          ParentChange {
              target: myTopMenu.__contentItem
              parent: rootWindow
          }

          PropertyChanges {
              target: myTopMenu.__contentItem
              x: 0
              y: 0
              width: menuWrapper.width
          }
        }
    }

    /**
     * Service IO list
     * @{
     */
    SIOSelector {
        id: imageReader
        mode: "reader"
        dataClassName: "::fwData::Image"
        selectionMode: "exclude"
        onDone: {
            console.log("Image readed")
            console.log(inout)
            imageAdaptor.image = inout
            imageAdaptor.configure()
            imageAdaptor.start()
        }
    }

    SIOSelector {
        id: meshReader
        mode: "reader"
        dataClassName: "::fwData::Mesh"
        selectionMode: "exclude"
    }

    SIOSelector {
        id: textureReader
        mode: "reader"
        dataClassName: "::fwData::Image"
        selectionMode: "exclude"
    }

    SRender {
        id: genericRenderer
        target: vtkRenderWindow

        scene: ({
            "renderMode": "timer",
            "rendererList": [{
                "id": "default",
            }],
            "pickerList": [{
                "id": "myPicker",
                "vtkclass": "fwVtkCellPicker"
            }]
        })
    }

    SNegatoMPR {
        id: imageAdaptor
        image: imageReader.inout
        renderService: genericRenderer

        config: ({
            "mode": "3d",
            "slices": "1",
            "renderer": "default",
            "picker": "myPicker"
        })
    }

    /**
     * }@
     */

    ColumnLayout {
        spacing: 2
        anchors.fill: parent
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "black"

            FrameBuffer {
                id: vtkRenderWindow
                anchors.fill: parent

                onReady: {
                    console.log("Started")
                    imageReader.configure()
                    imageReader.start()
                    meshReader.configure()
                    meshReader.start()
                    textureReader.configure()
                    textureReader.start()
                    genericRenderer.configure()
                    genericRenderer.start()
                    genericRenderer.update()
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.minimumWidth: parent.width
            Layout.preferredHeight: 50
            color: "red"
        }
    }
}
