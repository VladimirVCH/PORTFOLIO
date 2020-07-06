import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

// ожидание подключения клиента

Page
{
  background: Rectangle { color: "white" }
  font.pixelSize: textSize

  Timer
  {
    id: timerServer
    interval: 2000
    running: false
    repeat: false
    triggeredOnStart: false
    onTriggered: p4button = true
  }

  Rectangle
  {
    id: waitImage
    width: animation.width; height: animation.height + 8
    AnimatedImage { id: animation; source: "qrc:/Images/Loader2.gif" }
    anchors
    {
      horizontalCenter: parent.horizontalCenter
      verticalCenter: parent.verticalCenter
    }
  }

  Label
  {
    id: connLabel
    text: p4Connection
    anchors
    {
      horizontalCenter: parent.horizontalCenter
      bottom: waitImage.top
      bottomMargin: 50
    }
  }

  Custom_b
  {
    id: page4Button
    enabled: p4button
    textButton: "Next"
    anchors
    {
      horizontalCenter: parent.horizontalCenter
      top: waitImage.bottom
      topMargin: 50
    }

    onClicked: generalStack.currentIndex = 4
  }

  Back_button
  {
    id: back4
    onClicked:
    {
      my_backend.endConnection()
      p4button = false
      generalStack.currentIndex -= 2
    }
  }

  Connections
  {
    target: my_backend
    onGotConn:
    {
      p4Connection = "Connected"
      timerServer.start()
      my_backend.serverSend(myName)
    }
  }
}
