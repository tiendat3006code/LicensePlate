#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QThread>
#include "logger.h"

class ImageProcessing : public QObject
{
    Q_OBJECT
public:
    explicit ImageProcessing(QObject *parent = nullptr);
    void setImagePath(const QString& path);

private slots:
    void runImageProcess();
    void startImgeProcessing();
    QString textProcessing(QString& plate);
    void threadFinished();

signals:
    void queryPlateFromDB(const QString& plate);
    void controlReceivedData(bool isReceived);
    void sendPlateToArduino(const QString& plate);
    void startRequest(const QString& plate);

private:
    QString m_command;
    QThread *m_imageProcess;
    QString m_imagePath;
};

#endif // IMAGEPROCESSING_H
