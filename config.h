#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QSettings>
#include <QString>
#include "logger.h"
#include <QStringList>

class Config : public QObject
{
    Q_OBJECT
public:
    Config(Config&) = delete;
    void operator=(const Config&) = delete;
    static Config& getInstance();
    const QString getSetting(const QString& group, const QString& key) const;
    void setSetting(const QString& group, const QString& key, const QString& value);

signals:
private:
    QSettings m_config;

    explicit Config(QObject *parent = nullptr);
    ~Config() = default;
};

#endif // CONFIG_H
