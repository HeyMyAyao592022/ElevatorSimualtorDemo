#include "viewmodel.h"
#include "DispatchingSystem.h"
#include "passengerlist.h"
#include "framelesswindow.h"

#include <QUrl>
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQuick/QQuickView>
#include <QGuiApplication>
#include <QSettings>
#include <QIcon>
#include <QObject>
#include <QFont>
#include <QList>
#include <QtCore/QDir>
#include <QtQml/QQmlEngine>
#include <QStyle>
#include <QQuickStyle>
#include <QSharedPointer>

int main(int argc, char *argv[])
{
    QGuiApplication::setApplicationName("ElevatorSimulatorDemo");
    QGuiApplication::setOrganizationName("QtProject");
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QGuiApplication a(argc, argv);

    /// style
    QFont font;
    font.setFamily("Consolas");
    font.setPixelSize(14);
    a.setFont(font);
    QQuickStyle::setStyle("Material");

    /// ����
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "ElevatorSimulatorDemo_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    /// ����ϵͳ
    /// auto dispatchingSystem = new DispatchingSystem(nullptr);
    auto dispatchingSystem = DispatchingSystem::getInstance();

    /// ���� qml view
    QQmlApplicationEngine* engine = dispatchingSystem->getQmlEngine();

    /// �� QML ��ע�� passenger list ��
    qmlRegisterUncreatableType<PassengerList, 1>("com.ui.model", 1, 0,
                                                 "PassengerList",
                                                 "Cannot create ObjectModel");

    /// ע�ᴰ��
    qmlRegisterType<FramelessWindow>("com.ui.window", 1, 0, "FramelessWindow");

    /// �� view model ע�ᵽ qml ��
    auto viewModel = ViewModel::getInstance();
    engine->rootContext()->setContextProperty("viewModel",viewModel.get());

    /// ���� qml
    const QUrl url(QStringLiteral("qrc:/qml/ui/app.qml"));
    /// ����
    engine->load(url);

    if (engine->rootObjects().isEmpty())
        return -1;

    /// �������߳�
    //dispatchingSystem->start();

    return a.exec();
}
