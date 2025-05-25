#include "fingersetup.h"

FingerSetup::FingerSetup(QObject *parent)
    : QObject{parent}, m_port{nullptr}, m_state{FingerSetup::FingerSetupCommand::Idle}, m_step{0}, m_commandReturnStatus{FingerSetup::FingerCommandReturnStatus::IDLE},
m_fingerID{-1}, m_isConnected{false}  
{
  m_port = new QSerialPort(this);
  connect(m_port, &QSerialPort::readyRead, this, &FingerSetup::onReadyRead, Qt::QueuedConnection);
  connect(m_port, &QSerialPort::errorOccurred, this, &FingerSetup::onError, Qt::QueuedConnection);
  connect(this, &FingerSetup::fingerIDChanged, &m_loop, &QEventLoop::quit, Qt::QueuedConnection);
}

FingerSetup::~FingerSetup() {
  if (m_port) {
    m_port->close();
    delete m_port;
    m_port = nullptr;
  }
}

void FingerSetup::initializePort() {
  for (const auto &info : QSerialPortInfo::availablePorts()) {
    qInfo() << "Found port:" << info.portName()
            << "Description:" << info.description()
            << "Manufacturer:" << info.manufacturer();
    if (info.description() == PORT_DESC) {
      qInfo() << "Found matching port:" << info.portName();
      m_port->setPortName(info.systemLocation());
      break;
    }
  }
  m_port->setBaudRate(QSerialPort::Baud57600);
  m_port->setDataBits(QSerialPort::Data8);
  m_port->setParity(QSerialPort::NoParity);
  m_port->setStopBits(QSerialPort::OneStop);
  m_port->setFlowControl(QSerialPort::NoFlowControl);
  if (m_port->open(QIODevice::ReadWrite)) {
    qInfo() << "Port opened successfully:" << m_port->portName();
  } else {
    qWarning() << "Failed to open port:" << m_port->errorString();
  }
}
void FingerSetup::sendData(const QByteArray &data) {
  if (data.isNull() || data.isEmpty()) {
    qInfo() << "Data is null or empty, not sending.";
    return;
  }
  if (!m_port->isOpen()) {
    qInfo() << "Port is not open, cannot send data.";
    return;
  }
  m_port->write(data);

  if (m_port->waitForBytesWritten(1000)) {
  } else {
    qInfo() << "Timeout while waiting for data to be written.";
    qInfo() << "Error message:" << m_port->errorString();
  }
  m_commandReturnStatus = FingerSetup::FingerCommandReturnStatus::IDLE;  // Reset status after sending data
}

void FingerSetup::onReadyRead() {
  QByteArray buffer;
  buffer = m_port->readAll();
  if (buffer.isEmpty()) {
    qInfo() << "No data received.";
    return;
  }
  qInfo() << "Data received:" << buffer.toHex();
  this->dataProcess(buffer);
}

void FingerSetup::onError(QSerialPort::SerialPortError error) {
  if (error != QSerialPort::NoError) {
    qWarning() << "Serial port error:" << m_port->errorString();
  }
}

void FingerSetup::passwordVerify() {
  uint32_t password = 0;  // Example password, replace with actual password
  QByteArray content;
  content.append(0x13);  // instruction code
  content.append((password >> 24) & 0xFF);
  content.append((password >> 16) & 0xFF);
  content.append((password >> 8) & 0xFF);
  content.append(password & 0xFF);
  QByteArray packet = buildPacket(0x01, content);

  this->sendData(packet);
  qInfo() << "Password verification data sent.";
  m_state = FingerSetup::FingerSetupCommand::PasswordVerify;
}

uint8_t FingerSetup::calculateChecksum(const QByteArray &data) {
  uint8_t sum = 0;
  for (char byte : data) {
    sum += static_cast<uint8_t>(byte);
  }
  return sum;
}

void FingerSetup::dataProcess(const QByteArray &data) {
  // Convert the data to a string or process it as needed
  uint8_t code = data[9];
  switch (code)  // Assuming code is at index 9
  {
    case COMMAND_EXECUTION_COMPLETE:
      m_commandReturnStatus = FingerSetup::FingerCommandReturnStatus::COMMAND_EXECUTION_COMPLETE_STATUS;
      if (m_state == FingerSetup::FingerSetupCommand::PasswordVerify) {
        emit verifyPasswordSuccess();
      }
      else if(m_state == FingerSetup::FingerSetupCommand::ClearFingerData) {
        emit clearFingerDataSuccess();
      }
      else if(m_state == FingerSetup::FingerSetupCommand::GetSysParams) {
        qInfo() << "System parameters retrieved successfully.";
      }
      else if(m_state == FingerSetup::FingerSetupCommand::GenImage) {
        qInfo() << "Image generation completed.";
      }
      else if(m_state == FingerSetup::FingerSetupCommand::Img2Tz) {
        qInfo() << "Image to template conversion completed.";
      }
      else if(m_state == FingerSetup::FingerSetupCommand::RegModel) {
        qInfo() << "Model registration completed.";
      }
      else if(m_state == FingerSetup::FingerSetupCommand::Store) {
        qInfo() << "Fingerprint data stored successfully.";
      }
      else if(m_state == FingerSetup::FingerSetupCommand::Search) {
        qInfo() << "Fingerprint search completed.";
      }
      break;
    case ERROR_WHEN_RECEIVE_DATA_PACKAGE:
      qInfo() << "Error when receiving data package.";
      m_commandReturnStatus = FingerSetup::FingerCommandReturnStatus::ERROR_WHEN_RECEIVE_DATA_PACKAGE_STATUS;
      break;
    case 0x13:
      qInfo() << "Password verified fails.";
      m_commandReturnStatus = FingerSetup::FingerCommandReturnStatus::FAIL_TO_EXECUTE_COMMAND_STATUS;
      break;
    case NO_FINGER_ON_THE_SENSOR:
      qInfo() << "Put your finger on the sensor.";
      m_commandReturnStatus = FingerSetup::FingerCommandReturnStatus::NO_FINGER_ON_THE_SENSOR_STATUS;
      break;
    case FAIL_TO_ENROLL_THE_FINGER:
      qInfo() << "Failed to enroll the finger.";
      m_commandReturnStatus = FingerSetup::FingerCommandReturnStatus::FAIL_TO_ENROLL_THE_FINGER_STATUS;
      break;
    case FAIL_DUE_TO_OVER_DISORDERLY_IMAGE:
      qInfo() << "Failed due to over disorderly image.";
      m_commandReturnStatus = FingerSetup::FingerCommandReturnStatus::FAIL_DUE_TO_OVER_DISORDERLY_IMAGE_STATUS;
      break;
    case FAIL_DUE_TO_WEAK_OR_SMALL_IMAGE:
      qInfo() << "Failed due to weak or small image.";
      m_commandReturnStatus = FingerSetup::FingerCommandReturnStatus::FAIL_DUE_TO_WEAK_OR_SMALL_IMAGE_STATUS;
      break;
    case FINGER_DOES_NOT_MATCH:
      qInfo() << "Finger does not match.";
      m_commandReturnStatus = FingerSetup::FingerCommandReturnStatus::FINGER_DOES_NOT_MATCH_STATUS;
      break;
    case FAIL_TO_FIND_MATCHING_FINGER:
      qInfo() << "Failed to find matching finger.";
      m_commandReturnStatus = FingerSetup::FingerCommandReturnStatus::FAIL_TO_FIND_MATCHING_FINGER_STATUS;
      break;
    case FAIL_TO_COMBINE_CHARACTER_FILES:
      qInfo() << "Failed to combine character files.";
      m_commandReturnStatus = FingerSetup::FingerCommandReturnStatus::FAIL_TO_COMBINE_CHARACTER_FILES_STATUS;
      break;
    case PAGEID_OUT_OF_RANGE:
      qInfo() << "Page ID out of range.";
      m_commandReturnStatus = FingerSetup::FingerCommandReturnStatus::PAGEID_OUT_OF_RANGE_STATUS;
      break;
    case READ_TEMPLATE_ERROR_OR_INVALID:
      qInfo() << "Read template error or invalid.";
      m_commandReturnStatus = FingerSetup::FingerCommandReturnStatus::READ_TEMPLATE_ERROR_OR_INVALID_STATUS;
      break;
    case ERROR_WHEN_UPLOADING_TEMPLATE:
      qInfo() << "Error when uploading template.";
      m_commandReturnStatus = FingerSetup::FingerCommandReturnStatus::ERROR_WHEN_UPLOADING_TEMPLATE_STATUS;
      break;
    case CANNOT_RECEIVE_FOLLOWING_PACKAGES:
      qInfo() << "Cannot receive following packages.";
      m_commandReturnStatus = FingerSetup::FingerCommandReturnStatus::CANNOT_RECEIVE_FOLLOWING_PACKAGES_STATUS;
      break;
    case ERROR_WHEN_UPLOADING_IMAGE:
      qInfo() << "Error when uploading image.";
      m_commandReturnStatus = FingerSetup::FingerCommandReturnStatus::ERROR_WHEN_UPLOADING_IMAGE_STATUS;
      break;
    case FAIL_TO_DELETE_TEMPLATE:
      qInfo() << "Failed to delete template.";
      m_commandReturnStatus = FingerSetup::FingerCommandReturnStatus::FAIL_TO_DELETE_TEMPLATE_STATUS;
      break;
    case FAIL_TO_CLEAR_FINGER_LIBRARY:
      qInfo() << "Failed to clear finger library.";
      m_commandReturnStatus = FingerSetup::FingerCommandReturnStatus::FAIL_TO_CLEAR_FINGER_LIBRARY_STATUS;
      break;
    case FAIL_DUE_TO_WEAK_PRIMARY_IMAGE:
      qInfo() << "Failed due to weak primary image.";
      m_commandReturnStatus = FingerSetup::FingerCommandReturnStatus::FAIL_DUE_TO_WEAK_PRIMARY_IMAGE_STATUS;
      break;
    case ERROR_WHEN_WRITING_FLASH:
      qInfo() << "Error when writing flash.";
      m_commandReturnStatus = FingerSetup::FingerCommandReturnStatus::ERROR_WHEN_WRITING_FLASH_STATUS;
      break;
    case INVALID_REGISTER_NUMBER:
      qInfo() << "Invalid register number.";
      m_commandReturnStatus = FingerSetup::FingerCommandReturnStatus::INVALID_REGISTER_NUMBER_STATUS;
      break;
    default:
      break;
  }
  emit confirmationCodeReceived(m_commandReturnStatus);
}

void FingerSetup::getSysParams() {
  QByteArray dataToSend(16, '\0');
  dataToSend[0] = FIRST_HEADER;
  dataToSend[1] = SECOND_HEADER;

  dataToSend[2] = DEFAULT_ADDRESS;
  dataToSend[3] = DEFAULT_ADDRESS;
  dataToSend[4] = DEFAULT_ADDRESS;
  dataToSend[5] = DEFAULT_ADDRESS;

  dataToSend[6] = COMMAND_PACKAGE;

  dataToSend[7] = 0x00;
  dataToSend[8] = 0x03;

  dataToSend[9] = 0x0F;  // Command for getting system parameters

  uint8_t sum    = this->calculateChecksum(dataToSend);
  dataToSend[10] = (sum >> 8) & 0xFF;
  dataToSend[11] = sum & 0xFF;  // calculateChecksum or other data as needed
  this->sendData(dataToSend);
  m_state = FingerSetup::FingerSetupCommand::GetSysParams;
}

void FingerSetup::getEcho() {
  QByteArray content;
  content.append(0x53);  // instruction code
  QByteArray packet = buildPacket(0x01, content);
  this->sendData(packet);
  m_state = FingerSetup::FingerSetupCommand::GetEcho;
}

void FingerSetup::closePort() {
  if (m_port && m_port->isOpen()) {
    m_port->close();
    qInfo() << "Port closed successfully.";
  } else {
    qWarning() << "Port is not open or already closed.";
  }
}

QByteArray FingerSetup::buildPacket(uint8_t pid, const QByteArray &content) {
  QByteArray packet;
  packet.append(0xEF);
  packet.append(0x01);
  packet.append(0xFF);
  packet.append(0xFF);
  packet.append(0xFF);
  packet.append(0xFF);
  packet.append(pid);

  uint16_t length = content.size() + 2;
  packet.append((length >> 8) & 0xFF);
  packet.append(length & 0xFF);

  packet.append(content);

  uint16_t checksum = calculateChecksum(packet.mid(6));
  packet.append((checksum >> 8) & 0xFF);
  packet.append(checksum & 0xFF);

  return packet;
}

void FingerSetup::genImage() {
  QByteArray content;
  content.append(0x01);  // instruction code
  QByteArray packet = buildPacket(0x01, content);
  this->sendData(packet);
  m_state = FingerSetup::FingerSetupCommand::GenImage;
}

void FingerSetup::img2Tz(uint8_t bufferId) {
  QByteArray content;
  content.append(0x02); // instruction code
  content.append(bufferId);
  QByteArray packet = buildPacket(0x01, content);
  this->sendData(packet);
  qInfo() << "Image to template conversion started." << "Buffer ID:" << static_cast<int>(bufferId);
  m_state = FingerSetup::FingerSetupCommand::Img2Tz;
}

void FingerSetup::regModel() {
  QByteArray content;
  content.append(0x05);  // instruction code
  QByteArray packet = buildPacket(0x01, content);
  this->sendData(packet);
  qInfo() << "Registering model started.";
  m_state = FingerSetup::FingerSetupCommand::RegModel;
}

void FingerSetup::store(uint8_t bufferId, uint16_t pageId) {
  QByteArray content;
  content.append(0x06);  // instruction code
  content.append(bufferId);
  content.append((pageId >> 8) & 0xFF);
  content.append(pageId & 0xFF);
  QByteArray packet = buildPacket(0x01, content);
  this->sendData(packet);
  qInfo() << "Storing fingerprint data started with Finger ID:" << pageId;
  m_state = FingerSetup::FingerSetupCommand::Store;
}

void FingerSetup::search(uint8_t bufferId, uint16_t startPage, uint16_t pageNum) {
  QByteArray content;
  content.append(0x04);  // instruction code
  content.append(bufferId);
  content.append((startPage >> 8) & 0xFF);
  content.append(startPage & 0xFF);
  content.append((pageNum >> 8) & 0xFF);
  content.append(pageNum & 0xFF);
  QByteArray packet = buildPacket(0x01, content);
  this->sendData(packet);
  m_state = FingerSetup::FingerSetupCommand::Search;
}

void FingerSetup::handleConfirmation(FingerSetup::FingerCommandReturnStatus status) {
  if (status == FingerSetup::FingerCommandReturnStatus::COMMAND_EXECUTION_COMPLETE_STATUS) {
    switch (m_step){
      case 0:
        this->setMessage("Remove finger");
        this->img2Tz(1);
        m_step++;
        QThread::currentThread()->msleep(2000);
        break;
      case 1:
        qInfo() << "Remove finger";
        this->genImage();
        m_step++;
        break;
      case 2:
        this->genImage();
        m_step++;
        break;
      case 3:
        this->img2Tz(2);
        m_step++;
        break;
      case 4:
        this->regModel();
        m_step++;
        break;
      case 5:
        this->store(1, m_fingerID);
        this->setMessage("Store finger with ID: " + QString::number(m_fingerID));
        m_step++;
        break;
      case 6:
        qInfo() << "Store finger succeed";
        this->setMessage("Store finger succeed");
        QThread::currentThread()->msleep(5000);
        this->enroll();  // Restart the enrollment process
        qInfo() << "Enrollment process completed.";
        break;
      default:
        qInfo() << "Mismatch step: " << m_step;
        this->setMessage("Mismatch step: " + QString::number(m_step));
        m_step = 0;  // Reset step if it goes out of bounds
        this->genImage();  // Restart the process
        break;
    }
  }
  else {
    this->genImage();
    this->setMessage("Put your finger in");
  }
}

FingerSetup::FingerCommandReturnStatus FingerSetup::getDataProcStatus() const {
  return m_commandReturnStatus;
}

void FingerSetup::enroll() {
  m_step = 0;  // Reset step for new enrollment
  qInfo() << "Waiting for finger ID to be set...";
  this->setMessage("Waiting for finger ID to be set...");
  m_loop.exec();  // Wait for finger ID to be set before starting enrollment
  qInfo() << "Starting fingerprint enrollment process..." << "Finger ID:" << m_fingerID;
  this->handleConfirmation(FingerSetup::FingerCommandReturnStatus::IDLE);  // Start the enrollment process by capturing the first image
  if (!m_isConnected){
    connect(this, &FingerSetup::confirmationCodeReceived, this, &FingerSetup::handleConfirmation);
    m_isConnected = true;
  }
  qInfo() << "Enrollment process initialized.";
}

void FingerSetup::emptyFingerData() {
    QByteArray content;
    content.append(0x0D); // instruction code for 'Empty'
    QByteArray packet = buildPacket(0x01, content);
    this->sendData(packet);
    qInfo() << "Finger data cleared.";
    m_state = FingerSetup::FingerSetupCommand::ClearFingerData;
}
int FingerSetup::fingerID() const
{
  return m_fingerID;
}

void FingerSetup::setFingerID(int newFingerID)
{
  if(newFingerID < 0 || newFingerID > 127){
    qInfo() << "Invalid FingerID";
    return;
  }
  m_fingerID = newFingerID;
  emit fingerIDChanged();
}

QString FingerSetup::message() const
{
  return m_message;
}

void FingerSetup::setMessage(const QString &newMessage)
{
  m_message = newMessage;
  emit messageChanged();
}
