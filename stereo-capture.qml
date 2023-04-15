import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Window {
    property var margin: 9
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    ColumnLayout {
        RowLayout {
            Button {
                text: "Trigger"
                onClicked: {
                    manager.setDelay(delay.text.toString())
                    manager.captureAll()
                }
            }
        }
        RowLayout {
            Text {
                text: "Delay: "
            }
            TextField {
                text: "2.5"
                id: delay
            }
            Text {
                text: "ms"
            }
        }
    }


}
