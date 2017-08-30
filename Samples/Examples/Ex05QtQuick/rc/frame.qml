import QtQuick 2.6
import QtGraphicalEffects 1.0
import Render 1.0 as Render
import Ui 1.0 as Ui

Rectangle {
    id: root;
    color: "#282C34"
    width: parent.width
    height: parent.height
    clip: true;

    Ui.Header {
        id: header;

        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.top: parent.top;

        Ui.Button {
            objectName: "menu"
            anchors.verticalCenter: parent.verticalCenter;
            color: "transparent"
            border.color: "transparent"

            icon.type: icon.types.fa_navicon;
            onClicked: drawer.toggle();
        }

    }

    Ui.NavigationDrawer {
        id: drawer
        anchors.top: header.bottom
        anchors.bottom: parent.bottom

        position: Qt.LeftEdge
        visualParent: drawer


        Ui.Button {
            id: button1
            width: drawer.width;
            height: label.height + 6;
            objectName: "Ex05QtQuickConfig_1_openMesh"

            label.text: "Open Mesh";
            icon.type: icon.types.fa_cube;
            signal openClicked()
            onClicked: openClicked();
        }

        Ui.Button {
            id: button2
            anchors.top:button1.bottom;
            width: drawer.width;
            height: label.height + 6;
            objectName: "Ex05QtQuickConfig_1_openImage"

            label.text: "Open Image";
            icon.type: icon.types.fa_image;
            signal openClicked()
            onClicked: openClicked();
        }
    }

    Item {
        id: renderVTK
        y: header.height
        width: parent.width
        height:parent.height - header.height

        Render.VtkQuickItem {
            anchors.fill: parent
            objectName: "Ex05QtQuickConfig_1_renderingMesh"

        }
    }

    Item {
        x: parent.width - 300
        y: parent.height - 300
        width: 300
        height: 300

        MouseArea {
            anchors.fill: parent
            propagateComposedEvents: true

            onClicked: mouse.accepted = false;
            onPressed: mouse.accepted = false;
            onReleased: mouse.accepted = false;
            onDoubleClicked: mouse.accepted = false;
            onPositionChanged: mouse.accepted = false;
            onPressAndHold: mouse.accepted = false;
        }

        Image {
            id: mask

            source: "textures/ellipse.png"
            sourceSize: Qt.size(parent.width, parent.height)

            visible: false
        }


        InnerShadow {
              anchors.fill: parent
              radius: 16
              samples: 24
              horizontalOffset: -3
              verticalOffset: 3
              color: "#b00000"
              source: opMask
          }

        OpacityMask {
            id: opMask
            anchors.fill: parent
            source: renderVTK
            maskSource: mask
            visible: false
        }
    }

}
