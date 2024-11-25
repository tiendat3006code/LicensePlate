#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QDebug>
#include <QString>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonDocument>
#include "logger.h"

class HttpRequest : public QObject
{
    Q_OBJECT
public:
    explicit HttpRequest(QObject *parent = nullptr);

public slots:
    void startRequest(const QString& name, bool state);

signals:

private:
    QNetworkReply* m_reply;
    QNetworkAccessManager m_networkManager;
    QNetworkRequest m_request;
    QUrl m_baseUrl;
};

#endif // HTTPREQUEST_H
