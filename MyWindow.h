#pragma once

#include <QWidget>
#include "ui_MyWindow.h"

#include <QTimer>
#include <QSharedPointer>

#include "HomePage.h"
#include "AutoTreat.h"
#include "IntensiveTreat.h"
#include "FinishOrganize.h"
#include "AccoutManager.h"
#include "EquipInfo.h"
#include "SetUp.h"
#include "oder.h"
#include "inference.h"

#include "CameraGrabber.h"

enum RobotPort
{
    pDashboard,
    pwork,
    pmsg8,
    pmsg200,
    pmsgreturn
};

class PhysicalTherapyRobot;
class MyWindow : public QWidget
{
	Q_OBJECT

public:
	MyWindow(PhysicalTherapyRobot* robot,QWidget *parent = nullptr);
	~MyWindow();

    void sktDashboard_error();
    void MyWindow_connect();

    void SetWidgetHomePage();

    void pause();
    void WorkContinue();
    void poweron();
    void stop();

    void go();

    std::vector<cv::Point> detect(cv::Mat img, std::string ModelPath,cv::Mat& outImg);

    void getImage(cv::Mat& img/*std::vector<cv::Point3d>& points, cv::Mat& colorRawMat*/);

    std::vector<cv::Point3d> get3Dpoints(std::vector<cv::Point> base ,std::vector<OBColorPoint> pointCloud_frame_data);
public slots:
	void On_PushButton_Exit_Clicked();

	void On_pushButton_MainFrame_Clicked();

	void On_pushButton_AutoTreat_Clicked();

	void On_pushButton_IntensiveTreat_Clicked();

	void On_pushButton_FinishOrganize_Clicked();

	void On_pushButton_AccountManage_Clicked();

	void On_pushButton_EquipInfo_Clicked();

	void On_pushButton_SetUp_Clicked();

	void On_timeout();

private:
	Ui::MyWindowClass ui;

	PhysicalTherapyRobot* m_pRobot;

	QTimer* m_pTimer;

	QSharedPointer<HomePage> m_pHomePage;
	QSharedPointer<AutoTreat> m_pAutoTreat;
	QSharedPointer<IntensiveTreat> m_pIntensiveTreat;
	QSharedPointer<FinishOrganize> m_pFinishOrganize;
	QSharedPointer<AccoutManager> m_pAccoutManager;
	QSharedPointer<EquipInfo> m_pEquipInfo;
	QSharedPointer<SetUp> m_pSetUp;

    QString ip = "192.168.5.1";
    oder od;
    int cnt[5] = {1, 1, 0, 0, 0};

    void sktDashboard_connected();
    void sktDashboard_readyRead();
    void sktDashboard_disconnected();
    void sktwork_connected();
    void sktwork_error();
    void sktwork_readyRead();
    void sktwork_disconnected();
    void sktmsg8_connected();
    void sktmsg8_error();
    void sktmsg8_readyRead();
    void sktmsg8_disconnected();
    void sktmsg200_connected();
    void sktmsg200_error();
    void sktmsg200_readyRead();
    void sktmsg200_disconnected();
    void sktmsgreturn_connected();
    void sktmsgreturn_error();
    void sktmsgreturn_readyRead();
    void sktmsgreturn_disconnected();
    void sendodr(QByteArray odr);
    void sendrunodr(QByteArray odr);

    void Sync();

    void JointMovJ(double x,double y,double z,double Rx,double Ry,double Rz);

    void MovJ(double X, double Y, double Z, double Rx, double Ry, double Rz);
    int pw = 0;
    int pausebit = 0;
    void GetSixForceData();
    void PositiveSolution(double J1, double J2, double J3, double J4, double J5, double J6, int User, int Tool);

    void qsleep(int msec); 
    void qsleep_pause(int msec);
    void InverseSolution(double X, double Y, double Z, double Rx, double Ry, double Rz, int User, int Tool, int isJointNear = 0, QString JointNear = "");
    void ServoJ(double J1, double J2, double J3, double J4, double J5, double J6, float t = 3600.0f, float lookahead_time = 100.0f, float gain = 200.0f);
    ////detecter dtt;
    void setip(QString ip);
    std::vector<cv::Point3d> points;
    cv::Mat colorRawMat;
    //OBColorPoint* Colorpoint;
    int imageWidth = 1280;
    int imageHeight = 720;
    std::vector<cv::Point3d> convert_camera2arm(std::vector<cv::Point3d> pointsC);
    cv::Point3d start_Camera_Point = cv::Point3d(-180.3657, -466.7497, 401.4063);
    //cv::Point3d start_Camera_Point = cv::Point3d(-180.3657, 466.7497, 401.4063);
    //cv::Point3d start_Camera_Point = cv::Point3d(-180.3657, -466.7497, 601.4063);
};
