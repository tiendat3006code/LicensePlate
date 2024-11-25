#include "logger.h"
#include "config.h"

QLoggingCategory Logger::camera("Camera");
QLoggingCategory Logger::config("Config");
QLoggingCategory Logger::database("Database");
QLoggingCategory Logger::imageProcessing("Image_processing");
QLoggingCategory Logger::serial("Serial");
QLoggingCategory Logger::surface("Surface");
QLoggingCategory Logger::network("Network");

Logger::Logger(QObject *parent)
    : QObject{parent}
{}

void Logger::customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString category = context.category ? context.category : "Unknown";  // Lấy category từ context

    // Thêm category vào thông điệp log
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "[DEBUG][%s]: %s\n", category.toLocal8Bit().constData(), msg.toLocal8Bit().constData());
        break;
    case QtInfoMsg:
        fprintf(stderr, "[INFO][%s]: %s\n", category.toLocal8Bit().constData(), msg.toLocal8Bit().constData());
        break;
    case QtWarningMsg:
        fprintf(stderr, "[WARNING][%s]: %s\n", category.toLocal8Bit().constData(), msg.toLocal8Bit().constData());
        break;
    case QtCriticalMsg:
        fprintf(stderr, "[CRITICAL][%s]: %s\n", category.toLocal8Bit().constData(), msg.toLocal8Bit().constData());
        break;
    case QtFatalMsg:
        fprintf(stderr, "[FATAL][%s]: %s\n", category.toLocal8Bit().constData(), msg.toLocal8Bit().constData());
        abort();
    }
}

void Logger::initLog()
{
    if(Config::getInstance().getSetting("Log", "info") == "false"){
        Logger::config.setEnabled(QtInfoMsg, false);
        Logger::camera.setEnabled(QtInfoMsg, false);
        Logger::database.setEnabled(QtInfoMsg, false);
        Logger::imageProcessing.setEnabled(QtInfoMsg, false);
        Logger::serial.setEnabled(QtInfoMsg, false);
        Logger::surface.setEnabled(QtInfoMsg, false);
        Logger::network.setEnabled(QtInfoMsg, false);
    }

    if(Config::getInstance().getSetting("Log", "debug") == "false"){
        Logger::config.setEnabled(QtDebugMsg, false);
        Logger::camera.setEnabled(QtDebugMsg, false);
        Logger::database.setEnabled(QtDebugMsg, false);
        Logger::imageProcessing.setEnabled(QtDebugMsg, false);
        Logger::surface.setEnabled(QtDebugMsg, false);
        Logger::network.setEnabled(QtDebugMsg, false);
    }

    if(Config::getInstance().getSetting("Log", "warning") == "false"){
        Logger::config.setEnabled(QtWarningMsg, false);
        Logger::camera.setEnabled(QtWarningMsg, false);
        Logger::database.setEnabled(QtWarningMsg, false);
        Logger::imageProcessing.setEnabled(QtWarningMsg, false);
        Logger::serial.setEnabled(QtWarningMsg, false);
        Logger::surface.setEnabled(QtWarningMsg, false);
        Logger::network.setEnabled(QtWarningMsg, false);
    }

    if(Config::getInstance().getSetting("Log", "critical") == "false"){
        Logger::config.setEnabled(QtCriticalMsg, false);
        Logger::camera.setEnabled(QtCriticalMsg, false);
        Logger::database.setEnabled(QtCriticalMsg, false);
        Logger::imageProcessing.setEnabled(QtCriticalMsg, false);
        Logger::serial.setEnabled(QtCriticalMsg, false);
        Logger::surface.setEnabled(QtCriticalMsg, false);
        Logger::network.setEnabled(QtCriticalMsg, false);
    }
}



