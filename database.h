#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QPluginLoader>
#include <QDebug>
#include <QString>
#include <QDateTime>
#include <exception>
#include "logger.h"

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    ~Database();
    bool connectToDB();

public slots:
    void queryLicensePlate(const QString& plate);

signals:
    void controlReceivedData(bool isReceived);
    void sendPlateToArduino(const QString& plate);
    void sendNameToDisplay(const QString& name);
    void sendTimeToDisplay(const QString& time);

private:
    QSqlDatabase m_db;

    bool checkPlugin();
    void appendTimeIn(const QString& plate);
    void appendTimeOut(const QString& plate);
    bool checkState(const QString& plate);
    void changeState(const QString& plate, bool state);
};

#endif // DATABASE_H
