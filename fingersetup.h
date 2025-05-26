#ifndef FINGERSETUP_H
#define FINGERSETUP_H

#include <stdint.h>

#include <QByteArray>
#include <QDebug>
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QString>
#include <QTimer>
#include <QThread>
#include <QEventLoop>

#include "fingerdefinition.h"

class FingerSetup : public QObject {
    Q_OBJECT
    Q_PROPERTY(int fingerID READ fingerID WRITE setFingerID NOTIFY fingerIDChanged FINAL)
    Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY messageChanged FINAL)

public:
    explicit FingerSetup(QObject *parent = nullptr);
    virtual ~FingerSetup();
    enum class FingerSetupCommand {
        Idle,
        PasswordVerify,
        ClearFingerData,
        GetSysParams,
        GetEcho,
        GenImage,
        Img2Tz,
        RegModel,
        Store,
        Search
    };

    enum class FingerCommandReturnStatus{
        IDLE,
        COMMAND_EXECUTION_COMPLETE_STATUS,
        FAIL_TO_EXECUTE_COMMAND_STATUS,
        ERROR_WHEN_RECEIVE_DATA_PACKAGE_STATUS,
        NO_FINGER_ON_THE_SENSOR_STATUS,
        FAIL_TO_ENROLL_THE_FINGER_STATUS,
        FAIL_DUE_TO_OVER_DISORDERLY_IMAGE_STATUS,
        FAIL_DUE_TO_WEAK_OR_SMALL_IMAGE_STATUS,
        FINGER_DOES_NOT_MATCH_STATUS,
        FAIL_TO_FIND_MATCHING_FINGER_STATUS,
        FAIL_TO_COMBINE_CHARACTER_FILES_STATUS,
        PAGEID_OUT_OF_RANGE_STATUS,
        READ_TEMPLATE_ERROR_OR_INVALID_STATUS,
        ERROR_WHEN_UPLOADING_TEMPLATE_STATUS,
        CANNOT_RECEIVE_FOLLOWING_PACKAGES_STATUS,
        ERROR_WHEN_UPLOADING_IMAGE_STATUS,
        FAIL_TO_DELETE_TEMPLATE_STATUS,
        FAIL_TO_CLEAR_FINGER_LIBRARY_STATUS,
        FAIL_DUE_TO_WEAK_PRIMARY_IMAGE_STATUS,
        ERROR_WHEN_WRITING_FLASH_STATUS,
        INVALID_REGISTER_NUMBER_STATUS
    };

    int fingerID() const;
    void setFingerID(int newFingerID);

    QString message() const;
    void setMessage(const QString &newMessage);

private:
    QSerialPort *m_port;
    FingerSetupCommand m_state;
    int m_step;
    FingerCommandReturnStatus m_commandReturnStatus;
    QEventLoop m_loop;
    bool m_isConnected;

    int m_fingerID;

    QString m_message;

private slots:
    void sendData(const QByteArray &data);
    void onReadyRead();
    void onError(QSerialPort::SerialPortError error);
    uint8_t calculateChecksum(const QByteArray &data);
    QByteArray buildPacket(uint8_t pid, const QByteArray &content);
    void dataProcess(const QByteArray &data);
    void handleConfirmation(FingerSetup::FingerCommandReturnStatus status);
    void handleFingerPrint(FingerSetup::FingerCommandReturnStatus status);

signals:
    void confirmationCodeReceived(FingerSetup::FingerCommandReturnStatus status);
    void verifyPasswordSuccess();
    void clearFingerDataSuccess();

    void fingerIDChanged();

    void messageChanged();

public slots:
    void initializePort();
    void passwordVerify();
    void getSysParams();
    void getEcho();
    void closePort();
    void genImage();
    void img2Tz(uint8_t bufferId);
    void regModel();
    void store(uint8_t bufferId, uint16_t pageId);
    void search(uint8_t bufferId, uint16_t startPage, uint16_t pageNum);
    FingerCommandReturnStatus getDataProcStatus() const;
    void enroll();
    void emptyFingerData();
    void fingerPrint();
};

#endif  // FINGERSETUP_H
