import QtQuick 2.1
import QtQuick.Controls 1.0

MenuBar {
    Menu {
        title: "File"

        MenuItem {
            text: "Open image"
            shortcut: "Ctrl+I"

            onTriggered: imageReader.update()
        }
        MenuItem {
            text: "Open mesh"
            shortcut: "Ctrl+M"
            onTriggered: meshReader.update()
        }
        MenuItem {
            text: "Open texture"
            shortcut: "Ctrl+T"
            onTriggered: textureReader.update()
        }
    }
}
