#include "config.h"

Config::Config(QObject *parent)
    : QObject{parent}, m_config{"../config/setting.ini", QSettings::IniFormat}
{
    switch (m_config.status()) {
    case QSettings::NoError:
        qInfo(Logger::config) << "Setting no error";
        break;
    default:
        qCritical(Logger::config) << "Error";
        break;
    }
}

Config& Config::getInstance(){
    static Config config;
    return config;
}

const QString Config::getSetting(const QString &group, const QString &key) const
{
    if(m_config.allKeys().length() == 0){
        qWarning(Logger::config) << "No settings save";
    }
    if(!m_config.contains(group + "/" + key)){
        qWarning(Logger::config) << key + " not found";
    }
    return m_config.value(group + "/" + key).toString();
}

void Config::setSetting(const QString &group, const QString &key, const QString &value)
{
    qInfo(Logger::config) << "Set " + key + " to " + value;
    m_config.setValue(group + "/" + key, value);
    qInfo(Logger::config) << "Sunc settings";
    m_config.sync();
}


