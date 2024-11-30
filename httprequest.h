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
    void startRequest(const QString& name);
    void setState(bool state);

signals:
    void controlReceivedData(bool isReceived);
    void sendPlateToArduino(const QString& plate);
    void sendNameToDisplay(const QString& name);
    void sendClassToDisplay(const QString& className);

private:
    QNetworkReply* m_reply;
    QNetworkAccessManager m_networkManager;
    QNetworkRequest m_request;
    QUrl m_baseUrl;
    bool m_state;

    void handleReplyFromServer(const QByteArray& data);

private slots:
    void handleRequestRead();
    void handleRequestFinished();
};

#endif // HTTPREQUEST_H
