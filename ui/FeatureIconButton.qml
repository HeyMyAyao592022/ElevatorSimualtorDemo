import QtQuick 2.0

Rectangle {
    id: rec

    property alias img_src: icon.source

    property color clr_enter: Qt.rgba(0,0,0,0.2)
    property color clr_exit: "transparent"
    property color clr_click: Qt.rgba(0,0,0,0.6)
    property color clr_release: "transparent"

    //自定义点击信号
    signal clickedActive()
    signal release()

    width: 30
    height: 30
    radius: 10

    color:"transparent"

    Image {
        id: icon
        width: 25
        height: 25
        source: "qrc:/camera.png"
        fillMode: Image.PreserveAspectFit
        clip: true
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 5
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true

        //接受左键和右键输入
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: {
            parent.clickedActive()
        }

        //按下
        onPressed: {
            color = clr_click
        }

        //释放
        onReleased: {
            color = clr_enter
            parent.release()
        }

        //指针进入
        onEntered: {
            color = clr_enter
        }

        //指针退出
        onExited: {
            color = clr_exit
        }
    }
}

