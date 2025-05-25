import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    width: Screen.width
    height: Screen.height
    title: qsTr("Smart Parking")
    id: root
    visible: true

    property bool in_out

    Column {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.margins: 10
        spacing: 10
        id: title

        // Tiêu đề 1
        Text {
            text: "THPT Tân Yên số 2"
            font.pointSize: 16
            font.bold: true
            color: "black"
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        // Tiêu đề 2
        Text {
            text: "Smart Parking System"
            font.pointSize: 25
            font.italic: true
            font.bold: true
            color: "black"
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    GridLayout {
        id: grid
        rows: 2
        columns: 2
        columnSpacing: 10
        rowSpacing: 10
        anchors {
            top: title.bottom
            topMargin: 10
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        // Hình chữ nhật 1
        Rectangle {
            id: outVehicle
            Layout.fillWidth: true
            Layout.fillHeight: true
            border.color: "black"
            border.width: 5
            color: "white"

            Column {
                anchors.fill: parent
                spacing: 10
                anchors.margins: 10

                Text {
                    text: qsTr("Xe vào")
                    font.pixelSize: 30
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Image {
                    id: outVehicleImg
                    // source: "file:/home/pi/License-Plate-Recognition/License_Plate_Picture/17_11_2024_21_27_42.jpg"
                    visible: false
                    fillMode: Image.PreserveAspectFit
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.topMargin: 10
                    width: parent.width * 0.9
                    height: parent.height * 0.9
                }
            }
        }

        // Hình chữ nhật 2
        Rectangle {
            id: inVehicle
            Layout.fillWidth: true
            Layout.fillHeight: true
            border.color: "black"
            border.width: 5
            color: "white"

            Column {
                anchors.fill: parent
                spacing: 10
                anchors.margins: 10

                Text {
                    text: qsTr("Xe ra")
                    font.pixelSize: 30
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Image {
                    id: inVehicleImg
                    // source: "file:/home/pi/License-Plate-Recognition/License_Plate_Picture/17_11_2024_21_27_42.jpg"
                    fillMode: Image.PreserveAspectFit
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.topMargin: 10
                    width: parent.width * 0.9
                    height: parent.height * 0.9
                    visible: false
                }
            }
        }

        // Hình chữ nhật 3
        Rectangle {
            border.color: "black"
            border.width: 5
            color: "white"
            Layout.fillWidth: true
            Layout.fillHeight: true
            Text {
                text: qsTr("License Plate")
                font.pixelSize: 35
                font.bold: true
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Column{
                anchors.centerIn: parent
                spacing: 20
                Text {
                    id: outVehilePlate
                    text: "Welcome!"
                    font.pixelSize: 35
                    font.bold: true
                }
                Text {
                    id: outVehileName
                    font.pixelSize: 35
                    font.bold: true
                }
                Text {
                    id: outVehileTime
                    font.pixelSize: 35
                    font.bold: true
                }
            }
        }

        // Hình chữ nhật 4
        Rectangle {
            border.color: "black"
            border.width: 5
            color: "white"
            Layout.fillWidth: true
            Layout.fillHeight: true
            Text {
                text: qsTr("License Plate")
                font.pixelSize: 35
                font.bold: true
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Column{
                anchors.centerIn: parent
                spacing: 20
                Text {
                    id: inVehilePlate
                    text: "Welcome!"
                    font.pixelSize: 35
                    font.bold: true
                }
                Text {
                    id: inVehileName
                    font.pixelSize: 35
                    font.bold: true
                }
                Text {
                    id: inVehileTime
                    font.pixelSize: 35
                    font.bold: true
                }
            }
        }
    }

    Timer{
        id:counterIn
        interval: 5000
        running: false
        onTriggered: {
            inVehicleImg.visible = false
        }
    }

    Timer{
        id:counterOut
        interval: 5000
        running: false
        onTriggered: {
            outVehicleImg.visible = false
        }
    }

    Timer{
        id:textOut
        interval: 5000
        running: false
        onTriggered: {
            outVehilePlate.text = "Welcome!"
            outVehileName.text = ""
            outVehileTime.text = ""
        }
    }

    Timer{
        id:textIn
        interval: 5000
        running: false
        onTriggered: {
            inVehilePlate.text = "Welcome!"
            inVehileName.text = ""
            inVehileTime.text = ""
        }
    }

    Rectangle{
        focus: true
        visible: false
        anchors.fill: parent
        Keys.onPressed: {
            if(event.key === Qt.Key_Left){
                surfaceManager.openBarieInException()
                console.log("Enter key pressed")
            }
            else if(event.key === Qt.Key_Right){
                surfaceManager.openBarieOutException()
                console.log("Space key pressed")
            }
        }
    }

    Connections{
        target: surfaceManager
        onImagePathInChanged:{
            inVehicleImg.source = "file:/home/pi/License-Plate-Recognition/License_Plate_Picture/" + surfaceManager.imagePathIn
            console.log("Image in path: ", inVehicleImg.source);
            inVehicleImg.visible = true
            in_out = false
            counterIn.start()
        }
        onImagePathOutChanged:{
            outVehicleImg.source = "file:/home/pi/License-Plate-Recognition/License_Plate_Picture/" + surfaceManager.imagePathOut
            console.log("Image out path: ", outVehicleImg.source);
            outVehicleImg.visible = true
            in_out = true
            counterOut.start()
        }
        onPlateNumberChanged:{
            if(in_out){
                outVehilePlate.text = surfaceManager.plateNumber
                textOut.start()
            }
            else{
                inVehilePlate.text = surfaceManager.plateNumber
                textIn.start()
            }
        }
        onNameChanged:{
            if(in_out){
                outVehileName.text = surfaceManager.name
                textOut.start()
            }
            else{
                inVehileName.text = surfaceManager.name
                textIn.start()
            }
        }
        onTimeChanged:{
            if(in_out){
                outVehileTime.text = surfaceManager.time
                textOut.start()
            }
            else{
                inVehileTime.text = surfaceManager.time
                textIn.start()
            }
        }
    }
}

