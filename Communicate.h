#pragma once

#include "qobject.h"

#include <QSerialPort>
#include <QSerialPortInfo>

#include "RobotComm.h"

class Communicate:public QObject
{
	Q_OBJECT

public:
	Communicate();
	~Communicate();

	void openPort();//打开串口

	void setData(PROTOCOL p, int begin, int intensity, bool isStart, QByteArray& data);

private:
	QStringList getPortNameList();

signals:
	void setMessage(QString);

	void sendState(CONTACT_STATE);

public slots:
	void receiveInfo();

	void sendData(QString data);

private:
	QSerialPort* m_serialPort; //串口类

	QStringList m_portNameList;

	QByteArray m_sendedMessage;
};
