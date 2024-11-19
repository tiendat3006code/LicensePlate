#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QProcess>
#include "logger.h"

class ImageProcessing : public QObject
{
    Q_OBJECT
public:
    explicit ImageProcessing(QObject *parent = nullptr);
    void startImgeProcessing(const QString& pictureName);

signals:
    void queryPlateFromDB(const QString& plate);
    void controlReceivedData(bool isReceived);
    void sendPlateToArduino(const QString& plate);

private:
    QString m_command;
};

#endif // IMAGEPROCESSING_H
