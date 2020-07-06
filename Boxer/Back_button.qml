import QtQuick 2.12
import QtQuick.Controls 2.12

// кнопка возврата

Button
{
  width: 30
  height: 30
  anchors
  {
    bottom: parent.bottom
    left: parent.left

    leftMargin: 10
    bottomMargin: 10
  }

  background: Rectangle
  {
    border
    {
      color: pressed ? "green" : "white"
      width: 2
    }

    Image
    {
      id: butImage
      fillMode: Image.PreserveAspectFit
      opacity: enabled ? 1 : 0.5
      source: "qrc:/Images/back2.jpg"
      anchors
      {
        top: parent.top
        bottom: parent.bottom
        left: parent.left
        right: parent.right

        topMargin: 2
        leftMargin: 2
        rightMargin: 2
        bottomMargin: 2
      }
    }
  }
}
