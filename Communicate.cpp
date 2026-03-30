
#include "Communicate.h"
#include "qdebug.h"

Communicate::Communicate()
{
    m_serialPort = new QSerialPort();
}

Communicate::~Communicate()
{
    if (m_serialPort->isOpen())
    {
        m_serialPort->close();
    }

    delete m_serialPort;
    m_serialPort = NULL;
}

bool isDeviceConnected(const QString& portName, int baudRate = 9600)
{
    QSerialPort serial;
    serial.setPortName(portName);
    serial.setBaudRate(baudRate);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    if (!serial.open(QIODevice::ReadWrite)) {
        return false; // 打不开，说明可能无设备或被占用
    }

    // 可选：清空缓冲区
    serial.clear();

    // 示例：发送一个简单查询命令（根据你的设备修改！）
    // 比如：发送 "?" 并等待回应
    QByteArray data;
    data[0] = 0x7E;
    data[1] = 0x7E;
    data[2] = 0x00;
    data[3] = 0x05;
    data[4] = 0x28;
    data[5] = 0x00;
    data[6] = 0x0D;

    serial.write(data);
    serial.waitForBytesWritten(100);

    // 等待最多 200ms 响应
    if (serial.waitForReadyRead(200)) {
        QByteArray response = serial.readAll();
        serial.close();
        // 如果收到非空响应，认为有设备
        return !response.isEmpty();
    }

    serial.close();
    return false;
}

QStringList Communicate::getPortNameList()
{
    QStringList serialPortName;
    foreach(const QSerialPortInfo & info, QSerialPortInfo::availablePorts())
    {
        if (isDeviceConnected(info.portName()))
        {
            serialPortName << info.portName();
        }

        qDebug() << "serialPortName:" << info.portName();
    }

    return serialPortName;
}

void Communicate::openPort()
{
    m_portNameList = getPortNameList();
    if (m_portNameList.empty())
    {
        return;
    }

    if (m_serialPort->isOpen())
    {
        m_serialPort->clear();
        m_serialPort->close();
    }

    m_serialPort->setPortName(m_portNameList[0]);
    if (!m_serialPort->open(QIODevice::ReadWrite))   
    {
        qDebug() << "Open serial failed !";
        return;
    }
    qDebug() << " Open serieal success !"<< m_portNameList[0];

    m_serialPort->setBaudRate(QSerialPort::Baud9600, QSerialPort::AllDirections);
    m_serialPort->setDataBits(QSerialPort::Data8);     
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);      
    m_serialPort->setParity(QSerialPort::NoParity); 
    m_serialPort->setStopBits(QSerialPort::OneStop); 

    connect(m_serialPort, SIGNAL(readyRead()), this, SLOT(receiveInfo()));
}

void Communicate::setData(PROTOCOL p,int begin,int intensity,bool isStart,QByteArray& data)
{
    data[0] = 0x7E;
    data[1] = 0x7E;
    if (p==OPENBACK)
    {
        data[2] = 0x22;
    }
    else if (p== OPENLEG)
    {
        data[2] = 0x23;
    }
    else if (p==LEGUNBLOCK_RIGHTLEG1)
    {
        data[2] = 0x24;
    }
    else if (p == LEGUNBLOCK_LEFTLEG1)
    {
        data[2] = 0x25;
    }
    else if (p == LEGUNBLOCK_RIGHTLEG2)
    {
        data[2] = 0x26;
    }
    else if (p == LEGUNBLOCK_LEFTLEG2)
    {
        data[2] = 0x27;
    }
    else if (p == SHOULDERUNBLOCK)
    {
        data[2] = 0x21;
    }
    else if (p == HANDLE_MODEL)
    {
        data[2] = 0x32;
    }


    //QString hex1 = QString("%1").arg(begin, 2, 16, QChar('0')).toUpper(); // "05"
    //QString hex2 = QString("%1").arg(intensity, 2, 16, QChar('0')).toUpper(); // "28"
    //QString result = hex1 + hex2; // "0528"

    //data[3] = 0x05;
    //data[4] = 0x32;
    
    quint8 a = begin;
    quint8 b = intensity;
   // QString value = QString("%1").arg(begin, 2, 16, QLatin1Char('0'));
    //QByteArray temp = value.toUtf8();
    data.append(static_cast<char>(a));
    //value = QString("%1").arg(intensity, 2, 16, QLatin1Char('0'));
    //temp = value.toUtf8();
    data.append(static_cast<char>(b));

    if (isStart)
    {
        data[5] = 0x01;
    }
    else
    {
        data[5] = 0x00;
    }
    data[6] = 0x0D;
}

void Communicate::sendData(QString data)
{
    QByteArray dataToSend = data.toUtf8();
    int bytesWritten = m_serialPort->write(dataToSend);
    if (bytesWritten == -1)
    {
        qDebug() << "Failed to write data";
    }
    else
    {
        m_sendedMessage = dataToSend;
        qDebug() << bytesWritten << "bytes written";
    }
}

void Communicate::receiveInfo()
{
    QByteArray info = m_serialPort->readAll();
    //sendData(info);
    if (m_sendedMessage==info)
    {
        emit setMessage("Receive right return message !");
        qDebug() << "Receive right return message !";
    }
    else
    {
        if (0x55==info[3])
        {
            emit setMessage("out of touch !");
            emit sendState(CONTACT_ERROR);
        }
        else if (0xff == info[3])
        {
            emit setMessage("Touch ok !");
            emit sendState(CONTACT_OK);
        }

        //sendData(m_sendedMessage);
    }

    QString message = info.toHex().toUpper();

    emit setMessage(message);

    qDebug() << "receive info:" << message;
}