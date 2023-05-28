import QtQuick.Controls 2.0
import QtQuick 2.0
import QtQuick.Layouts 1.12

Rectangle {
    id: rec

    // icon
    property alias img_src: icon.source

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

    // model
    property var textModel: ["F01", "F10", "F20", "F"]
    property var valModel: [0,1,2]

    // text
    property string textName: "跟随"
    property string textIndex: textModel[0]

    //自定义点击信号
    signal clickActive(var index)

    // 属性
    height: 28
    color:"transparent"

    RowLayout{
        Image {
            id: icon
            width: 25
            height: 25
            source: "qrc:/icons/icons/indicator_up"
            fillMode: Image.PreserveAspectFit
            clip: true
            Layout.margins: 10
        }

        Text {
            id: text_index
            text: qsTr(textIndex)
            color: colorSubText
            font.pixelSize: fontSizeSmall
            font.weight: Font.DemiBold
            font.family: "Consolas"
        }

        ColumnLayout{
            Image {
                id: indicator_up
                width: 12
                height: 12
                source: "qrc:/icons/icons/indicator_up.png"
                fillMode: Image.PreserveAspectFit
                clip: true
            }

            Image {
                id: indicator_down
                width: 12
                height: 12
                source: "qrc:/icons/icons/indicator_down.png"
                fillMode: Image.PreserveAspectFit
                clip: true
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
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
