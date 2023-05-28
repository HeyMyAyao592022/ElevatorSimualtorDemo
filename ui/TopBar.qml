import QtQml 2.12
import QtQuick 2.5
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Imagine 2.12
import QtQuick.Window 2.0

Rectangle{
    id: topBar
    Layout.fillWidth: true
    Layout.maximumHeight: 40
    Layout.preferredHeight: 40
    color: colorBackground

    readonly property color colorBackground: Qt.rgba(41/255,45/255,62/255,1)
    readonly property color colorDivider: Qt.rgba(255/255,255/255,255/255,0.3)
    readonly property color primary: Qt.rgba(128/255,203/255,196/255,1)

    readonly property color colorTitle: Qt.rgba(1,1,1,1)
    readonly property color colorText: Qt.rgba(1,1,1,0.8)
    readonly property color colorSubText: Qt.rgba(1,1,1,0.6)

    readonly property int fontSizeExtraSmall: Qt.application.font.pixelSize * 0.6
    readonly property int fontSizeSmall: Qt.application.font.pixelSize * 0.8
    readonly property int fontSizeMedium: Qt.application.font.pixelSize * 1
    readonly property int fontSizeLarge: Qt.application.font.pixelSize * 1.5
    readonly property int fontSizeExtraLarge: Qt.application.font.pixelSize * 2

    Image {
        id: icon_app
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 25
        height: 25
        source: "qrc:/icons/icons/app_icon.png"
        fillMode: Image.PreserveAspectFit
        clip: true
        anchors.margins: 10
    }

    Row{
        id: text_tool
        anchors.left: icon_app.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        spacing: 10
        anchors.leftMargin: 10

        Text {
            id: text_file
            text: qsTr("文件")
            color: colorSubText
            font.family: "微软雅黑"
            font.weight: Font.DemiBold
            font.pixelSize: fontSizeMedium
            anchors.verticalCenter: parent.verticalCenter
        }


        Text {
            id: text_view
            text: qsTr("查看")
            color: colorSubText
            font.family: "微软雅黑"
            font.weight: Font.DemiBold
            font.pixelSize: fontSizeMedium
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            id: text_terminate
            text: qsTr("终端")
            color: colorSubText
            font.family: "微软雅黑"
            font.weight: Font.DemiBold
            font.pixelSize: fontSizeMedium
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            id: text_help
            text: qsTr("帮助")
            color: colorSubText
            font.family: "微软雅黑"
            font.weight: Font.DemiBold
            font.pixelSize: fontSizeMedium
            anchors.verticalCenter: parent.verticalCenter
        }

    }

    Text {
        id: text_title
        anchors.centerIn: parent
        text: qsTr("Elevator Simulator")
        color: Qt.rgba(1,1,1,0.8)
        font.family: "Consolas"
        font.weight: Font.DemiBold
        font.pixelSize: fontSizeMedium
    }

    Row{
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        FlatButton{
            id: btn_minmize
            img_src: "qrc:/icons/icons/minimize.png"
            onClickActive: {
                window.showMinimized()
            }
        }

        FlatButton{
            id: btn_maxmize
            img_src: "qrc:/icons/icons/maxmize.png"
            onClickActive: {
                if(!isMaximized){
                    window.showMaximized()
                    isMaximized = true
                    window.color = colorBackground
                }
                else{
                    window.showNormal()
                    isMaximized = false
                    window.color = "transparent"
                }
            }
        }

        FlatButton{
            id: btn_close
            img_src: "qrc:/icons/icons/close.png"
            clr_enter: Qt.rgba(1,0,0,0.8)
            clr_click: Qt.rgba(1,0,0,1)
            onClickActive: {
                window.close()
            }
        }
    }
}
