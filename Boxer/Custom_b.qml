import QtQuick 2.12
import QtQuick.Controls 2.12

// свой формат основной кнопки

Button
{
  property string textButton: "default"

  width: 200
  height: 50

  background: Rectangle
  {
    id: background_button_3
    radius: 15

    color: enabled ? "#FFFFFF" : "#A9A9A9"

    border
    {
      color: pressed ? "green" : "black"
      width: 3
    }
  }

  contentItem: Text
  {
    text: textButton
    color: "#000000"
    font.pixelSize: textSize
    font.bold: true

    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter
  }

  bottomPadding: 0
}
