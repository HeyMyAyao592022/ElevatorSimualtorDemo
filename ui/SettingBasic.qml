import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0

Item {
    function finishSetting(){
        //viewModel
        viewModel.elNum = spinbox_el_num.value
        viewModel.elMaxNum = spinbox_el_max_num.value
        viewModel.passengerNumber = spinbox_psg_num.value
        viewModel.elevatorInterval = spinbox_el_time.value
    }

    function getElNum(){
        return spinbox_el_num.value
    }

    RowLayout{
        id: rowLayout
        width: parent.width
        anchors.fill: parent
        spacing: 10

        ColumnLayout{
            id: colLayout1
            Layout.alignment: Qt.AlignTop
            Layout.preferredWidth: 100
            Layout.fillHeight: true
            Layout.topMargin: 10
            Layout.leftMargin: 15
            spacing: 5

            Text{
                text: "> 全部"
                color: primary
                font.pixelSize: fontSizeMedium
                font.family: "微软雅黑"
                font.weight: Font.DemiBold
            }

            Text{
                text: "> 乘客"
                color: colorSubText
                font.pixelSize: fontSizeMedium
                font.family: "微软雅黑"
                font.weight: Font.DemiBold
            }

            Text{
                text: "> 电梯"
                color: colorSubText
                font.pixelSize: fontSizeMedium
                font.family: "微软雅黑"
                font.weight: Font.DemiBold
            }
        }

        ColumnLayout{
            id: colLayout2
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 10

            Text{
                text: "设置"
                color: primary
                font.pixelSize: fontSizeExtraLarge
                font.family: "微软雅黑"
                font.weight: Font.DemiBold
                Layout.bottomMargin: 10
            }

            Row{
                Layout.fillWidth: true
                spacing: 5

                Text{
                    text: "乘客："
                    color: colorText
                    font.pixelSize: fontSizeMedium
                    font.family: "微软雅黑";
                    font.weight: Font.DemiBold
                }

                Text{
                    text: "数量"
                    color: primary
                    font.pixelSize: fontSizeMedium
                    font.family: "微软雅黑"
                    font.weight: Font.DemiBold
                }
            }

            Text{
                text: "设置模拟时出现的乘客数量"
                color: colorSubText
                font.pixelSize: fontSizeSmall
                font.family: "微软雅黑"
                font.weight: Font.DemiBold
            }

            SpinBox {
                id: spinbox_psg_num
                anchors.margins: 10
                implicitWidth: 150
                implicitHeight: 30
                editable: true
                value: 20
                stepSize:20
                from:0
                to:1000
                contentItem:
                    Text{
                    text: spinbox_psg_num.value
                    font.pixelSize: fontSizeMedium
                    font.family: "Consolas"
                    font.weight: Font.DemiBold
                    color: colorText
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                }
            }

            Row{
                Layout.fillWidth: true
                spacing: 5

                Text{
                    text: "电梯："
                    color: colorText
                    font.pixelSize: fontSizeMedium
                    font.family: "微软雅黑"
                    font.weight: Font.DemiBold
                }

                Text{
                    text: "数量"
                    color: primary
                    font.pixelSize: fontSizeMedium
                    font.family: "微软雅黑"
                    font.weight: Font.DemiBold
                }
            }

            Text{
                text: "设置模拟时出现的电梯数量"
                color: colorSubText
                font.pixelSize: fontSizeSmall
                font.family: "微软雅黑"
                font.weight: Font.DemiBold
            }

            SpinBox {
                id: spinbox_el_num
                anchors.margins: 10
                implicitWidth: 150
                implicitHeight: 30
                editable: true
                value: 10
                stepSize:1
                from:1
                to:10
                contentItem:
                    Text{
                    text: spinbox_el_num.value
                    font.pixelSize: fontSizeMedium
                    font.family: "Consolas"
                    font.weight: Font.DemiBold
                    color: colorText
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                }
            }

            Row{
                Layout.fillWidth: true
                spacing: 5

                Text{
                    text: "电梯："
                    color: colorText
                    font.pixelSize: fontSizeMedium
                    font.family: "微软雅黑"
                    font.weight: Font.DemiBold
                }

                Text{
                    text: "运行速度"
                    color: primary
                    font.pixelSize: fontSizeMedium
                    font.family: "微软雅黑"
                    font.weight: Font.DemiBold
                }
            }

            Text{
                text: "设置模拟时电梯运行一层楼所需的时间"
                color: colorSubText
                font.pixelSize: fontSizeSmall
                font.family: "微软雅黑"
                font.weight: Font.DemiBold
            }

            SpinBox {
                id: spinbox_el_time
                anchors.margins: 10
                implicitWidth: 150
                implicitHeight: 30
                editable: true
                value: 200
                stepSize:50
                from:100
                to:500

                property int decimals: 2
                property real realValue: value / 100
                textFromValue: function(value, locale)
                {
                    return Number(value / 100).toLocaleString(locale, 'f', spinbox_el_time.decimals)
                }

                valueFromText: function(text, locale)
                {
                    return Number.fromLocaleString(locale, text) * 100
                }

                contentItem:
                    Text{
                    text: spinbox_el_time.displayText
                    font.pixelSize: fontSizeMedium
                    font.family: "Consolas"
                    font.weight: Font.DemiBold
                    color: colorText
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                }
            }

            Row{
                Layout.fillWidth: true
                spacing: 5

                Text{
                    text: "电梯："
                    color: colorText
                    font.pixelSize: fontSizeMedium
                    font.family: "微软雅黑"
                    font.weight: Font.DemiBold
                }

                Text{
                    text: "满载人数"
                    color: primary
                    font.pixelSize: fontSizeMedium
                    font.family: "微软雅黑"
                    font.weight: Font.DemiBold
                }
            }

            Text{
                text: "设置模拟时出现的电梯最大搭乘人数"
                color: colorSubText
                font.pixelSize: fontSizeSmall
                font.family: "微软雅黑"
                font.weight: Font.DemiBold
            }

            SpinBox {
                id: spinbox_el_max_num
                anchors.margins: 10
                implicitWidth: 150
                implicitHeight: 30
                editable: true
                value: 10
                stepSize:1
                from:5
                to:15
                contentItem:
                    Text{
                    text: spinbox_el_max_num.value
                    font.pixelSize: fontSizeMedium
                    font.family: "Consolas"
                    font.weight: Font.DemiBold
                    color: colorText
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                }
            }

        }
    }
}
