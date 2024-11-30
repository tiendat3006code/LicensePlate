#ifndef SURFACEMANAGER_H
#define SURFACEMANAGER_H

#include <QObject>
#include <QDebug>
#include <QString>
#include "logger.h"

class SurfaceManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString imagePathIn READ imagePathIn WRITE setImagePathIn NOTIFY imagePathInChanged FINAL);
    Q_PROPERTY(QString imagePathOut READ imagePathOut WRITE setImagePathOut NOTIFY imagePathOutChanged FINAL);
    Q_PROPERTY(QString plateNumber READ plateNumber WRITE setPlateNumber NOTIFY plateNumberChanged FINAL);
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QString time READ time WRITE setTime NOTIFY timeChanged FINAL)

public:
    explicit SurfaceManager(QObject *parent = nullptr);

    QString imagePathIn() const;
    void setImagePathIn(const QString &newImagePathIn);

    QString imagePathOut() const;
    void setImagePathOut(const QString &newImagePathOut);

    QString time() const;
    QString plateNumber() const;
    QString name() const;

    Q_INVOKABLE void openBarieInException();
    Q_INVOKABLE void openBarieOutException();

public slots:
    void setPlateNumber(const QString &newPlateNumber);
    void handleDisplayImg(const QString& name, bool in_out);
    void setName(const QString &newName);
    void setTime(const QString &newTime);

signals:
    void imagePathInChanged();
    void imagePathOutChanged();
    void plateNumberChanged();
    void nameChanged();
    void timeChanged();
    void sendOpenBarieException(const QString& data);

private:
    QString m_imagePathIn;
    QString m_imagePathOut;
    QString m_plateNumber;
    QString m_name;
    QString m_time;
};

#endif // SURFACEMANAGER_H
