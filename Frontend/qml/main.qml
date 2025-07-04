// main.qml
import QtQuick 2.15
import QtQuick.Window 2.15

Window {
    width: 640
    height: 480
    visible: true
    title: "STM32 Button Indicator"

    // Reference the C++ ButtonController object exposed to QML
    // The 'buttonController' name must match the setContextProperty call in C++
    ButtonController {
        id: buttonLogic
    }

    Rectangle {
        id: background
        anchors.fill: parent
        color: "#282C34" // Dark background

        Column {
            anchors.centerIn: parent
            spacing: 20

            Text {
                text: "Press User Button on STM32F769 Discovery"
                font.pixelSize: 24
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                width: parent.width
            }

            // The indicator icon (a simple circle for demonstration)
            Rectangle {
                id: indicator
                width: 100
                height: 100
                radius: 50 // Makes it a circle
                anchors.horizontalCenter: parent.horizontalCenter
                border.color: "white"
                border.width: 2

                // Bind the color to the isButtonPressed property from the C++ backend
                // If buttonLogic.isButtonPressed is true, color is green, otherwise red.
                color: buttonLogic.isButtonPressed ? "green" : "red"

                // Optional: Add a transition for smooth color change
                Behavior on color {
                    ColorAnimation { duration: 150 }
                }

                Text {
                    text: buttonLogic.isButtonPressed ? "ON" : "OFF"
                    font.pixelSize: 30
                    color: "white"
                    anchors.centerIn: parent
                }
            }
        }
    }
}
