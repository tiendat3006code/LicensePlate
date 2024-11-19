#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "cameracontrol.h"
#include "database.h"
#include "logger.h"
#include "imageprocessing.h"
#include "communationwitharduino.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);
    qInstallMessageHandler(Logger::customMessageHandler);
    Logger::initLog();

    CameraControl* camera = new CameraControl(&app);
    Database* database = new Database(&app);
    ImageProcessing* image = new ImageProcessing(&app);
    CommunationWithArduino* arduino = new CommunationWithArduino(&app);

    QObject::connect(camera, &CameraControl::startProcessing, image, &ImageProcessing::startImgeProcessing);
    QObject::connect(image, &ImageProcessing::queryPlateFromDB, database, &Database::queryLicensePlate);
    QObject::connect(arduino, &CommunationWithArduino::vehicleDetected, camera, &CameraControl::takePicture);
    QObject::connect(camera, &CameraControl::controlReceivedData, arduino, &CommunationWithArduino::controlHandleReceivedData);
    QObject::connect(database, &Database::controlReceivedData, arduino, &CommunationWithArduino::controlHandleReceivedData);
    QObject::connect(image, &ImageProcessing::controlReceivedData, arduino, &CommunationWithArduino::controlHandleReceivedData);
    QObject::connect(database, &Database::sendPlateToArduino, arduino, &CommunationWithArduino::sendData);
    QObject::connect(image, &ImageProcessing::sendPlateToArduino, arduino, &CommunationWithArduino::sendData);

    database->connectToDB();
    arduino->initCommunication();

    return app.exec();
}
