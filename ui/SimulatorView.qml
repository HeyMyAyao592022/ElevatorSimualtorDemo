import QtQml 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Imagine 2.12
import QtQuick.Window 2.0
import QtQuick 2.0
import QtQuick 2.2 as QQ2
import QtGraphicalEffects 1.12
// 自定义 qml
import com.ui.model 1.0
// 3d
import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0
import QtCanvas3D 1.0
import QtQuick.Scene3D 2.0
// 自定义 script
import "qrc:///elevator_view.js" as GLCode

Rectangle {
    id: simualtorRec
    color: "transparent"

    function startSimulator(){
        viewModel.emitStartSignal()
    }

    // 切换相机视角
    function updateCameraModel(model, index){
        GLCode.updataCamera(model, index)
    }

    RowLayout{
        anchors.fill: parent

        // 左边的选择按钮
        ColumnLayout {
            id: leftTabBar
            spacing: 5
            Layout.maximumWidth: 135
            Layout.minimumWidth: 100
            Layout.fillHeight: true

            Row{
                id:start_controls
                spacing: 0
                Layout.alignment: Qt.AlignTop
                Layout.leftMargin: 0
                FeatureIconButton {
                    id: btn_start
                    img_src: "qrc:/icons/icons/start.png"
                    onClickedActive:{
                        startSimulator()
                    }
                }
                FeatureIconButton {
                    id: btn_refresh
                    img_src: "qrc:/icons/icons/refresh.png"
                    onClickedActive: {
                        GLCode.updataCamera(2,0)
                    }
                }
                FeatureIconButton {
                    id: btn_stop
                    img_src: "qrc:/icons/icons/stop.png"
                    onClickedActive: {
                        GLCode.updataCamera(2,1)
                    }
                }
                FeatureIconButton {
                    id: btn_end
                    img_src: "qrc:/icons/icons/end.png"
                    onClickedActive: {
                        GLCode.updataCamera(2,2)
                    }
                }
            }

            ListView {
                clip: true
                Layout.alignment: Qt.AlignTop
                Layout.fillHeight: true
                Layout.fillWidth: true
                ScrollIndicator.vertical: ScrollIndicator {
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                }
                model: viewModel.objectModel()

                delegate: ItemDelegate {
                    width: parent.width
                    height: 68
                    contentItem:
                        PassengerInfo{
                        upward: model.modelData.from < model.modelData.to
                        str_name: model.modelData.name
                        str_from: model.modelData.from
                        str_to: model.modelData.to
                        el_id: model.modelData.elID
                    }
                }
            }
        }

        // 中间的电梯
        Rectangle {
            id: scene
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "transparent"

            Canvas3D {
                id: canvas3d
                anchors.fill: parent
                onInitializeGL: {
                    const elNum = viewModel.elNum
                    const elSpeed = viewModel.elevatorInterval
                    GLCode.iniView(elNum, elSpeed)
                    GLCode.initializeGL(canvas3d);
                }
                onPaintGL: {
                    GLCode.paintGL(canvas3d);
                }
                onResizeGL: {
                    GLCode.resizeGL(canvas3d);
                }
            }

            Component.onCompleted: {
                const elNum = viewModel.elNum
                // 绑定电梯信号
                for(let i = 0; i < elNum; i+=1){
                    // 电梯移动
                    elevators[i].approachNew.connect(function(newFloor, id){
                        GLCode.updateElState(id,newFloor)
                    })
                    // 绑定乘客
                    elevators[i].connectPassenger.connect(function(psgID, elID){
                        //console.log("bind psg" + psgID + " to el" + elID)
                        GLCode.connectPassenger(psgID, elID)
                    })
                    // 乘客进入
                    elevators[i].loadPassengerSignal.connect(function(psgID,elID,time){
                        GLCode.loadPassenger(psgID,elID,time)
                    })
                    // 乘客退出
                    elevators[i].passengerExitSignal.connect(function(elID, psgID, time){
                        GLCode.unLoadPassenger(elID , psgID, time)
                    })
                }
                // 绑定生成乘客的信号
                viewModel.generateNewPassenger.connect(function(id,from,to){
                    GLCode.generateNewPassenger(id,from,to)
                })
            }
        }

        /// 右边的电梯信息
        ColumnLayout {
            id: rightBar
            Layout.fillWidth: true
            Layout.maximumWidth: 175
            Layout.minimumWidth: 120
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignRight

            RowLayout{
                id: finish_layout
                Layout.fillWidth: true
                Layout.leftMargin: 20
                spacing: 10

                Column{
                    Layout.alignment: Qt.AlignTop
                    Text {
                        text: qsTr("已完成：")
                        color: colorSubText
                        font.pixelSize: fontSizeSmall
                        font.family: "微软雅黑"
                    }
                    Text {
                        id: text_finish
                        text: qsTr(viewModel.finishedNumber.toString() + "/" + viewModel.passengerNumber.toString())
                        color: colorText
                        font.pixelSize: Qt.application.font.pixelSize * 1.2
                        font.family: "Consolas"
                    }
                }

                Column{
                    Layout.alignment: Qt.AlignTop
                    RowLayout{
                        Image {
                            id: icon
                            width: 16
                            height: 16
                            source: "qrc:/icons/icons/clock-light.png"
                            fillMode: Image.PreserveAspectFit
                            clip: true
                        }
                        Text {
                            text: qsTr("5min")
                            color: colorSubText
                            font.pixelSize: fontSizeSmall
                            font.family: "微软雅黑"
                        }
                    }

                    Text {
                        id: text_time
                        text: qsTr("3:21")
                        color: colorText
                        font.pixelSize: Qt.application.font.pixelSize * 1.2
                        font.family: "Consolas"
                    }
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                ListView {
                    id: elevatorListView
                    clip: true
                    anchors.fill: parent
                    ScrollIndicator.vertical: ScrollIndicator {
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.rightMargin: 1
                        anchors.bottom: parent.bottom
                        anchors.topMargin: 0
                    }
                    model: elevators
                    property int itemIndex: 0
                    delegate: ItemDelegate {
                        id: infoDelegate
                        width: parent.width
                        height: 120
                        Layout.fillHeight: true
                        anchors.topMargin: 0

                        contentItem:ElevatorInfo{
                            el_name: model.id
                            el_type: model.type
                            passenger_num: model.passengerNumber
                            currentFloor: model.currentFloor
                            movingDirection: model.direction
                            movingState: model.movingState
                            plan_maxfloor:model.planMaxFloor
                            plan_minfloor:model.planMinFloor
                            maxNum: viewModel.elMaxNum
                            describe: model.describe

                            Component.onCompleted: {
//                                if(elevatorListView.itemIndex < 10)
//                                {
//                                    elevators[elevatorListView.itemIndex].approachNew.connect(elevatorApproachingNewFloor)
//                                    elevatorListView.itemIndex += 1
//                                }
                            }
                        }
                    }
                }

            }

        }
    }
}

