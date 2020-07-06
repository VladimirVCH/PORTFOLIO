import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

// игра

Page
{
  property string imageNum: "qrc:/Images/mainBoxer.jpg"
  property bool isServer: my_backend.isServer()

  background: Rectangle { color: "white" }
  font.pixelSize: textSize

  Timer
  {
    id: timerGamePage
    interval: 3000
    running: false
    repeat: false
    triggeredOnStart: false
    onTriggered:
    {
      generalStack.currentIndex = 0
      imageNum = "qrc:/Images/mainBoxer.jpg"
      statusB.value = 100
    }
  }

  Label
  {
    id: gameLabel
    text: my_backend._text
    anchors
    {
      horizontalCenter: parent.horizontalCenter
      bottom: parent.bottom
      bottomMargin: 20
    }

    onTextChanged: imageArea.border.color = "black"
  }

  Label
  {
    id: players
    font.pixelSize: 24
    text: "(you) " + myName + " VS " + my_backend.namePlayer
    anchors
    {
      horizontalCenter: parent.horizontalCenter
      top: parent.top
      topMargin: 35
    }
  }

  ProgressBar
  {
    id: statusB
    value: 100
    height: 20
    from: 0
    to: 100
    anchors
    {
      left: parent.left
      top: parent.top
      right: parent.right
      topMargin: 5
      leftMargin: 5
      rightMargin: 5
    }
  }

  Rectangle
  {
    id: imageArea
    width: 250
    height: 300
    radius: 30
    border
    {
      color: "black"
      width: 3
    }

    anchors
    {
      horizontalCenter: parent.horizontalCenter
      verticalCenter: parent.verticalCenter
    }

    Image
    {
      id: mainImage
      fillMode: Image.PreserveAspectFit
      opacity: 1
      source: imageNum
      anchors
      {
        top: parent.top
        bottom: parent.bottom
        left: parent.left
        right: parent.right
        topMargin: 5
        bottomMargin: 5
        leftMargin: 5
        rightMargin: 5
      }
    }
  }

  Custom_b
  {
    id: hitLeft
    textButton: "Hit Left"
    enabled: my_backend.attack
    anchors
    {
      left: parent.left
      top: parent.top
      leftMargin: 15
      topMargin: 100
    }

    onClicked: my_backend.hitLeft()
  }

  Custom_b
  {
    id: blockLeft
    textButton: "Block Left"
    enabled: my_backend.block
    anchors
    {
      top: hitLeft.bottom
      left: parent.left
      leftMargin: 15
      topMargin: 20
    }

    onClicked: my_backend.blockLeft()
  }

  Custom_b
  {
    id: hitRight
    textButton: "Hit Right"
    enabled: my_backend.attack
    anchors
    {
      right: parent.right
      top: parent.top
      rightMargin: 15
      topMargin: 100
    }

    onClicked: my_backend.hitRight()
  }

  Custom_b
  {
    id: blockRight
    textButton: "Block Right"
    enabled: my_backend.block
    anchors
    {
      top: hitRight.bottom
      right: parent.right
      rightMargin: 15
      topMargin: 20
    }

    onClicked: my_backend.blockRight()
  }

  Custom_b
  {
    id: log
    textButton: "Log"
    enabled: false
    anchors
    {
      bottom: parent.bottom
      left: parent.left
      leftMargin: 15
      bottomMargin: 60
    }

    onClicked:
    {
      popup.open()
      textPopup.text = my_backend.logFileRead()
    }
  }

  Popup
  {
    id: popup
    padding: 10
    width: 400
    height: 300
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

    contentItem: ScrollView
    {
      id: view
      anchors.fill: parent
      TextArea
      {
        id: textPopup
      }
    }
  }

  Back_button
  {
    id: back5
    enabled: false
    onClicked:
    {
      generalStack.currentIndex = 0
      back5.enabled = false
      log.enabled = false
      p4button = false
      p4Connection = "Waiting for client to connect"
      my_backend.endConnGUI()
    }
  }

  Connections
  {
    target: statusB
    onValueChanged: if(statusB.value <= 0)
                      my_backend.lostGame()
  }

  Connections
  {
    target: my_backend
    onWin:
    {
      imageNum = "qrc:/Images/boxerWin.jpg"
      back5.enabled = true
      log.enabled = true
    }
  }

  Connections
  {
    target: my_backend
    onLost:
    {
      imageNum = "qrc:/Images/lostBoxer.jpg"
      back5.enabled = true
      log.enabled = true
    }
  }

  Connections
  {
    target: my_backend
    onChangeHealth:
    {
      statusB.value -= _health
      imageArea.border.color = "red"
    }
  }

  Connections
  {
    target: my_backend
    onClientDisconnected:
    {
      back5.enabled = false
      log.enabled = false
      p4button = false
      p4Connection = "Waiting for client to connect"
      timerGamePage.start()
    }
  }
}
