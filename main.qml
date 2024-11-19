import QtQuick 2.15
import QtQuick.Window 2.15

Window {
    width: Screen.width
    height: Screen.height
    title: qsTr("Smart Parking")
    flags: Qt.FramelessWindowHint
    visible: true
    color: "yellow"

    Rectangle{
        height: 100
        width: parent.width
        color: "transparent"
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        id: schoolName

        Text {
            text: "THCS Tân Yên số 2"
            font.pixelSize: 50
            color: "black"
            anchors.centerIn: parent
        }
    }

    Grid {
        columns: 2
        rows: 2
        anchors.top: parent.top
        width: parent.width - 40
        height: parent.height - schoolName.height - 10
        anchors.centerIn: parent
        spacing: 20  // Padding giữa các phần tử trong GridLayout

        // Các phần tử trong grid
        Rectangle {
            width: parent.width / 2 - 20  // Chia đều
            height: parent.height / 2 - 20  // Chia đều
            color: "white"
        }

        Rectangle {
            width: parent.width / 2 - 20
            height: parent.height / 2 - 20
            color: "white"
        }

        Rectangle {
            width: parent.width / 2 - 20
            height: parent.height / 2 - 20
            color: "white"
        }

        Rectangle {
            width: parent.width / 2 - 20
            height: parent.height / 2 - 20
            color: "white"
        }
    }
}
