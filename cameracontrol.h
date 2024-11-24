#ifndef CAMERACONTROL_H
#define CAMERACONTROL_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QProcess>
#include <QDateTime>
#include <QStringList>
#include <QThread>
#include "logger.h"

class CameraControl : public QObject
{
    Q_OBJECT
public:
    explicit CameraControl(QObject *parent = nullptr);

public slots:
    void takePicture(int camPos, bool in_out);

signals:
    void setPathToBeginProcessImage(const QString& imageName);
    void controlReceivedData(bool isReceived);
    void displayImage(const QString& imgName, bool in_out);

private:
    QProcess m_ps;
};

#endif // CAMERACONTROL_H
