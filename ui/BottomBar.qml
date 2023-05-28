import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Imagine 2.12
import QtQuick.Window 2.0

Rectangle{
    id: bottomBar
    Layout.fillWidth: true
    Layout.preferredHeight: 30
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

    // 摄像机按钮信号
    signal cameraBtnClicked(var model, var index)

    RowLayout{
        id: bottomBarLayout
        anchors.fill: parent

        FlatButton{
            width: 32
            height: 28
            clr_exit: primary
            clr_release: primary
            clr_enter: Qt.rgba(85/255,185/255,175/255,1)
            clr_click: Qt.rgba(61/255,150/255,141/255,1)
            color: primary
            clip: true
            img_src: "qrc:/icons/icons/remote.png"
            icon_margin: 5
        }

        FlatButton{
            width: 32
            height: 28
            img_src: "qrc:/icons/icons/error.png"
            Layout.leftMargin: 5
            icon_margin: 5
        }

        Text {
            text: qsTr("0")
            color: colorSubText
            font.family: "微软雅黑"
            font.weight: Font.DemiBold
            font.pixelSize: fontSizeSmall
        }

        FlatButton{
            width: 32
            height: 28
            img_src: "qrc:/icons/icons/warning.png"
            Layout.leftMargin: 5
            icon_margin: 5
        }

        Text {
            text: qsTr("0")
            color: colorSubText
            font.family: "微软雅黑"
            font.weight: Font.DemiBold
            font.pixelSize: fontSizeSmall
        }

        Rectangle{
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            color: "transparent"

            RowLayout{
                id: camera_btn_layout
                anchors.centerIn: parent
                Layout.fillWidth: true
                spacing: 0
                // 当前模式
                property int currentModel: 2
                FeatureRadioButton{
                    id: btn_camera_distant
                    img_src: "qrc:/icons/icons/camera1.png"
                    modelName: "全景"
                    height: 28
                    width: 65
                    onClickActive: {
                        // 发出信号
                        bottomBar.cameraBtnClicked(0,btn_camera_distant.modelIndex)
                        // 切换 spin box
                        if(camera_btn_layout.currentModel !== 0){
                            spinBox_minotor.visible = false
                            spinBox_follow.visible = false
                            camera_btn_layout.currentModel = 0
                        }
                    }
                }

                FeatureRadioButton{
                    id: btn_camera_follow
                    img_src: "qrc:/icons/icons/camera2.png"
                    modelName: "跟随"
                    height: 28
                    width: 65
                    onClickActive: {
                        // 发出信号
                        bottomBar.cameraBtnClicked(1,btn_camera_follow.modelIndex)
                        // 切换 spin box
                        if(camera_btn_layout.currentModel !== 1){
                            spinBox_follow.visible = true
                            spinBox_minotor.visible = false
                            camera_btn_layout.currentModel = 1
                        }
                    }
                }

                FeatureRadioButton{
                    id: btn_camera_minotor
                    img_src: "qrc:/icons/icons/camera3.png"
                    modelName: "观察"
                    height: 28
                    width: 65
                    onClickActive: {
                        // 发出信号
                        bottomBar.cameraBtnClicked(2,btn_camera_minotor.modelIndex)
                        // 切换 spin box
                        if(camera_btn_layout.currentModel !== 2){
                            spinBox_minotor.visible = true
                            spinBox_follow.visible = false
                            camera_btn_layout.currentModel = 2
                        }
                    }
                }

                SpinBox
                {
                    id: spinBox_follow
                    implicitWidth: 130
                    visible: false
                    from: 0
                    to: items.length - 1
                    value: 0
                    property var items: ["E00", "E01", "E02", "E03", "E04", "E05", "E06", "E07", "E08"]
                    // style
                    validator: DoubleValidator
                    {
                        bottom: Math.min(spinBox_follow.from, spinBox_follow.to)
                        top:  Math.max(spinBox_follow.from, spinBox_follow.to)
                    }
                    contentItem:
                        Text{
                        anchors.centerIn: parent
                        text: spinBox_follow.valueToText()
                        font.pixelSize: fontSizeMedium
                        font.family: "Consolas"
                        font.weight: Font.DemiBold
                        color: colorText
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                    }
                    // logic
                    valueFromText: function(text)
                    {
                        for (var i = 0; i < items.length; ++i)
                        {
                            if (items[i].toLowerCase().indexOf(text.toLowerCase()) === 0)
                                return i
                        }
                        return spinBox.value
                    }
                    function valueToText(){
                        return items[spinBox_follow.value]
                    }
                    onValueChanged: {
                        // 切换视角
                        if(camera_btn_layout.currentModel === 1){
                            btn_camera_follow.modelIndex = spinBox_follow.value
                            bottomBar.cameraBtnClicked(1,btn_camera_follow.modelIndex)
                        }
                    }
                }

                SpinBox
                {
                    id: spinBox_minotor
                    implicitWidth: 130
                    from: 0
                    to: items.length - 1
                    value: 0
                    property var items: ["Area1", "Area2", "Area3", "Area4"]
                    // style
                    validator: DoubleValidator
                    {
                        bottom: Math.min(spinBox_minotor.from, spinBox_minotor.to)
                        top:  Math.max(spinBox_minotor.from, spinBox_minotor.to)
                    }
                    contentItem:
                        Text{
                        anchors.centerIn: parent
                        text: spinBox_minotor.valueToText()
                        font.pixelSize: fontSizeMedium
                        font.family: "Consolas"
                        font.weight: Font.DemiBold
                        color: colorText
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                    }
                    // logic
                    valueFromText: function(text)
                    {
                        for (var i = 0; i < items.length; ++i)
                        {
                            if (items[i].toLowerCase().indexOf(text.toLowerCase()) === 0)
                                return i
                        }
                        return spinBox.value
                    }
                    function valueToText(){
                        return items[spinBox_minotor.value]
                    }
                    onValueChanged: {
                        // 切换视角
                        if(camera_btn_layout.currentModel === 2){
                            btn_camera_minotor.modelIndex = spinBox_minotor.value
                            bottomBar.cameraBtnClicked(2,btn_camera_minotor.modelIndex)
                        }
                    }
                }
            }

        }

        FlatButton{
            width: 32
            height: 28
            img_src: "qrc:/icons/icons/feedback.png"
            icon_margin: 5
        }

        FlatButton{
            width: 32
            height: 28
            img_src: "qrc:/icons/icons/msg.png"
            Layout.rightMargin: 10
            Layout.leftMargin: 5
            icon_margin: 5
        }
    }
}
