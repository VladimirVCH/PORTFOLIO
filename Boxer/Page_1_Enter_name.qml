import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

// ввод имени

Page
{
  background: Rectangle { color: "white" }
  font.pixelSize: textSize
  Component.onCompleted: nameEdit.forceActiveFocus()
  onVisibleChanged: nameEdit.forceActiveFocus()

  Label
  {
    id: nameLabel
    font.pixelSize: 30
    text: qsTr("Enter your name:")
    anchors
    {
      horizontalCenter: parent.horizontalCenter
      verticalCenter: parent.verticalCenter
      verticalCenterOffset: -80
    }
  }

  TextField
  {
    id: nameEdit
    cursorVisible: true
    placeholderText: myName
    font.pixelSize: 30
    anchors
    {
      horizontalCenter: parent.horizontalCenter
      top: nameLabel.bottom
      topMargin: 50
    }

    onTextChanged: myName = nameEdit.text
  }

  Custom_b
  {
    id: page1Button
    enabled: nameEdit.contentWidth > 0
    textButton: "Next"
    anchors
    {
      horizontalCenter: parent.horizontalCenter
      top: nameEdit.bottom
      topMargin: 50
    }

    onClicked: generalStack.currentIndex = 1
  }
}
