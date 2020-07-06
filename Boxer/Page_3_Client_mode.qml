import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

// подключение к серверу

Page
{
  background: Rectangle { color: "white" }
  font.pixelSize: textSize
  onVisibleChanged: host.forceActiveFocus()

  Timer
  {
    id: timerClient
    interval: 2000
    running: false
    repeat: false
    triggeredOnStart: false
    onTriggered:
    {
      my_backend.clientSend(myName)
      generalStack.currentIndex = 4
    }
  }

  Label
  {
    id: hostText
    text: qsTr("Host:")
    font.pixelSize: 30
    anchors
    {
      horizontalCenter: parent.horizontalCenter
      verticalCenter: parent.verticalCenter
      verticalCenterOffset: -80
    }
   }

  TextField
  {
    id: host
    placeholderText: hostInput
    font.pixelSize: 30
    anchors
    {
      horizontalCenter: parent.horizontalCenter
      top: hostText.bottom
      topMargin: 50
    }

    onTextChanged: hostInput = host.text
  }

  Custom_b
  {
    id: page3Button
    enabled: host.contentWidth > 0
    textButton: "Next"
    anchors
    {
      horizontalCenter: parent.horizontalCenter
      top: host.bottom
      topMargin: 50
    }

    onClicked:
    {
      my_backend.setHostPort(hostInput)
      my_backend.clientClicked()
      timerClient.start()
    }
  }

  Back_button
  {
    id: back3
    onClicked: generalStack.currentIndex--
  }
}
