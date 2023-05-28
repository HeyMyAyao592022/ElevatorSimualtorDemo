import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0

Rectangle {
    id: settingRec
    color: "transparent"
    Layout.fillWidth: true
    Layout.fillHeight: true

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

    // 信号
    signal enterSimulator()

    // 属性
    property int currIndex: 0

    function finishSetting(){
        // 调用相关函数
        setting_type.finishSetting()
        setting_basic.finishSetting()
        // 发出信号
        enterSimulator()
    }

    Rectangle{
        id: btn_finish
        width: 38
        height: 38
        radius: 16
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.rightMargin: 230
        anchors.bottomMargin: 40
        color: primary

        Image {
            width: 20
            height: 20
            id: img_finish
            anchors.centerIn: parent
            anchors.margins: 20
            source: "qrc:/icons/icons/done.png"
            fillMode: Image.PreserveAspectFit
            clip: true
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: {
                settingRec.finishSetting()
            }
            onEntered: {
                btn_finish.color = Qt.rgba(108/255,183/255,176/255,1)
            }
            onExited: {
                btn_finish.color = primary
            }
            onPressed: {
                btn_finish.color = Qt.rgba(98/255,173/255,166/255,1)
            }
            onReleased: {
                btn_finish.color = primary
            }
        }
    }

    RowLayout{
        id: option_bar
        spacing: 10
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 10


        Text{
            id: text_basic
            text: "基础"
            color: primary
            font.pixelSize: fontSizeMedium
            font.family: "微软雅黑"
            font.weight: Font.DemiBold

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    changeCurrIndex(0)
                }
                //按下
                onPressed: {
                    text_basic.color = Qt.rgba(138/255,213/255,206/255,1)
                }
                //指针进入
                onEntered: {
                    text_basic.color = primary
                }
                //指针退出
                onExited: {
                    if(currIndex !== 0)
                        text_basic.color = colorText
                }
                onReleased: {
                    if(currIndex !== 0)
                        text_basic.color = colorText
                }
            }


        }

        Text{
            id: text_type
            text: "类型"
            color: colorText
            font.pixelSize: fontSizeMedium
            font.family: "微软雅黑"
            font.weight: Font.DemiBold

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    changeCurrIndex(1)
                }
                //按下
                onPressed: {
                    text_type.color = Qt.rgba(138/255,213/255,206/255,1)
                }
                //指针进入
                onEntered: {
                    text_type.color = primary
                }
                //指针退出
                onExited: {
                    if(currIndex !== 1)
                        text_type.color = colorText
                }
                onReleased: {
                    if(currIndex !== 1)
                        text_type.color = colorText
                }
            }
        }

    }

    SwipeView{
        id: view_swipe
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: option_bar.bottom
        anchors.bottom: parent.bottom
        orientation: Qt.Horizontal
        currentIndex: 0
        interactive: false

        SettingBasic{
            id: setting_basic
        }

        SettingType{
            id: setting_type
        }
    }

    function changeCurrIndex(index){
        if(currIndex !== index){
            switch(currIndex){
            case 0:
                text_basic.color = colorText
                setting_type.setProp(setting_basic.getElNum())
                //btn_finish.visible = false
                break
            case 1:
                text_type.color = colorText
                break
            }

            switch(index){
            case 0:
                text_basic.color = primary
                //btn_finish.visible = true
                break
            case 1:
                text_type.color = primary
                break
            }

            currIndex = index
            view_swipe.currentIndex = currIndex
        }
    }
}
