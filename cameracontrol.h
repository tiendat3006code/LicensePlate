#ifndef CAMERACONTROL_H
#define CAMERACONTROL_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QProcess>
#include <QDateTime>
#include <QStringList>
#include "logger.h"

class CameraControl : public QObject
{
    Q_OBJECT
public:
    explicit CameraControl(QObject *parent = nullptr);

public slots:
    void takePicture(int camPos);

signals:
    void startProcessing(const QString& imageName);
    void controlReceivedData(bool isReceived);

private:
    QProcess m_ps;
};

#endif // CAMERACONTROL_H
