// main.cpp (Qt application entry point)
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext> // To expose C++ objects to QML

#include "buttoncontroller.h"

// Define the global flag here for compilation purposes in the Qt side,
// but in a real embedded scenario, this would be linked to the actual
// flag in your C embedded code.
volatile uint8_t buttonPressedFlag = 0;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // Create an instance of your ButtonController
    ButtonController buttonController;

    // Expose the ButtonController object to QML under the name "buttonController"
    engine.rootContext()->setContextProperty("buttonController", &buttonController);

    // Load the QML file
    const QUrl url(QStringLiteral("qrc:/main.qml")); // Assuming main.qml is in qrc
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
