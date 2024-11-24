#include "imageprocessing.h"

ImageProcessing::ImageProcessing(QObject *parent)
    : QObject{parent}, m_command{"python3"}, m_imageProcess{new QThread(parent)}
{
    this->setParent(nullptr);
    connect(m_imageProcess, &QThread::started, this, &ImageProcessing::startImgeProcessing, Qt::QueuedConnection);
    connect(m_imageProcess, &QThread::finished, this, [](){
            qInfo(Logger::imageProcessing) << "Image processing finished";
        }, Qt::QueuedConnection);
}

void ImageProcessing::setImagePath(const QString &path)
{
    qInfo(Logger::imageProcessing) << "Received image from: " << sender();
    m_imagePath = path;
    this->runImageProcess();
}

void ImageProcessing::runImageProcess()
{
    if(!m_imageProcess->isRunning()){
        this->moveToThread(m_imageProcess);
        m_imageProcess->start();
        m_imageProcess->setPriority(QThread::HighPriority);
        qInfo(Logger::imageProcessing) << "Start processing";
    }
    else{
        qInfo(Logger::imageProcessing) << "This thread is still running";
    }
}

void ImageProcessing::startImgeProcessing()
{
    QProcess ps;
    ps.setWorkingDirectory("/home/pi/License-Plate-Recognition");
    qInfo(Logger::imageProcessing) << "Picture name: "<<m_imagePath;
    QStringList param;
    param << "printLicensePlate.py" << "-i" << "/home/pi/License-Plate-Recognition/License_Plate_Picture/" + m_imagePath;
    ps.start(m_command, param);
    if(!ps.waitForFinished()){
        qCritical(Logger::imageProcessing) << "Error: "<< ps.readAllStandardError();
        // return;
    }
    QString plate = ps.readAllStandardOutput();
    plate = this->textProcessing(plate);
    qInfo(Logger::imageProcessing) << "Plate: "<<plate;
    if(plate == "0000" || plate.isEmpty() || plate.isNull()){
        qWarning(Logger::imageProcessing) << "No license plate detected";
        emit controlReceivedData(true);
        emit sendPlateToArduino("Invalid");
        m_imageProcess->quit();
        return;
    }
    emit queryPlateFromDB(plate);
    m_imageProcess->quit();
}

QString ImageProcessing::textProcessing(QString &plate)
{
    plate = plate.mid(0, plate.length() - 1);
    if(plate.contains("\n")){
        QStringList list = plate.split("\n");
        plate = list.last();
        return plate;
    }
    return plate;
}


