#include "cameracontrol.h"

CameraControl::CameraControl(QObject *parent)
    : QObject{parent}
{
    m_ps.setWorkingDirectory("/home/pi/License-Plate-Recognition/");
}

void CameraControl::takePicture(int camPos, bool in_out)
{
    emit controlReceivedData(false);
    QString command{"python3"};
    QString pictureName = QDateTime::currentDateTime().toString("dd_MM_yyyy") + "_" + QDateTime::currentDateTime().toString("hh_mm_ss") + ".jpg";
    QStringList param;
    param << "takePicture.py"<<"-i"<<pictureName<<"-c"<<QString::number(camPos);
    m_ps.start(command, param);
    if(!m_ps.waitForFinished()){
        qCritical(Logger::camera)<<"Error: "<< m_ps.readAllStandardError();
        return;
    }
    qInfo(Logger::camera)<<"Output: "<<m_ps.readAllStandardOutput();
    emit displayImage(pictureName, in_out);
    emit setPathToBeginProcessImage(pictureName);
}
