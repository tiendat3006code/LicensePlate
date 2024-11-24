#include "communationwitharduino.h"

CommunationWithArduino::CommunationWithArduino(QObject *parent)
    : QObject{parent}, m_port{new QSerialPort(parent)}, m_isReceived{true}
{
    connect(m_port, &QSerialPort::readyRead, this, &CommunationWithArduino::handleData);
}

CommunationWithArduino::~CommunationWithArduino()
{
    if(m_port->isOpen()){
        m_port->close();
    }
}

void CommunationWithArduino::initCommunication()
{
    bool isFoundArduino{false};
    QString portLocation{""};
    foreach (const QSerialPortInfo& info, QSerialPortInfo::availablePorts()) {
        qInfo(Logger::serial) << "Port name: " << info.portName();
        qInfo(Logger::serial) << "Port's description: " << info.description();
        if(info.description() == Config::getInstance().getSetting("Common", "port")){
            qInfo(Logger::serial) << "Arduino found";
            isFoundArduino= true;
            portLocation = info.systemLocation();
            break;
        }
    }
    if(!isFoundArduino){
        qCritical(Logger::serial) << "No port was found";
        return;
    }
    m_port->setPortName(portLocation);
    m_port->setBaudRate(QSerialPort::Baud9600);
    m_port->setParity(QSerialPort::NoParity);
    m_port->setDataBits(QSerialPort::Data8);
    m_port->setFlowControl(QSerialPort::NoFlowControl);
    m_port->setStopBits(QSerialPort::OneStop);

    if(!m_port->open(QIODevice::ReadWrite)){
        qCritical(Logger::serial) << "Error: " << m_port->errorString();
        return;
    }
    qInfo(Logger::serial) <<  "Open " + m_port->portName() + " successfully";
}

void CommunationWithArduino::sendData(const QString &data)
{
    if(!m_port->isOpen()){
        qCritical(Logger::serial) << "Port is closed";
        return;
    }
    qInfo(Logger::serial) << "Data send: " << data << " from: " << sender();
    QByteArray dataToSend;
    dataToSend = QString(data).toLatin1();
    qint64 sendStatus = m_port->write(dataToSend);
    if(sendStatus == -1){
        qWarning(Logger::serial)<<"Data sent fail";
    }
    else{
        qInfo(Logger::serial)<<"Data sent successfully";
    }
}

void CommunationWithArduino::controlHandleReceivedData(bool isReceived)
{
    if(!isReceived){
        qInfo(Logger::serial) << "Stop received data from: " << sender();
    }
    else{
        qInfo(Logger::serial) << "Continue received data from: " << sender();
    }
    m_isReceived = isReceived;
}

void CommunationWithArduino::handleData()
{
    if(!m_isReceived){
        qInfo(Logger::serial) << "Stop received data";
        m_port->readAll();
        m_port->flush();
        return;
    }
    QByteArray data = m_port->readAll();
    m_port->flush();
    if(data.isEmpty() || data.isNull() || data.length() != 1){
        qDebug(Logger::serial) << "Data received error";
        return;
    }
    qInfo(Logger::serial) << "Data received: " << QString(data);
    if(data == "1"){
        emit vehicleDetected(Config::getInstance().getSetting("Common", "out_camera").toInt(), true);
    }
    else if(data == "0"){
        emit vehicleDetected(Config::getInstance().getSetting("Common", "in_camera").toInt(), false);
    }
}
