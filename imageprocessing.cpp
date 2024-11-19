#include "imageprocessing.h"

ImageProcessing::ImageProcessing(QObject *parent)
    : QObject{parent}, m_command{"python3"}
{}

void ImageProcessing::startImgeProcessing(const QString &pictureName)
{
    QProcess ps;
    ps.setWorkingDirectory("/home/pi/License-Plate-Recognition");
    qInfo(Logger::imageProcessing) << "Picture name: "<<pictureName;
    QStringList param;
    param << "printLicensePlate.py" << "-i" << "/home/pi/License-Plate-Recognition/License_Plate_Picture/" + pictureName;
    ps.start(m_command, param);
    if(!ps.waitForFinished()){
        qCritical(Logger::imageProcessing) << "Error: "<< ps.readAllStandardError();
        // return;
    }
    QString output = ps.readAllStandardOutput();
    QString plate = output.mid(0, output.length() - 1);
    qInfo(Logger::imageProcessing) << "Plate: " << plate;
    if(plate == "0000" || plate.isEmpty() || plate.isNull()){
        qWarning(Logger::imageProcessing) << "No license plate detected";
        emit controlReceivedData(true);
        emit sendPlateToArduino("Invalid");
        return;
    }
    emit queryPlateFromDB(plate);
}
