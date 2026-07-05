#ifndef ODER_H
#define ODER_H

#include <QObject>
#include <QTcpSocket>

class oder : public QObject
{
    Q_OBJECT
public:
    explicit oder(QObject *parent = nullptr);
    QTcpSocket *sktDashboard;   // 29999: Dashboard (RequestControl,EnableRobot,力控配置)
    QTcpSocket *sktwork;        // 30003: 运动指令 (MovJ/MovL/ServoJ/GetPose 等)
    QTcpSocket *sktmsg8;        // 30004: 实时反馈 8ms (SixForceValue @1304)
    QTcpSocket *sktmsg200;      // 30005: RobotMode 状态 200ms (byte[24])
    QTcpSocket *sktmsgreturn;   // 30006: 返回信息 1000ms

    QString server = "192.168.5.1";
    uint16_t portDashboard = 29999;
    uint16_t portwork = 30003;
    uint16_t portmsg8 = 30004;
    uint16_t portmsg200 = 30005;
    uint16_t portmsgreturn = 30006;

    void init(QString svr = "192.168.5.1");
signals:

public slots:
};

#endif // ODER_H
