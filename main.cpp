#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQuick>
#include <QTextStream>
#include <QtQml>

// Added for reMarkable support
#include <QtPlugin>
#ifdef __arm__
Q_IMPORT_PLUGIN(QsgEpaperPlugin)
#endif
// end reMarkable additions

#include "edit_utils.h"
#include "mytcpserver.h"
#include "keyemitter.h"

int main(int argc, char *argv[])
{
    QTextStream standardOutput(stdout);
    standardOutput << "HEEERE 1"
                   << "\n";
    //  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    // Added for reMarkable support
#ifdef __arm__
    qputenv("QMLSCENE_DEVICE", "epaper");
    qputenv("QT_QPA_PLATFORM", "epaper:enable_fonts");
    qputenv("QT_QPA_EVDEV_TOUCHSCREEN_PARAMETERS", "rotate=180");
    qputenv("QT_QPA_GENERIC_PLUGINS", "evdevtablet");
#endif
    // end reMarkable additions

    QString configDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    qDebug() << "Config dir: " << configDir ;

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<EditUtils>("io.singleton", 1, 0, "EditUtils");

    engine.rootContext()->setContextProperty("screen", app.primaryScreen()->geometry());
    engine.rootContext()->setContextProperty("home_dir", configDir);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

//    QObject *topLevel = engine.rootObjects().value(0);
//    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);

    KeyEmitter keyEmitter;
    engine.rootContext()->setContextProperty("keyEmitter", &keyEmitter);

    // Server server;
    MyTcpServer server;

    QObject::connect(&server, SIGNAL(dataReceived(QVariant)), &keyEmitter, SLOT(keyPressed(QVariant)));

    return app.exec();
}
