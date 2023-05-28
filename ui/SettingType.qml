import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls.Styles 1.2
import QtQuick.Particles 2.0

Item {

    // style
    readonly property color colorBackground: Qt.rgba(41/255,45/255,62/255,1)
    readonly property color colorDivider: Qt.rgba(255/255,255/255,255/255,0.1)
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

    readonly property var colorArray: [subPrimary1, subPrimary2, subPrimary3, subPrimary4, subPrimary5]
    readonly property int colorSize: 5

    // model
    property int elNum: 10
    property int modelNum: 5
    property int floorNum: 40
    property int currModelIndex: 0
    property var floorModels: []
    //property var elModels: []

    function finishSetting(){
        const size = listmodel_type.count
        // 设置类型
        viewModel.setTypesNum(size)
        // 绑定
        let i = 0
        let j = 0
        // 给类型设置每一层楼的运行范围
        for(i = 0; i < modelNum; i++){
            const tempModel = floorModels[i]
            for(j = 1; j <= floorNum; j++){
                if(tempModel[j] === 1){
                    // 设置运行范围
                    viewModel.iniTypes(i, j)
                }
            }
            // 添加描述
            viewModel.setTypeDescribe(i, listmodel_type.get(i).type_decribe)
        }
        // 电梯
        for(i = 0; i < elNum; i++){
            const tIndex = listmodel_el.get(i).type
            // 设置电梯属于哪一种类型
            viewModel.bindElTypes(i, tIndex)
        }
    }

    function setProp(elNum){
        let dis = 10 - elNum
        while(listmodel_el.count > elNum){
            const lastIndex = listmodel_el.count - 1
            listmodel_el.remove(lastIndex)
        }
    }

    ColumnLayout{
        anchors.fill: parent
        spacing: 10

        ColumnLayout{
            Layout.fillWidth: true
            Layout.preferredHeight: 110
            Layout.maximumHeight: 110

            RowLayout{
                Layout.fillWidth: true
                Layout.preferredHeight: 30
                Layout.maximumHeight: 30

                OptionButton{
                    path_normal: "qrc:/icons/icons/setting_gray.png"
                    path_checked: "qrc:/icons/icons/setting.png"
                }

                Text {
                    text: qsTr("设置电梯类型")
                    color: colorText
                    font.pixelSize: fontSizeMedium
                    font.family: "微软雅黑"
                    font.weight: Font.DemiBold
                }
            }

            ListModel{
                id: listmodel_el

                ListElement{
                    elID: 0
                    type: 0
                }

                ListElement{
                    elID: 1
                    type: 0
                }

                ListElement{
                    elID: 2
                    type: 1
                }

                ListElement{
                    elID: 3
                    type: 1
                }

                ListElement{
                    elID: 4
                    type: 2
                }

                ListElement{
                    elID: 5
                    type: 2
                }

                ListElement{
                    elID: 6
                    type: 3
                }

                ListElement{
                    elID: 7
                    type: 3
                }

                ListElement{
                    elID: 8
                    type: 4
                }

                ListElement{
                    elID: 9
                    type: 4
                }
            }

            ListView {
                id: list_el
                clip: true
                Layout.alignment: Qt.AlignTop
                Layout.fillHeight: true
                Layout.fillWidth: true
                orientation:ListView.Horizontal
                model: listmodel_el

                delegate: ItemDelegate {
                    contentItem:ColumnLayout{
                        RowLayout{
                            spacing: 5
                            Text {
                                text: if(index < 10) qsTr("电梯0" + index); else qsTr("电梯" + index)
                                color: colorText
                                font.pixelSize: fontSizeMedium
                                font.family: "微软雅黑"
                                font.weight: Font.DemiBold
                            }

                            Rectangle {
                                id:sign_type
                                Layout.preferredHeight: 8
                                Layout.preferredWidth: 8
                                Layout.alignment: Qt.AlignVCenter
                                color: colorArray[type % colorSize]
                                radius: 5
                                clip: true
                            }

                        }

                        ComboBox{
                            id: currCtrl
                            implicitHeight: 40
                            implicitWidth: 90
                            currentIndex: type
                            model:listmodel_type
                            delegate: ItemDelegate {
                                width: currCtrl.width
                                contentItem: Text {
                                    text: "类型0" + index
                                    color: colorSubText
                                    font.pixelSize: fontSizeSmall
                                    font.family: "微软雅黑"
                                    font.weight: Font.DemiBold
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }
                            contentItem: Text {
                                text: "类型0" + currCtrl.currentIndex
                                font.pixelSize: fontSizeSmall
                                font.family: "微软雅黑"
                                font.weight: Font.DemiBold
                                color: currCtrl.pressed ? primary : colorSubText
                                anchors.leftMargin: 5
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }
                            background: Rectangle {
                                implicitWidth: 90
                                implicitHeight: 40
                                color: Qt.rgba(0,0,0,0.2)
                                radius: 2
                            }
                            popup: Popup {
                                y: currCtrl.height
                                width: currCtrl.width
                                implicitHeight: contentItem.implicitHeight
                                padding: 1
                                contentItem: ListView {
                                    clip: true
                                    implicitHeight: contentHeight
                                    model: currCtrl.popup.visible ? currCtrl.delegateModel : null
                                }
                                background: Rectangle {
                                    color: Qt.rgba(53/255,58/255,81/255,1)
                                    radius: 2
                                }
                            }

                            onCurrentIndexChanged: {
                                listmodel_el.get(index).type = currCtrl.currentIndex
                            }
                        }
                    }
                }
            }
        }

        Rectangle{
            color: colorDivider
            Layout.fillWidth: true
            Layout.maximumHeight: 1
            Layout.minimumHeight: 1
            Layout.leftMargin: 50
            Layout.rightMargin: 50
        }

        RowLayout{
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout{
                Layout.fillHeight: true
                Layout.fillWidth: true

                RowLayout{
                    Layout.fillWidth: true
                    Layout.preferredHeight: 30
                    Layout.maximumHeight: 30

                    OptionButton{
                        path_normal: "qrc:/icons/icons/setting_gray.png"
                        path_checked: "qrc:/icons/icons/setting.png"
                    }

                    Text {
                        text: qsTr("设置电梯规则")
                        color: colorText
                        font.pixelSize: fontSizeMedium
                        font.family: "微软雅黑"
                        font.weight: Font.DemiBold
                    }

                    Text {
                        text: qsTr("+")
                        color: colorText
                        font.pixelSize: fontSizeLarge
                        font.family: "Consolas"
                        font.weight: Font.DemiBold
                        Layout.leftMargin: 20
                    }
                }

                ListModel{
                    id: listmodel_type

                    ListElement{
                        typeID: 0
                        type_decribe: "可到达每一层"
                    }

                    ListElement{
                        typeID: 1
                        type_decribe: "可到达1、25~40层"
                    }

                    ListElement{
                        typeID: 2
                        type_decribe: "可达1~25层"
                    }

                    ListElement{
                        typeID: 3
                        type_decribe: "可达1、2~40层中的偶数层"
                    }

                    ListElement{
                        typeID: 4
                        type_decribe: "可达1~39层中的奇数层"
                    }
                }

                ListView {
                    id: list_type
                    clip: true
                    Layout.alignment: Qt.AlignTop
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    ScrollIndicator.vertical: ScrollIndicator {
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                    }

                    model: listmodel_type

                    delegate: ItemDelegate {
                        width: parent.width
                        height: 150
                        contentItem:ColumnLayout{
                            spacing: 5
                            RowLayout{
                                spacing: 5
                                Text {
                                    text: if(index < 10) qsTr("类型0" + index); else qsTr("类型" + index)
                                    color: colorText
                                    font.pixelSize: fontSizeMedium
                                    font.family: "微软雅黑"
                                    font.weight: Font.DemiBold
                                }

                                Rectangle {
                                    Layout.preferredHeight: 8
                                    Layout.preferredWidth: 8
                                    Layout.alignment: Qt.AlignVCenter
                                    color: colorArray[index % colorSize]
                                    radius: 5
                                    clip: true
                                }
                            }

                            RowLayout{
                                Text {
                                    text: qsTr("设置范围")
                                    color: colorSubText
                                    font.pixelSize: fontSizeSmall
                                    font.family: "微软雅黑"
                                    font.weight: Font.DemiBold
                                }

                                FlatButton{
                                    width: 25
                                    height: 25
                                    img_src: "qrc:/icons/icons/feedback.png"
                                    icon_margin: 5
                                    onClickActive: {
                                        currModelIndex = index
                                        listmodel_floors.changeModelIndex()
                                    }
                                }
                            }

                            RowLayout{
                                Text {
                                    text: qsTr("添加描述")
                                    color: colorSubText
                                    font.pixelSize: fontSizeSmall
                                    font.family: "微软雅黑"
                                    font.weight: Font.DemiBold
                                }

                                FlatButton{
                                    width: 25
                                    height: 25
                                    img_src: "qrc:/icons/icons/msg.png"
                                    icon_margin: 5
                                }

//                                MouseArea{
//                                    Layout.fillWidth: true
//                                    Layout.fillHeight: true
//                                    onClicked: {
//                                        currModelIndex = index
//                                    }
//                                }
                            }

                            Rectangle{
                                Layout.preferredWidth: 200
                                Layout.preferredHeight: 100
                                Layout.maximumHeight: 50
                                radius: 3
                                color: Qt.rgba(0,0,0,0.2)
                                TextInput{
                                    id:input_describe
                                    anchors.fill: parent
                                    clip:true
                                    autoScroll:true
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignVCenter
                                    anchors.leftMargin: 5
                                    anchors.topMargin: 10
                                    font.pixelSize: fontSizeSmall
                                    font.family: "Microsoft YaHei"
                                    wrapMode: TextInput.NoWrap
                                    onTextChanged: {
                                        console.log(input_describe.text)
                                        type_decribe = input_describe.text
                                    }

                                    Text {
                                        anchors.left: parent.left
                                        anchors.leftMargin: 2
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        text: qsTr(type_decribe)
                                        color: colorSubText
                                        font.family: "Microsoft YaHei"
                                        font.pixelSize: fontSizeSmall
                                        visible: (input_describe.text === "" && !input_describe.focus) ? true : false
                                    }
                                }
                            }

                            //                            Text {
                            //                                text: qsTr("点击查看可到达楼层")
                            //                                color: colorSubText
                            //                                font.pixelSize: fontSizeSmall
                            //                                font.family: "微软雅黑"
                            //                                font.weight: Font.DemiBold
                            //                            }

                            Rectangle{
                                color: colorDivider
                                Layout.fillWidth: true
                                Layout.maximumHeight: 1
                                Layout.minimumHeight: 1
                                Layout.leftMargin: 5
                                Layout.rightMargin: 5
                            }
                        }
                    }
                }
            }

            Rectangle{
                color: colorDivider
                Layout.fillHeight: true
                Layout.maximumWidth: 1
                Layout.minimumWidth: 1
                Layout.leftMargin: 400
                Layout.rightMargin: 10
            }

            Rectangle{
                Layout.maximumWidth: 200
                Layout.minimumWidth: 200
                Layout.fillHeight: true
                color:"transparent"

                ColumnLayout{
                    anchors.fill: parent
                    spacing: 5
                    RowLayout{
                        spacing: 5
                        Text {
                            text: if(currModelIndex < 10) qsTr("类型0" + currModelIndex); else qsTr("类型" + currModelIndex)
                            color: colorText
                            font.pixelSize: fontSizeMedium
                            font.family: "微软雅黑"
                            font.weight: Font.DemiBold
                        }

                        Rectangle {
                            Layout.preferredHeight: 8
                            Layout.preferredWidth: 8
                            Layout.alignment: Qt.AlignVCenter
                            color: typeColor(0)
                            radius: 5
                            clip: true
                        }
                    }

                    Text {
                        text: qsTr("设置该类型的运行范围")
                        color: colorSubText
                        font.pixelSize: fontSizeMedium
                        font.family: "微软雅黑"
                        font.weight: Font.DemiBold
                    }

                    ListView {
                        id: list_floors
                        model: listmodel_floors
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        delegate: CheckBox {
                            text: if(index < 9) qsTr("选择楼层0" + (index + 1)); else qsTr("选择楼层" + (index + 1))
                            checked: reachable
                            onCheckedChanged: {
                                const floor = index + 1
                                const curr = floorModels[currModelIndex]
                                if(curr === undefined){
                                    return
                                }
                                if(checked === true){
                                    curr[floor] = 1
                                }
                                else{
                                    curr[floor] = 0
                                }
                                //console.log("type "+currModelIndex + " -> floor " + floor + " :" + curr[floor])
                            }
                        }
                    }

                    ListModel{
                        id: listmodel_floors

                        // 0~39 的 element, 每个都有一个长度为 model num 的数组
                        ListElement{
                            reachable: true
                        }

                        Component.onCompleted: {
                            // 初始化类型
                            iniFloorModels()
                            // 初始化 list model
                            changeModelIndex()
                        }

                        function changeModelIndex(){
                            listmodel_floors.clear()
                            const currModel = floorModels[currModelIndex]
                            for(let i = 1; i <= floorNum; i++){
                                const reach = currModel[i] === 1
                                listmodel_floors.append({reachable: reach})
                            }
                            //list_floors.currentIndex = 0
                        }
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        iniFloorModels()
    }

    function iniFloorModels(){
        // 初始化 4 种类型
        let j = 0
        let k = 0
        for(j = 0; j < modelNum; j++){
            floorModels.push(new Array)
            for(k = 0; k <= floorNum; k++){
                floorModels[j].push(0);
            }
        }

        let i = 0
        let curr = null

        // 能达到则标记为 1

        // 00
        // 1~40
        curr = floorModels[0]
        for(i = 1; i <= floorNum; i++){
            curr[i] = 1;
        }

        // 01
        // 可到达1、25~40层
        curr = floorModels[1]
        curr[1] = 1
        for(i = 25; i <= floorNum; i++){
            curr[i] = 1
        }

        // 02
        // 可达1~25层
        curr = floorModels[2]
        for(i = 1; i <= 25; i++){
            curr[i] = 1
        }

        // 03
        // 可达1、2~40层\n中的偶数层
        curr = floorModels[3]
        curr[1] = 1
        for(i = 2; i <= floorNum; i += 2){
            curr[i] = 1
        }

        // 04
        // 可达1~39层\n中的奇数层
        curr = floorModels[4]
        for(i = 1; i <= floorNum; i += 2){
            curr[i] = 1
        }
    }

    // 颜色依类型而不同
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

    function typeColor(type){
        switch(type){
        case 0:
            return subPrimary1
        case 1:
            return subPrimary2
        case 2:
            return subPrimary3
        case 3:
            return subPrimary4
        case 4:
            return subPrimary5
        default:
            return primary
        }
    }

    function signInfo(type){
        switch(type){
        case 0:
            return "可到达每一层";
        case 1:
            return "可到达1、25~40层";
        case 2:
            return "可达1~25层";
        case 3:
            return "可达1、2~40层\n中的偶数层";
        case 4:
            return "可达1~39层\n中的奇数层";
        }
    }
}
