import QtQuick 2.7
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.0
import com.fw4spl.uiIO 1.0
import com.fw4spl.vtk 1.0
import com.fw4spl.vtk.adaptors 1.0
import com.fw4spl.uiVisuQt 1.0
import com.fw4spl 1.0


ApplicationWindow {
    id: rootWindow
    width: 800
    height: 600


    Material.theme: Material.Dark
    Material.accent: Material.color(Material.Green)
    visible: true

    Button {
        text: "Stop"
        highlighted: true
    }

    menuBar: MenuBar {
        Menu {
            title: "File"

            MenuItem {
                text: "Open image"
                onTriggered: imageReader.update()
            }
            MenuItem {
                text: "Open mesh"
                onTriggered: meshReader.update()
            }
            MenuItem {
                text: "Open texture"
                onTriggered: textureReader.update()
            }
        }
    }

    TypeHolder {
        objectType: "::fwData::Image"
    }

    /**
     * Service IO list
     * @{
     */
    SIOSelector {
        id: imageReader
        autoStart: true
        mode: "reader"
        dataClassName: "::fwData::Image"
        selectionMode: "exclude"
        onDone: {
            imageAdaptor.image = inout
            imageAdaptor.configure()
            imageAdaptor.start()
            imageAdaptor.update()
            genericRenderer.update()
            sliceSelector.image = inout
            sliceSelector.enabled = true
        }
    }

    SIOSelector {
        id: meshReader
        autoStart: true
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
        autoStart: true
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
                Layout.preferredWidth: 150

                color: "transparent"
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
                Layout.preferredWidth: 75

                color: "transparent"
                Button {
                    checkable: true
                    checked: false
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter

                    text: "Scan"
                    onCheckedChanged: {
                        sliceSelector.enabled = !checked && imageAdaptor.image
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
                Layout.preferredWidth: 70
                color: "transparent"

                Button {
                    text: "Snap"
                    anchors.fill: parent
                    anchors.margins: 4
                    onClicked: {
                        snapshotEditor.onSnapButton()
                    }
                    font.capitalization: Font.Capitalize
                }
            }
        }
    }
}
