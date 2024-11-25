#include "database.h"

Database::Database(QObject *parent)
    : QObject{parent}, m_db{QSqlDatabase::addDatabase("QSQLITE")}
{
    m_db.setDatabaseName("../database/data.sqlite");
}

Database::~Database()
{
    m_db.close();
}

bool Database::connectToDB()
{
    if(!this->checkPlugin()){
        return false;
    }
    if(!m_db.open()){
        qWarning(Logger::database)<<"Fail to connect to DB";
        qWarning(Logger::database)<<m_db.lastError().text();
        return false;
    }
    qInfo(Logger::database)<<"Connect to DB success";
    return true;
}

void Database::queryLicensePlate(const QString &plate)
{
    QString command = "SELECT Name FROM dbTY_Sheet1 WHERE Plate = '" + plate + "';";
    QSqlQuery sqlQuery;
    if(!sqlQuery.exec(command)){
        qWarning(Logger::database) << "Error from database: "<< m_db.lastError().text();
        qWarning(Logger::database) << "Error from query: "<< sqlQuery.lastError().text();
        emit controlReceivedData(true);
        return;
    }
    if (sqlQuery.next()) {
        QString name = sqlQuery.value(0).toString();
        qInfo(Logger::database) << "Name: " << name;
        emit sendNameToDisplay(name);
        if(!this->checkState(plate)){
            this->appendTimeIn(plate);
            this->changeState(plate, true);
            emit sendPlateToArduino(plate);
            emit startRequest(name, true);
        }
        else{
            this->appendTimeOut(plate);
            this->changeState(plate, false);
            emit sendPlateToArduino(plate);
            emit startRequest(name, false);
        }
    }
    else{
        qWarning(Logger::database) << "No data in database";
        emit sendPlateToArduino("Invalid");
    }
    emit controlReceivedData(true);
}

void Database::appendTimeIn(const QString &plate)
{
    QSqlQuery sqlQuery;
    sqlQuery.prepare("UPDATE dbTY_Sheet1 SET TimeIn = :timeIn WHERE Plate = :plate");
    sqlQuery.bindValue(":timeIn", QDateTime::currentDateTime().toString());
    sqlQuery.bindValue(":plate", plate);
    if(!sqlQuery.exec()){
        qWarning(Logger::database) << "Error from database: "<< m_db.lastError().text();
        qWarning(Logger::database) << "Error from query: "<< sqlQuery.lastError().text();
        return;
    }
    qInfo(Logger::database) << "Append TimeIn successfully";
    emit sendTimeToDisplay(QDateTime::currentDateTime().toString());
}

void Database::appendTimeOut(const QString &plate)
{
    QSqlQuery sqlQuery;
    sqlQuery.prepare("UPDATE dbTY_Sheet1 SET TimeOut = :timeIn WHERE Plate = :plate");
    sqlQuery.bindValue(":timeIn", QDateTime::currentDateTime().toString());
    sqlQuery.bindValue(":plate", plate);
    if(!sqlQuery.exec()){
        qWarning(Logger::database) << "Error from database: "<< m_db.lastError().text();
        qWarning(Logger::database) << "Error from query: "<< sqlQuery.lastError().text();
        return;
    }
    qInfo(Logger::database) << "Append TimeOut successfully";
    emit sendTimeToDisplay(QDateTime::currentDateTime().toString());
}

bool Database::checkState(const QString &plate)
{
    QString command = "SELECT State FROM dbTY_Sheet1 WHERE Plate = '" + plate +"';";
    QSqlQuery sqlQuery;
    if(!sqlQuery.exec(command)){
        qWarning(Logger::database) << "Error from database: "<< m_db.lastError().text();
        qWarning(Logger::database) << "Error from query: "<< sqlQuery.lastError().text();
        return false;
    }
    while(sqlQuery.next()){
        int state = sqlQuery.value(0).toInt();
        if(state == 1)
            return true;
        else if(state == 0)
            return false;
        else
            throw QString("Error: Invalid state from databse");
    }
}

void Database::changeState(const QString &plate, bool state)
{
    QSqlQuery sqlQuery;
    sqlQuery.prepare("UPDATE dbTY_Sheet1 SET State = :state WHERE Plate = :plate");
    if (state) {
        sqlQuery.bindValue(":state", 1);
    }
    else{
        sqlQuery.bindValue(":state", 0);
    }
    sqlQuery.bindValue(":plate", plate);
    if(!sqlQuery.exec()){
        qWarning(Logger::database) << "Error from database: "<< m_db.lastError().text();
        qWarning(Logger::database) << "Error from query: "<< sqlQuery.lastError().text();
    }
    qInfo(Logger::database) << "Update state successfully";
}

bool Database::checkPlugin()
{
    qInfo(Logger::database)<<"Testing plugin";
    QPluginLoader loader("/usr/lib/aarch64-linux-gnu/qt5/plugins/sqldrivers/libqsqlite.so");
    qInfo(Logger::database)<<loader.metaData();
    if(loader.load()){
        qInfo(Logger::database)<<"Loaded the plugin";
        return true;
    }
    qWarning(Logger::database)<<"Error: "<<loader.errorString();
    return false;
}




