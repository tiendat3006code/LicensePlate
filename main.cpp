#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QCursor>
#include "cameracontrol.h"
#include "database.h"
#include "logger.h"
#include "imageprocessing.h"
#include "communationwitharduino.h"
#include "surfacemanager.h"
#include "httprequest.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    SurfaceManager* manager = new SurfaceManager(&app);
     engine.rootContext()->setContextProperty("surfaceManager", manager);
    // app.setOverrideCursor(QCursor(Qt::BlankCursor));
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
    engine.rootContext()->setContextProperty("Cameratest", camera);
    Database* database = new Database(&app);
    ImageProcessing* image = new ImageProcessing(&app);
    CommunationWithArduino* arduino = new CommunationWithArduino(&app);
    HttpRequest* request = new HttpRequest(&app);

    QObject::connect(camera, &CameraControl::setPathToBeginProcessImage, image, &ImageProcessing::setImagePath, Qt::QueuedConnection);
    QObject::connect(arduino, &CommunationWithArduino::vehicleDetected, camera, &CameraControl::takePicture, Qt::QueuedConnection);
    QObject::connect(camera, &CameraControl::controlReceivedData, arduino, &CommunationWithArduino::controlHandleReceivedData, Qt::QueuedConnection);
    QObject::connect(request, &HttpRequest::controlReceivedData, arduino, &CommunationWithArduino::controlHandleReceivedData, Qt::QueuedConnection);
    QObject::connect(image, &ImageProcessing::controlReceivedData, arduino, &CommunationWithArduino::controlHandleReceivedData, Qt::QueuedConnection);
    QObject::connect(request, &HttpRequest::sendPlateToArduino, arduino, &CommunationWithArduino::sendData, Qt::QueuedConnection);
    QObject::connect(image, &ImageProcessing::sendPlateToArduino, arduino, &CommunationWithArduino::sendData, Qt::QueuedConnection);
    QObject::connect(camera, &CameraControl::displayImage, manager, &SurfaceManager::handleDisplayImg, Qt::QueuedConnection);
    QObject::connect(image, &ImageProcessing::sendPlateToArduino, manager, &SurfaceManager::setPlateNumber, Qt::QueuedConnection);
    QObject::connect(arduino, &CommunationWithArduino::setStateForRequest, request, &HttpRequest::setState, Qt::QueuedConnection);
    QObject::connect(image, &ImageProcessing::startRequest, request, &HttpRequest::startRequest, Qt::QueuedConnection);
    QObject::connect(request, &HttpRequest::sendPlateToArduino, manager, &SurfaceManager::setPlateNumber, Qt::QueuedConnection);
    QObject::connect(request, &HttpRequest::sendNameToDisplay, manager, &SurfaceManager::setName, Qt::QueuedConnection);
    QObject::connect(request, &HttpRequest::sendClassToDisplay, manager, &SurfaceManager::setTime, Qt::QueuedConnection);
    QObject::connect(manager, &SurfaceManager::sendOpenBarieException, arduino, &CommunationWithArduino::sendData, Qt::QueuedConnection);

     // QObject::connect(image, &ImageProcessing::queryPlateFromDB, database, &Database::queryLicensePlate, Qt::QueuedConnection);
    // QObject::connect(database, &Database::startRequest, request, &HttpRequest::startRequest, Qt::QueuedConnection);

    database->connectToDB();
    arduino->initCommunication();

    return app.exec();
}
