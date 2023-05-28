import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Imagine 2.12
import QtQuick.Window 2.0
import QtGraphicalEffects 1.12

import QtQuick.Controls.Material 2.0
import com.ui.model 1.0
import com.ui.window 1.0

FramelessWindow {
    id: window
    width: 1115
    height: 742
    visible: true
    title: "Elevator Simualator Demo"
    /// 设置样式
    color: "transparent"
    Material.theme: Material.Dark
    Material.accent: Material.Purple
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowMinimizeButtonHint
    modality: Qt.ApplicationModal

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

    //property int bw: 3
    property bool  isMaximized: false

    Shortcut {
        sequence: "Ctrl+Q"
        onActivated: Qt.quit()
    }

    Rectangle{
        id: mainRec
        width: parent.width
        height: parent.height
        color: colorBackground

        ColumnLayout {
            id: mainLayout
            anchors.fill: parent
            spacing: 0
            // 顶栏
            TopBar{}
            // 横向布局
            RowLayout {
                id: mainRowLayout
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 0
                property int checkedRole: 0

                Column{
                    id: button_group_options
                    Layout.fillHeight: true
                    Layout.preferredWidth: 40

                    OptionButton{
                        id: btn_view1
                        role: 0
                        path_normal: "qrc:/icons/icons/setting_gray.png"
                        path_checked: "qrc:/icons/icons/setting.png"
                    }

                    OptionButton{
                        id: btn_view2
                        role: 1
                        path_normal: "qrc:/icons/icons/simulator_gray.png"
                        path_checked: "qrc:/icons/icons/simulator.png"
                    }

                    OptionButton{
                        id: btn_view3
                        role: 2
                        path_normal: "qrc:/icons/icons/monitor_el_gray.png"
                        path_checked: "qrc:/icons/icons/monitor_el.png"
                    }

                    // 让这几个按钮互斥
                    function swiftView(selectedRole){
                        if(selectedRole !== mainRowLayout.checkedRole){
                            switch(mainRowLayout.checkedRole){
                            case 0:
                                btn_view1.checked = false
                                break
                            case 1:
                                btn_view2.checked = false
                                break
                            case 2:
                                btn_view3.checked = false
                                break
                            }

                            switch(selectedRole){
                            case 0:
                                btn_view1.checked = true
                                // 切换到 setting view
                                view_swipe.currentIndex = 0
                                break;
                            case 1:
                                btn_view2.checked = true
                                // 切换到 simualtor view
                                view_swipe.currentIndex = 2
                                break
                            case 2:
                                btn_view3.checked = true
                                // result view
                                view_swipe.currentIndex = 4
                                break
                            }
                            mainRowLayout.checkedRole = selectedRole
                        }
                    }

                    // 连接
                    Component.onCompleted: {
                        btn_view1.checkedActive.connect(button_group_options.swiftView)
                        btn_view2.checkedActive.connect(button_group_options.swiftView)
                        btn_view3.checkedActive.connect(button_group_options.swiftView)
                        // 初始选择
                        btn_view1.checked = true
                    }
                }

                SwipeView{
                    id: view_swipe
                    Layout.alignment: Qt.AlignTop
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    orientation: Qt.Vertical
                    currentIndex: 0
                    interactive: false

                    SettingView{
                        id: view_setting
                    }

                    Rectangle{
                        color: "transparent"
                        height: 50
                    }

                    Loader {
                        id: loader_simulator
                        asynchronous: true
                        signal updateCameraModel(var model, var index)
                        onStatusChanged: {
                            if(loader_simulator.status === Loader.Ready){
                                console.log("finish loading")
                                // 切换相机视角
                                updateCameraModel.connect(loader_simulator.item.updateCameraModel)
                                // 切换
                                button_group_options.swiftView(1)
                            }
                        }
                    }

                    Rectangle{
                        color: "transparent"
                        height: 50
                    }

                    ResultView{
                        id: result_view
                    }

                    // 连接信号槽
                    Connections{
                        target: bottom_bar
                        onCameraBtnClicked:{
                            loader_simulator.updateCameraModel(model, index)
                        }
                    }

                    Connections{
                        target: view_setting
                        onEnterSimulator: {
                            // 完成设置,view model 发出信号,通知系统加载电梯
                            console.log("start ini els")
                            viewModel.emitIniElSignal();
                        }
                    }

                    Component.onCompleted: {
                        // view model 发出信号，则开始加载 simulator view
                        viewModel.finishIniEls.connect(function(){
                            console.log("load view")
                            loader_simulator.source = "SimulatorView.qml"
                        })
                        // 结束
                        viewModel.finishSimulating.connect(function(){
                            main_timer.running = true
                        })
                    }
                }
            }
            // 底栏
            BottomBar {
                id: bottom_bar
            }
        }
    }

    Timer{
        id: main_timer
        interval: 3000
        repeat: false
        onTriggered: {
            finishSimualting()
        }
    }

    function finishSimualting(){
        // 处理数据
        result_view.analayseData()
        // 切换界面
        button_group_options.swiftView(2)
    }

    MouseArea {
        x: 0
        y: 0
        height: 40
        width:parent.width - 120
        acceptedButtons: Qt.LeftButton
        property point clickPos: "0,0"
        onPressed: {
            clickPos  = Qt.point(mouse.x,mouse.y)
        }
        onPositionChanged: {
            var delta = Qt.point(mouse.x-clickPos.x, mouse.y-clickPos.y)
            window.setX(window.x+delta.x)
            window.setY(window.y+delta.y)
        }
    }
}

