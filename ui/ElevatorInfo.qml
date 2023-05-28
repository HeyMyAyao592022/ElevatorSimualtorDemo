import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Imagine 2.12
import QtQuick.Window 2.0
import QtQuick 2.0
import QtQuick 2.2 as QQ2

Rectangle {
    radius: 10
    color: Qt.rgba(0,0,0,0.05)

    property int maxNum: 10
    property int el_name: 0
    property int el_type: 1
    property int currentFloor: 1
    property int passenger_num: 0
    property string describe: "可到达某层"
    // @author tu
    property int movingDirection: 0
    property int movingState: 0
    property int plan_maxfloor: 0
    property int plan_minfloor: 0
    // signal
    signal approachingNew()

    // style
    readonly property color colorDivider: Qt.rgba(1,1,1,0.03)
    readonly property color colorTitle: Qt.rgba(1,1,1,1)
    readonly property color colorText: Qt.rgba(1,1,1,0.8)
    readonly property color colorSubText: Qt.rgba(1,1,1,0.6)
    readonly property color colorPrimary: Qt.rgba(128/255,203/255,196/255,1)
    readonly property int fontSizeXXS: Qt.application.font.pixelSize * 0.65
    readonly property int fontSizeExtraSmall: Qt.application.font.pixelSize * 0.7
    readonly property int fontSizeSmall: Qt.application.font.pixelSize * 0.9
    readonly property int fontSizeMedium: Qt.application.font.pixelSize * 1
    readonly property int fontSizeLarge: Qt.application.font.pixelSize * 1.5
    readonly property int fontSizeExtraLarge: Qt.application.font.pixelSize * 2

    readonly property color subPrimary1: Qt.rgba(199/255,146/255,234/255,1)
    readonly property color subPrimary2: Qt.rgba(130/255,170/255,249/255,1)
    readonly property color subPrimary3: Qt.rgba(233/255,134/255,101/255,1)
    readonly property color subPrimary4: Qt.rgba(51/255,204/255,224/255,1)
    readonly property color subPrimary5: Qt.rgba(205/255,236/255,159/255,1)

    readonly property var colorArray: [subPrimary1, subPrimary2, subPrimary3, subPrimary4, subPrimary5]
    readonly property int colorSize: 5

    ColumnLayout{
        anchors.fill: parent
        anchors.margins: 10
        spacing: 3

        RowLayout{
            spacing: 10

            Text {
                id: text_name
                text: qsTr("E0" + el_name.toString())
                color: colorTitle
                font.family: "微软雅黑"
                font.pixelSize: fontSizeMedium
                font.bold: Font.DemiBold
            }

            ColumnLayout{
                Text {
                    id: text_num
                    text: qsTr("人数:" + passenger_num.toString() + "/" + maxNum.toString())
                    color: colorSubText
                    font.family: "微软雅黑"
                    font.pixelSize: fontSizeSmall
                    font.bold: Font.DemiBold
                }

                RowLayout{
                    Rectangle {
                        id:sign_type
                        Layout.preferredHeight: 8
                        Layout.preferredWidth: 8
                        Layout.alignment: Qt.AlignVCenter
                        color: colorArray[el_type % colorSize]
                        radius: 4
                        clip: true
                    }


                    Text {
                        id: text_sign_info
//                        text: qsTr(signInfo(el_name))
                        text: describe
                        Layout.preferredWidth: 65
                        color: colorSubText
                        font.family: "微软雅黑"
                        font.pixelSize: fontSizeExtraSmall
                        font.bold: Font.DemiBold
                        wrapMode: Text.WrapAnywhere
                    }

                }
            }
        }

        RowLayout{
            Image {
                id: icon_direction
                source: if(movingDirection == 0) "qrc:/icons/icons/up.png"; else if(movingDirection == 1) "qrc:/icons/icons/down.png"; else "qrc:/icons/icons/stop-light.png";
                width: 20
                height: 20
                fillMode: Image.PreserveAspectFit
                clip: true
                Layout.alignment: Qt.AlignTop
            }

            Text {
                id: text_curr_floor
                text: if(currentFloor < 10) qsTr("0"+currentFloor.toString()); else currentFloor.toString()
                color: colorTitle
                font.family: "Consolas"
                font.pixelSize: fontSizeMedium
                font.bold: Font.DemiBold
            }

//            ProgressBar {
//                id: progress_control
//                width: 40
//                value: 0.2
//                Layout.fillWidth: true
//                Layout.preferredHeight: 5
//                background: Rectangle {
//                    implicitWidth: progress_control.width
//                    implicitHeight: progress_control.height
//                    color: "transparent"
//                    radius: 3
//                }

//                contentItem: Item {
//                    Rectangle {
//                        width: progress_control.visualPosition * progress_control.width
//                        height: progress_control.height
//                        radius: 2
//                        color: signColor(el_name)
//                    }
//                }
//            }
            Text {
                text: if(movingDirection === 0)qsTr("=>:"+plan_maxfloor);else if(movingDirection === 1)qsTr("=>:"+plan_minfloor);
                      else qsTr("=>:无")
                id: floor
                color: colorSubText
                font.family: "Consolas"
                font.pixelSize: fontSizeSmall
                font.bold: Font.DemiBold
            }
        }
    }

//    // 颜色依类型而不同
//    function signColor(type){
//        switch(type){
//        case 0:
//        case 1:
//            return subPrimary1
//        case 2:
//        case 3:
//            return subPrimary2
//        case 4:
//        case 5:
//            return subPrimary3
//        case 6:
//        case 7:
//            return subPrimary4
//        case 8:
//        case 9:
//            return subPrimary5
//        default:
//            return primary
//        }
//    }

    function signInfo(type){
        switch(type){
        case 0:
        case 1:
            return "可到达每一层";
        case 2:
        case 3:
            return "可到达1、25~40层";
        case 4:
        case 5:
            return "可达1~25层";
        case 6:
        case 7:
            return "可达1、2~40层中的偶数层";
        case 8:
        case 9:
            return "可达1~39层中的奇数层";
        }
    }

//    function elevatorApproachingNewFloor(){
//        if(movingState === 0 || movingState === 1){
//            animation_progress.running = true
//        }
//    }

//    NumberAnimation{
//        id:animation_progress
//        target: progress_control
//        properties: "value"
//        from: 0
//        to: 1.0
//        duration: 2000
//    }
}
