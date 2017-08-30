import QtQuick 2.6
import Ui 1.0 as Ui

Item{
    id: root;
    width: 1000
    height: 1000

    property var previousObject: undefined;

    Item {
        id: container;

        anchors.fill: parent;
    }

    function compile() {
        var qmlObject = undefined;
        //        App.editor.issues.clear();

        try {
            qmlObject = Qt.createQmlObject(App.compiler.source, container, "root");

            if (previousObject) {
                previousObject.destroy();
            }
            previousObject = qmlObject;
            App.compiler.failed = false;
        }
        catch (exc) {
            var errors = exc.qmlErrors;

            for (var i = 0; i < errors.length; i++) {
                var error = errors[i];
                //                App.editor.issues.addError(error.lineNumber, error.columnNumber, error.message);
                console.error("Compile error in at line",error.lineNumber," column",error.columnNumber," :",error.message);
            }

            App.compiler.failed = true;
        }
    }

    Connections {
        target: App.compiler;
        onSourceChanged: { root.compile(); }
    }
}
