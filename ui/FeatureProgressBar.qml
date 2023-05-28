import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQml 2.12
//import Material 0.1 as Material
//import QtQuick.Controls.Styles.Material 0.1 as MaterialStyle

// draws two arcs (portion of a circle)
// fills the circle with a lighter secondary color
// when pressed

Canvas {
    id: canvas
    width: 45
    height: 45
    antialiasing: true

    property color primaryColor: Qt.rgba(173/255,216/255,230/255,0.2)
    property color secondaryColor: Qt.rgba(128/255,203/255,196/255,1)
    readonly property color colorTitle: Qt.rgba(1,1,1,1)
    readonly property color colorText: Qt.rgba(1,1,1,0.8)
    readonly property color colorSubText: Qt.rgba(1,1,1,0.6)
    readonly property int fontSizeExtraSmall: Qt.application.font.pixelSize * 0.6
    readonly property int fontSizeSmall: Qt.application.font.pixelSize * 0.8
    readonly property int fontSizeMedium: Qt.application.font.pixelSize * 1
    readonly property int fontSizeLarge: Qt.application.font.pixelSize * 1.5
    readonly property int fontSizeExtraLarge: Qt.application.font.pixelSize * 2

    property real centerWidth: width / 2
    property real centerHeight: height / 2
    //property real radius: Math.min(canvas.width, canvas.height) / 2
    property real radius: Math.min(canvas.width-10, canvas.height-10) / 2

    property real minimumValue: 0
    property real maximumValue: 100
    property real currentValue: 0
    /// 对应了完成的乘客数量
    property int finishedNum: 0
    property int totalNum: 1000

    // this is the angle that splits the circle in two arcs
    // first arc is drawn from 0 radians to angle radians
    // second arc is angle radians to 2*PI radians
    property real angle: (currentValue - minimumValue) / (maximumValue - minimumValue) * 2 * Math.PI

    // we want both circle to start / end at 12 o'clock
    // without this offset we would start / end at 9 o'clock
    property real angleOffset: -Math.PI / 2

    signal clicked()
    onPrimaryColorChanged: requestPaint()
    onSecondaryColorChanged: requestPaint()
    onMinimumValueChanged: requestPaint()
    onMaximumValueChanged: requestPaint()
    onCurrentValueChanged: requestPaint()
    onPaint: {
        var ctx = getContext("2d");
        ctx.save();

        ctx.clearRect(0, 0, canvas.width, canvas.height);

        // fills the mouse area when pressed
        // the fill color is a lighter version of the
        // secondary color

        if (mouseArea.pressed) {
            ctx.beginPath();
            ctx.lineWidth = 5;
//            ctx.fillStyle = Qt.lighter(canvas.secondaryColor, 1.25);
            ctx.fillStyle = Qt.darker(canvas.secondaryColor, 1.25);
            ctx.arc(canvas.centerWidth,
                    canvas.centerHeight,
                    canvas.radius,
                    0,
                    2*Math.PI);
            ctx.fill();

            timer.running = true;
        }

        // First, thinner arc
        // From angle to 2*PI

        ctx.beginPath();
        ctx.lineWidth = 5;
        ctx.strokeStyle = primaryColor;
        ctx.arc(canvas.centerWidth,
                canvas.centerHeight,
                canvas.radius,
                angleOffset + canvas.angle,
                angleOffset + 2*Math.PI);
        ctx.stroke();


        // Second, thicker arc
        // From 0 to angle

        ctx.beginPath();
        ctx.lineWidth = 5;
        ctx.strokeStyle = canvas.secondaryColor;
        ctx.arc(canvas.centerWidth,
                canvas.centerHeight,
                canvas.radius,
                canvas.angleOffset,
                canvas.angleOffset + canvas.angle);
        ctx.stroke();

        ctx.restore();
    }

    onFinishedNumChanged: {
        currentValue = 100 * finishedNum/totalNum
    }

    Text {
        id: txt_progress
        anchors.centerIn: parent
        text: if(finishedNum < 10) "0" + finishedNum.toString(); else  finishedNum.toString();
        font.pixelSize: fontSizeSmall
        color: colorSubText
    }

    MouseArea {
        id: mouseArea

        anchors.fill: parent
        onClicked: canvas.clicked()
        onPressedChanged: canvas.requestPaint()
    }


//    Timer{
//        id: timer
//        interval: 300;
//        running: false;
//        repeat: true;
//        onTriggered: {
//            if(currentValue === 100) {
//                currentValue = 0
//                txt_progress.text = "0"
//            }
//            currentValue += 1
//            txt_progress.text = qsTr(currentValue.toString())
//        }

//    }
}
