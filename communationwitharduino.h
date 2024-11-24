#ifndef COMMUNATIONWITHARDUINO_H
#define COMMUNATIONWITHARDUINO_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include "logger.h"
#include "config.h"

class CommunationWithArduino : public QObject
{
    Q_OBJECT
public:
    explicit CommunationWithArduino(QObject *parent = nullptr);
    ~CommunationWithArduino();
    void initCommunication();

public slots:
    void controlHandleReceivedData(bool isReceived);
    void sendData(const QString& data);

signals:
    void vehicleDetected(int state, bool in_out);

private:
    QSerialPort* m_port;
    bool m_isReceived;

private slots:
    void handleData();
};

#endif // COMMUNATIONWITHARDUINO_H
