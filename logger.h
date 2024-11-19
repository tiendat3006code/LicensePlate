#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QDebug>
#include <QLoggingCategory>
#include <QMessageLogger>

class Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = nullptr);

    static QLoggingCategory camera;
    static QLoggingCategory config;
    static QLoggingCategory database;
    static QLoggingCategory imageProcessing;
    static QLoggingCategory serial;

    static void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static void initLog();

signals:
};

#endif // LOGGER_H
