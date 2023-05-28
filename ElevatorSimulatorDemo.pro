QT += core gui qml quick quickcontrols2
QT += 3dcore 3drender 3dinput 3dlogic 3dextras 3dquick 3danimation
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \

TRANSLATIONS += \
    ElevatorSimulatorDemo_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    elevatorsimulatordemo.qrc \

#RC_FILE = ico.rc

INCLUDEPATH += ./ui logic

include(./ui/Ui.pri)
include(./logic/Logic.pri)
