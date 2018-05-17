import QtQuick 2.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0
import com.fw4spl.uiIO 1.0
import com.fw4spl.vtk 1.0
import com.fw4spl.vtk.adaptors 1.0
import com.fw4spl 1.0
import com.fw4spl.uiVisuQt 1.0

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
            imageAdaptor.image = inout
            imageAdaptor.configure()
            imageAdaptor.start()
            genericRenderer.update()
            sliceSelector.image = inout
            sliceSelector.enabled = true
        }
    }

    SIOSelector {
        id: meshReader
        mode: "reader"
        dataClassName: "::fwData::Mesh"
        selectionMode: "exclude"
        onDone: {
            meshAdaptor.mesh = inout
            meshAdaptor.configure();
            meshAdaptor.start();
            genericRenderer.update();

        }
    }

    SIOSelector {
        id: textureReader
        mode: "reader"
        dataClassName: "::fwData::Image"
        selectionMode: "exclude"
        onDone: {
            textureAdaptor.texture = inout;
            textureAdaptor.update();
        }
    }

    SRender {
        id: genericRenderer
        target: vtkRenderWindow

        scene: ({
            "renderMode": "auto",
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
            "slices": "3",
            "renderer": "default",
            "picker": "myPicker"
        })
    }

    SSnapshot {
        id: snapshotAdaptor
        renderService: genericRenderer

        config: ({
            "renderer": "default"
        })
    }

    SnapshotEditor {
        id: snapshotEditor
        Component.onCompleted: {
            configure()
            start();
        }
        onSnapped: {

            snapshotAdaptor.configure();
            snapshotAdaptor.start();
            snapshotAdaptor.snap(filepath);
        }
    }

    SMesh {
        id: meshAdaptor
        mesh: meshReader.inout
        renderService: genericRenderer

        config: ({
            "renderer": "default",
            "picker": "",
            "uvgen": "sphere"
        })
        onTextureApplied: {
            textureAdaptor.applyTexture(material);
        }
    }

    STexture {
        id: textureAdaptor
        texture: textureReader.inout
        renderService: genericRenderer
        config: ({
            "renderer": "default",
            "picker": "",
            "filtering": "linear",
            "wrapping": "repeat"
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
                    imageReader.configure()
                    imageReader.start()
                    meshReader.configure()
                    meshReader.start()
                    textureReader.configure()
                    textureReader.start()
                    genericRenderer.configure()
                    genericRenderer.start()
                    textureAdaptor.configure()
                    textureAdaptor.start()
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.minimumWidth: parent.width
            Layout.maximumHeight: 50
            spacing: 4

            Rectangle {
                Layout.fillHeight: true
                Layout.preferredWidth: 125
                ComboBox {
                    id: sliderIndexEditor
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter

                    model: ["One slice", "Three slice"]

                    currentIndex: 1
                    onActivated: {
                        imageAdaptor.updateSliceMode((index == 0) ? 1 : 3)
                    }
                }
            }

            Rectangle {
                Layout.fillHeight: true
                Layout.preferredWidth: 50
                Button {
                    checkable: true
                    checked: false
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter

                    iconSource: "sliceShow.png"
                    onCheckedChanged: {
                        sliceSelector.enabled = !checked && imageAdaptor.image
                        iconSource = (checked ? "sliceHide.png" : "sliceShow.png")

                    }
                }
            }

            SliceSelector {
                id: sliceSelector
                Layout.fillHeight: true
                Layout.fillWidth: true
                enabled: false

                onUpdatedSliceIndex: {
                    imageAdaptor.updateSliceIndex(axial, frontal, sagittal);
                }

                onUpdatedSliceType: {
                    imageAdaptor.updateSliceType(from, to);
                }
            }


            Rectangle {
                Layout.fillHeight: true
                Layout.preferredWidth: 30

                Button {
                    text: "Snap"
                    anchors.fill: parent
                    onClicked: {
                        snapshotEditor.onSnapButton()
                    }
                }
            }
        }
    }
}
