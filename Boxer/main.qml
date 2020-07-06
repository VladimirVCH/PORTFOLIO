import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

Window
{
  id: generalWindow
  visible: true
  width: 710
  height: 460
  title: qsTr("Boxer v1.1")

  property int textSize: 20
  property string myName: "here"
  property string hostInput: "127.0.0.1"
  property string p4Connection: "Waiting for client to connect"
  property bool p4button: false

  StackLayout
  {
    id: generalStack
    anchors.fill: parent

    Page_1_Enter_name
    {
      id: p1_Name
    }

    Page_2_Game_mode
    {
      id: p2_Mode
    }

    Page_3_Client_mode
    {
      id: p3_Client
    }

    Page_4_Server_mode
    {
      id: p4_Server
    }

    Page_5_Game_main
    {
      id: p5_Game
    }
  }
}
