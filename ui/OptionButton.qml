import QtQuick.Controls 2.0
import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Imagine 2.12

Rectangle {
    id: rec

    //property alias img_src: icon.source
    property string path_normal: "qrc:/icons/icons/end"
    property string path_checked: "qrc:/icons/icons/end"
    // 特效
    property color clr_enter: Qt.rgba(0,0,0,0.2)
    property color clr_exit: "transparent"
    property color clr_click: Qt.rgba(0,0,0,0.6)
    property color clr_release: "transparent"
    // 颜色
    readonly property color colorTitle: Qt.rgba(1,1,1,1)
    readonly property color colorText: Qt.rgba(1,1,1,0.8)
    readonly property color colorSubText: Qt.rgba(1,1,1,0.6)
    readonly property color colorPrimary: Qt.rgba(128/255,203/255,196/255,1)
    //自定义点击信号
    property int role: 0
    property bool checked: false
    property bool hoverAbove: false
    signal checkedActive(var num)
    // 属性
    height: 38
    width: 38
    radius: 0
    color:"transparent"

    RowLayout{
        id: row_item
        anchors.fill: parent

        Rectangle{
            id: sign_line
            Layout.fillHeight: true
            Layout.preferredWidth: 3
            Layout.topMargin: 1
            Layout.bottomMargin: 1
            color: checked ? colorPrimary : "transparent"
        }

        Image {
            id: icon
            Layout.preferredHeight: 24
            Layout.preferredWidth: 24
            //source: "qrc:/icons/icons/end"
            source: (checked | hoverAbove) ? path_checked : path_normal
            fillMode: Image.PreserveAspectFit
            clip: true
        }

    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true

        //接受左键和右键输入
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: {
            parent.checkedActive(role)
        }

        //指针进入
        onEntered: {
            hoverAbove = true
            //color = clr_enter
        }

        //指针退出
        onExited: {
            hoverAbove = false
            //color = clr_exit
        }
    }
}
