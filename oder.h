#ifndef ODER_H
#define ODER_H

#include <QObject>
#include <QTcpSocket>

class oder : public QObject
{
    Q_OBJECT
public:
    explicit oder(QObject *parent = nullptr);
    QTcpSocket *sktDashboard;
    QTcpSocket *sktwork;
    QTcpSocket *sktmsg8;
    QTcpSocket *sktmsg200;
    QTcpSocket *sktmsgreturn;

    QString server = "192.168.1.6";
    uint16_t portDashboard = 29999;
    uint16_t portwork = 30003;
    uint16_t portmsg8 = 30004;
    uint16_t portmsg200 = 30005;
    uint16_t portmsgreturn = 30006;

    void init(QString svr = "192.168.1.6");
signals:

public slots:
};

#endif // ODER_H
