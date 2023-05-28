import QtQuick.Controls 2.0
import QtQuick 2.0
import QtQuick.Layouts 1.12

Rectangle {
    id: rec

    // style
    property alias img_src: icon.source
    property int icon_margin: 0
    // index
    property int modelIndex: 0
    // text
    property string modelName: "全景"
    // style
    property color clr_enter: Qt.rgba(0,0,0,0.2)
    property color clr_exit: "transparent"
    property color clr_click: Qt.rgba(0,0,0,0.6)
    property color clr_release: "transparent"
    readonly property color colorTitle: Qt.rgba(1,1,1,1)
    readonly property color colorText: Qt.rgba(1,1,1,0.8)
    readonly property color colorSubText: Qt.rgba(1,1,1,0.6)
    readonly property int fontSizeExtraSmall: Qt.application.font.pixelSize * 0.6
    readonly property int fontSizeSmall: Qt.application.font.pixelSize * 0.8
    readonly property int fontSizeMedium: Qt.application.font.pixelSize * 1
    readonly property int fontSizeLarge: Qt.application.font.pixelSize * 1.5
    readonly property int fontSizeExtraLarge: Qt.application.font.pixelSize * 2
    //自定义点击信号
    signal clickActive(var index)
    // 属性
    color:"transparent"

    RowLayout{
        anchors.centerIn: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.topMargin: 5
        anchors.bottomMargin: 5
        spacing: 3

        Image {
            id: icon
            source: "qrc:/icons/icons/end"
            fillMode: Image.PreserveAspectFit
            clip: true
            Layout.margins: icon_margin
        }

        Text {
            id: text_model_name
            text: qsTr(modelName)
            color: colorSubText
            font.pixelSize: fontSizeMedium
            font.family: "微软雅黑"
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            parent.clickActive(modelIndex)
        }
        //按下
        onPressed: {
            color = clr_click
        }
        //释放
        onReleased: {
            color = clr_enter
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
