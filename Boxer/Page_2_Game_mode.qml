import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

// выбор сервер или клиент

Page
{
  background: Rectangle { color: "white" }
  font.pixelSize: textSize

  Custom_b
  {
    id: createButton
    textButton: "Create game"
    anchors
    {
      horizontalCenter: parent.horizontalCenter
      verticalCenter: parent.verticalCenter
      horizontalCenterOffset: -120
    }

    onClicked:
    {
      my_backend.setHostPort(hostInput)
      my_backend.serverClicked()
      generalStack.currentIndex = 3
    }
  }

  Custom_b
  {
    id: joinButton
    textButton: "Join game"
    anchors
    {
      horizontalCenter: parent.horizontalCenter
      verticalCenter: parent.verticalCenter
      horizontalCenterOffset: 120
    }

    onClicked: generalStack.currentIndex = 2
  }

  Back_button
  {
    id: back2
    onClicked: generalStack.currentIndex--
  }
}
