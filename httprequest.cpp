#include "httprequest.h"
#include "qjsonobject.h"

HttpRequest::HttpRequest(QObject *parent)
    : QObject{parent}, m_reply{nullptr}
{
    m_baseUrl.setScheme("https");
    m_baseUrl.setHost("itmf.com.vn");
    m_baseUrl.setPath("/api/history");
    m_request.setUrl(m_baseUrl);
    m_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    connect(m_reply, &QNetworkReply::readyRead, [this](){
        QByteArray dataFromServer = m_reply->readAll();
        qInfo(Logger::network) << "Data from server: "<< dataFromServer;
    });
    connect(m_reply, &QNetworkReply::finished, [this](){
        if(m_reply){
            m_reply->deleteLater();
            m_reply = nullptr;
        }
        qInfo(Logger::network) << "Request finished";
    });
}

void HttpRequest::startRequest(const QString &name, bool state)
{
    QJsonObject jsonObject;
    jsonObject["name"] = name;
    if(!state)
        jsonObject["state"] = "In";
    else{
        jsonObject["state"] = "Out";
    }
    QJsonDocument jsonDoc(jsonObject);
    m_reply = m_networkManager.post(m_request, jsonDoc.toJson());
}
