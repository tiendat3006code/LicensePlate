#include "surfacemanager.h"

SurfaceManager::SurfaceManager(QObject *parent)
    : QObject{parent}
{}

QString SurfaceManager::imagePathIn() const
{
    return m_imagePathIn;
}

void SurfaceManager::setImagePathIn(const QString &newImagePathIn)
{
    m_imagePathIn = newImagePathIn;
    emit imagePathInChanged();
}

QString SurfaceManager::imagePathOut() const
{
    return m_imagePathOut;
}

void SurfaceManager::setImagePathOut(const QString &newImagePathOut)
{
    m_imagePathOut = newImagePathOut;
    emit imagePathOutChanged();
}

QString SurfaceManager::plateNumber() const
{
    return m_plateNumber;
}

void SurfaceManager::setPlateNumber(const QString &newPlateNumber)
{
    m_plateNumber = newPlateNumber;
    emit plateNumberChanged();
}

void SurfaceManager::handleDisplayImg(const QString &name, bool in_out)
{
    if(in_out){
        qInfo(Logger::surface)<< "Received image from: "<<sender();
        qInfo(Logger::surface) << "Image name: " << name;
        qInfo(Logger::surface) << "State: out";;
        this->setImagePathIn(name);
    }
    else{
        qInfo(Logger::surface)<< "Received image from: "<<sender();
        qInfo(Logger::surface) << "Image name: " << name;
        qInfo(Logger::surface) << "State: in";;
        this->setImagePathOut(name);
    }
}

QString SurfaceManager::name() const
{
    return m_name;
}

void SurfaceManager::setName(const QString &newName)
{
    m_name = newName;
    emit nameChanged();
}

QString SurfaceManager::time() const
{
    return m_time;
}

void SurfaceManager::setTime(const QString &newTime)
{
    m_time = newTime;
    emit timeChanged();
}
