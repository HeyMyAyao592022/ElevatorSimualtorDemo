import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Imagine 2.12
import QtQuick.Window 2.0
import QtQuick 2.0
import QtQuick 2.2 as QQ2

Rectangle {
    id: rec
    color: "transparent"
    anchors.left: parent.left
    opacity: 0

    // style
    readonly property color colorTitle: Qt.rgba(1,1,1,1)
    readonly property color colorText: Qt.rgba(1,1,1,0.8)
    readonly property color colorSubText: Qt.rgba(1,1,1,0.6)
    readonly property color colorPrimary: Qt.rgba(128/255,203/255,196/255,1)
    readonly property int fontSizeXXS: Qt.application.font.pixelSize * 0.65
    readonly property int fontSizeExtraSmall: Qt.application.font.pixelSize * 0.8
    readonly property int fontSizeSmall: Qt.application.font.pixelSize * 0.9
    readonly property int fontSizeMedium: Qt.application.font.pixelSize * 1
    readonly property int fontSizeLarge: Qt.application.font.pixelSize * 1.5
    readonly property int fontSizeExtraLarge: Qt.application.font.pixelSize * 2
    readonly property color subPrimary1: Qt.rgba(199/255,146/255,234/255,1)
    readonly property color subPrimary2: Qt.rgba(130/255,170/255,249/255,1)
    readonly property color subPrimary3: Qt.rgba(233/255,134/255,101/255,1)
    readonly property color subPrimary4: Qt.rgba(51/255,204/255,224/255,1)
    readonly property color subPrimary5: Qt.rgba(205/255,236/255,159/255,1)

    // 属性
    property bool upward: false
    property string str_name: "05"
    property int str_from: 25
    property int str_to: 30
    property int el_id: 0

    OpacityAnimator
    {
        id: animator_appear
        target: rec;
        from: 0;
        to: 1;
        duration: 450
        running: false
    }

    Component.onCompleted: {
        animator_appear.running = true
    }

    ColumnLayout{
        anchors.fill: parent
        spacing: 3
        anchors.leftMargin: 10
        anchors.rightMargin: 10

        Text {
            id: text_id
            text: qsTr("乘客" + str_name)
            color: colorTitle
            font.family: "微软雅黑"
            font.pixelSize: fontSizeMedium
            font.weight: Font.Normal
        }

        RowLayout{
            spacing: 3

            Text {
                id: text_status
                text: qsTr(">>")
                Layout.alignment: Qt.AlignTop
                color: colorText
                font.pixelSize: fontSizeMedium
                font.family: "Consolas"
                font.weight: Font.DemiBold
            }

            Rectangle{
                color: signColor(el_id)
                Layout.preferredHeight: 5
                Layout.preferredWidth: 5
                radius: 4
            }

            Text {
                id: text_chosen_elevator
                text: qsTr(signText(el_id))
                Layout.alignment: Qt.AlignTop
                color: colorText
                font.pixelSize: fontSizeMedium
                font.family: "Consolas"
                font.weight: Font.DemiBold
            }
        }

        RowLayout{
            spacing: 1

            Image {
                id: icon_direction
                source: upward ? "qrc:/icons/icons/up-small.png" : "qrc:/icons/icons/down-small.png"
                width: 15
                height: 15
                fillMode: Image.PreserveAspectFit
                clip: true
            }

            Text {
                id: text_from
                text: str_from < 10 ? "0" + str_from.toString() : str_from.toString()
                color: colorSubText
                font.family: "微软雅黑"
                font.pixelSize: fontSizeSmall
                font.bold: Font.DemiBold
            }
            Text {
                text: qsTr("=>")
                color: colorSubText
                font.family: "微软雅黑"
                font.pixelSize: fontSizeSmall
                font.bold: Font.DemiBold
            }
            Text {
                id: text_to
                text: str_to < 10 ? "0" + str_to.toString() : str_to.toString()
                color: colorSubText
                font.family: "微软雅黑"
                font.pixelSize: fontSizeSmall
                font.bold: Font.DemiBold
            }
        }
    }

    // 颜色依选择的电梯而不同
    function signColor(type){
        switch(type){
        case 0:
        case 1:
            return subPrimary1
        case 2:
        case 3:
            return subPrimary2
        case 4:
        case 5:
            return subPrimary3
        case 6:
        case 7:
            return subPrimary4
        case 8:
        case 9:
            return subPrimary5
        default:
            return primary
        }
    }

    function signText(type){
        if(type !== -1){
            if(type < 10)
                return "E0" + type.toString()
            else
                return "E" + type.toString()
        }
        else{
            return "无";
        }
    }
}
