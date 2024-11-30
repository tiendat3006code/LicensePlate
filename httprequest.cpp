#include "httprequest.h"
#include "qjsonobject.h"

HttpRequest::HttpRequest(QObject *parent)
    : QObject{parent}, m_reply{nullptr}, m_state{false}
{
    m_baseUrl.setScheme("https");
    m_baseUrl.setHost("ty2.itmf.com.vn");
    m_baseUrl.setPath("/api/verify");
    m_request.setUrl(m_baseUrl);
    m_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
}

void HttpRequest::startRequest(const QString &name)
{
    QJsonObject jsonObject;
    jsonObject["license"] = name;
    if(!m_state)
        jsonObject["state"] = "In";
    else{
        jsonObject["state"] = "Out";
    }
    QJsonDocument jsonDoc(jsonObject);
    m_reply = m_networkManager.post(m_request, jsonDoc.toJson());
    connect(m_reply, &QNetworkReply::readyRead, this, &HttpRequest::handleRequestRead, Qt::QueuedConnection);
    connect(m_reply, &QNetworkReply::finished, this, &HttpRequest::handleRequestFinished, Qt::QueuedConnection);
}

void HttpRequest::setState(bool state)
{
    m_state = state;
    if(state == false){
        qInfo(Logger::network) << "IN state";
    }
    else{
        qInfo(Logger::network) << "OUT state";
    }
}

void HttpRequest::handleReplyFromServer(const QByteArray &data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    int id = obj["id"].toInt();
    if(id == -1){
        emit sendPlateToArduino("Invalid");
        return;
    }
    QString name = obj["name"].toString();
    QString className = obj["class_name"].toString();
    QString plate = obj["license_place"].toString();
    QString state = obj["state"].toString();
    qInfo(Logger::network) << "Name: "<<name;
    qInfo(Logger::network) << "Class: "<<className;
    qInfo(Logger::network) << "Plate: "<<plate;
    if(state == "In"){
        emit sendPlateToArduino(plate + "00");
    }
    else if(state == "Out"){
        emit sendPlateToArduino(plate + "11");
    }
    emit sendNameToDisplay(name);
    emit sendClassToDisplay(className);
}

void HttpRequest::handleRequestRead()
{
    QByteArray dataFromServer = m_reply->readAll();
    qInfo(Logger::network) << "Data from server: "<< dataFromServer;
    this->handleReplyFromServer(dataFromServer);
}

void HttpRequest::handleRequestFinished()
{
    if(m_reply){
        m_reply->deleteLater();
        m_reply = nullptr;
    }
    qInfo(Logger::network) << "Request finished";
    emit this->controlReceivedData(true);
}
