#include "oder.h"

oder::oder(QObject *parent) : QObject(parent)
{
    sktDashboard = new QTcpSocket(this);
    sktwork = new QTcpSocket(this);
    sktmsg8 = new QTcpSocket(this);
    sktmsg200 = new QTcpSocket(this);
    sktmsgreturn = new QTcpSocket(this);
}

void oder::init(QString svr)
{
    server = svr;
    sktDashboard->connectToHost(svr, portDashboard, QIODevice::ReadWrite);
    sktwork->connectToHost(svr, portwork, QIODevice::ReadWrite);
    sktmsg8->connectToHost(svr, portmsg8, QIODevice::ReadOnly);
    sktmsg200->connectToHost(svr, portmsg200, QIODevice::ReadOnly);
    sktmsgreturn->connectToHost(svr, portmsgreturn, QIODevice::ReadOnly);
}
