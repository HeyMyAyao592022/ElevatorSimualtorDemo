import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0


Item {
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

    readonly property color subPrimary1: Qt.rgba(199/255,146/255,234/255,1)
    readonly property color subPrimary2: Qt.rgba(130/255,170/255,249/255,1)
    readonly property color subPrimary3: Qt.rgba(233/255,134/255,101/255,1)
    readonly property color subPrimary4: Qt.rgba(51/255,204/255,224/255,1)
    readonly property color subPrimary5: Qt.rgba(205/255,236/255,159/255,1)

    function analayseData(){
        //console.log("ana")
        // time
        text_consume_time.text = viewModel.getTotalTime()
        setElListModel()
    }

    function setElListModel(){
        listmodel_el.clear()
        // get data
        const tempArr = []
        const eN = viewModel.elNum
        //console.log("eN:" + eN)
        let maxCsm = -1
        let i = 0
        for(i = 0; i < eN; i++){
            let elCsm = elevators[i].getConsume()/10
            if(elCsm < 0){
                elCsm = 0
            }
            //console.log("csm:" + elCsm)
            tempArr.push(elCsm)
            // set model
            listmodel_el.append({consume: elCsm, share: 0.2})
            // get max
            if(maxCsm < elCsm){
                maxCsm = elCsm
            }
        }
        // set share
        for(i = 0; i < eN; i++){
            const currShare = tempArr[i]/maxCsm
            listmodel_el.get(i).share = currShare
            console.log("i " + currShare)
        }
    }

    Rectangle{
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.topMargin: 10
        color: "transparent"

        ColumnLayout{
            anchors.fill: parent
            spacing: 10
            RowLayout{
                spacing: 5
                Text{
                    text: "模拟结束"
                    color: colorText
                    font.pixelSize: fontSizeExtraLarge
                    font.family: "微软雅黑"
                    font.weight: Font.DemiBold
                    Layout.bottomMargin: 10
                }
            }

            RowLayout{
                Layout.minimumHeight: 30
                Layout.maximumHeight: 50
                Text {
                    text: qsTr("总共用时: ")
                    color: colorText
                    font.pixelSize: fontSizeMedium
                    font.family: "微软雅黑"
                    font.weight: Font.DemiBold
                }

                Text {
                    id: text_consume_time
                    text: qsTr("3'22''")
                    color: colorText
                    font.pixelSize: fontSizeMedium
                    font.family: "微软雅黑"
                    font.weight: Font.DemiBold
                }
            }

//            RowLayout{
//                Text {
//                    text: qsTr("乘客平均等待时间: 1min 10s")
//                    color: colorText
//                    font.pixelSize: fontSizeMedium
//                    font.family: "微软雅黑"
//                    font.weight: Font.DemiBold
//                }
//            }

            Rectangle{
                //Layout.fillWidth: true
                Layout.preferredWidth: 300
                Layout.maximumHeight: 1
                Layout.minimumHeight: 1
                color: Qt.rgba(1,1,1,0.1)
                //Layout.leftMargin: 50
                //Layout.rightMargin: 100
            }

            RowLayout{
                Text {
                    text: qsTr("电梯耗能情况:")
                    color: colorText
                    font.pixelSize: fontSizeMedium
                    font.family: "微软雅黑"
                    font.weight: Font.DemiBold
                }
            }

            Rectangle{
                //Layout.fillWidth: true
                Layout.preferredWidth: 300
                Layout.maximumHeight: 1
                Layout.minimumHeight: 1
                color: Qt.rgba(1,1,1,0.1)
                //Layout.leftMargin: 50
                //Layout.rightMargin: 100
            }

            ColumnLayout{
                Layout.fillHeight: true
                //Layout.minimumHeight: 500
                Layout.preferredWidth: 300
                Layout.minimumWidth: 200
                ListView {
                    id: list_el
                    clip: true
                    Layout.alignment: Qt.AlignTop
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    model: listmodel_el

                    delegate: ItemDelegate {
                        contentItem:ColumnLayout{
                            Text {
                                text: if(index < 10) qsTr("电梯0" + index); else  qsTr("电梯" + index);
                                color: colorText
                                font.pixelSize: fontSizeMedium
                                font.family: "微软雅黑"
                                font.weight: Font.DemiBold
                            }

                            RowLayout{
                                spacing: 5

                                Text {
                                    text: qsTr("消耗能源：" + consume)
                                    color: colorSubText
                                    font.pixelSize: fontSizeSmall
                                    font.family: "微软雅黑"
                                    font.weight: Font.DemiBold
                                }

                                ProgressBar {
                                    id: progress_control
                                    width: 100
                                    value: share
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 5
                                    background: Rectangle {
                                        implicitWidth: progress_control.width
                                        implicitHeight: progress_control.height
                                        color: "transparent"
                                        radius: 3
                                    }

                                    contentItem: Item {
                                        Rectangle {
                                            width: progress_control.visualPosition * progress_control.width
                                            height: progress_control.height
                                            radius: 2
                                            color: subPrimary1
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                ListModel{
                    id: listmodel_el

                    ListElement{
                        consume: 100
                        share: 0.2
                    }
                }
            }
        }
    }
}
