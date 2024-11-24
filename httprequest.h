#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QObject>

class HttpRequest : public QObject
{
    Q_OBJECT
public:
    explicit HttpRequest(QObject *parent = nullptr);

signals:
};

#endif // HTTPREQUEST_H
