#pragma once

#include <QWidget>
#include "ui_MyWindow.h"

#include <QTimer>
#include <QSharedPointer>

#include "HomePage.h"
#include "TreatInstruction.h"
#include "AutoTreat.h"
#include "AccountInfo.h"
#include "IntensiveTreat.h"
#include "FinishOrganize.h"
#include "AccoutManager.h"
#include "EquipInfo.h"
#include "SetUp.h"
#include "oder.h"
#include "inference.h"

#include "CameraGrabber.h"

#include "RobotComm.h"

#include <vector>

#include "src/Wifi/WifiList.h"

#include <Eigen/Dense>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include "Algo/Rotation.h"

enum TreatType
{
    Treat_Auto = 0,
    Treat_Manual,
    TreatPlan_Set,
    Settings,
    Treat_Unknown
};

enum RobotPort
{
    pDashboard,
    pwork,
    pmsg8,
    pmsg200,
    pmsgreturn
};

void CreateFolder(const QString& folderPath);

QString GetCurrentTimeSecond();

void SaveImage(const cv::Mat& img);

class PhysicalTherapyRobot;
class AdmittanceController;
class Communicate;

class MyWindow : public QWidget
{
	Q_OBJECT

public:
	MyWindow(PhysicalTherapyRobot* robot,QWidget *parent = nullptr);
	~MyWindow();

    // 开始机械臂拖拽模式
    void StartDrag();

    // 停止机械臂拖拽模式
    void StopDrag();

    void ClearError();

    void RobotStorage();

    // 机械臂移动到默认的初始位置
    void MoveToNormalPos();

    void sktDashboard_error();
    void MyWindow_connect();

    void SetWidgetHomePage();

    AutoTreat* GetAutoTreat()
    {
        return m_pAutoTreat.get();
    }

    void SetWidgetSetUp();

    void SetWidgetInstruction(TreatType type);

    void SetWidgetAfterInstruction();

    void pause();
    void WorkContinue();
    void poweron();
    void stop();

    int GetIntensity();

    void IncrIntensity();

    void DecrIntensity();

    void SetStoped(bool flag);

    void RobotGoHome();

    Communicate* GetCommunicate();

    void go();

    void Run(PROTOCOL pro,int level = 2);

    void SetModel(OPENBACK_MODEL model);

    void SetCurrentProj(QString str);

    void ResetRobot();

    bool SetXuewei(const std::vector<std::vector<XUEWEI_INFO>>& xueweis);

    bool AdmittanceControl();

    bool AdmittanceControlNew(int group, int row);

    Eigen::VectorXd getForceFeedback();

    void MovJInterface(double x, double y, double z, double Rx, double Ry, double Rz);

    std::vector<cv::Point> detect(cv::Mat img, std::string ModelPath,cv::Mat& outImg);

    bool getImage(cv::Mat& img/*std::vector<cv::Point3d>& points, cv::Mat& colorRawMat*/);

    std::vector<Robot3d> get3Dpoints(std::vector<cv::Point> base ,std::vector<OBColorPoint> pointCloud_frame_data);

    cv::Mat& getPlanImage();

    QPixmap cvMatToQPixmap(const cv::Mat& inMat);

    std::vector<RobotPoint>& GetPlanPoints();

    void setPlanPoints(std::vector<RobotPoint> points);
    bool MoveToNextAcupointNew_Improved(int group, int row, DETECTED_XUEWEI currentxuewei,
        DETECTED_XUEWEI nextxuewei, std::vector<double>& next);

    bool MoveToNextAcupointNew_ImprovedV2(int group, int row, DETECTED_XUEWEI currentxuewei,
        DETECTED_XUEWEI nextxuewei, std::vector<double>& next);

    bool MoveToNextAcupointNew_ImprovedV3(int group, int row, DETECTED_XUEWEI currentxuewei,
        DETECTED_XUEWEI nextxuewei, std::vector<double>& next);

    bool MoveToNextAcupointNew_ImprovedV4(int group, int row, DETECTED_XUEWEI currentxuewei,
        DETECTED_XUEWEI nextxuewei, std::vector<double>& next);

    bool MoveToNextAcupointNew_ImprovedV5(int group, int row, DETECTED_XUEWEI currentxuewei,
        DETECTED_XUEWEI nextxuewei, std::vector<double>& next);

    bool MoveToNextAcupointNew_ImprovedV6(int group, int row, DETECTED_XUEWEI currentxuewei,
        DETECTED_XUEWEI nextxuewei, std::vector<double>& next,int level);

    bool MoveToNextAcupointNew_ImprovedV6_Optimized(int group, int row, DETECTED_XUEWEI currentxuewei,
        DETECTED_XUEWEI nextxuewei, std::vector<double>& next, int ration);

    bool MoveToNextAcupointNew_ImprovedV7(int group, int row, DETECTED_XUEWEI currentxuewei,
        DETECTED_XUEWEI nextxuewei, std::vector<double>& next, int level);

    bool MoveToNextAcupointNew_ImprovedV8(int group, int row, DETECTED_XUEWEI currentxuewei,
        DETECTED_XUEWEI nextxuewei, std::vector<double>& next, int level);

    bool MoveToNextAcupointNew_ImprovedV9(int group, int row, DETECTED_XUEWEI currentxuewei,
        DETECTED_XUEWEI nextxuewei, std::vector<double>& next, int ration);

    bool MoveToNextAcupointNew_ImprovedV10(int group, int row, DETECTED_XUEWEI currentxuewei,
        DETECTED_XUEWEI nextxuewei, std::vector<double>& next, int ration);

    bool MoveToNextAcupointNew_ImprovedV11(int group, int row, DETECTED_XUEWEI currentxuewei,
        DETECTED_XUEWEI nextxuewei, std::vector<double>& next, int ration);

public slots:
	void On_PushButton_Exit_Clicked();

	void On_pushButton_MainFrame_Clicked();

	void On_pushButton_AutoTreat_Clicked();

	void On_pushButton_IntensiveTreat_Clicked();

	void On_pushButton_FinishOrganize_Clicked();

	void On_pushButton_AccountManage_Clicked();

	void On_pushButton_EquipInfo_Clicked();

	void On_pushButton_SetUp_Clicked();

    void On_pushButton_Account_Clicked();

    void On_pushButton_Wifi_Clicked();

    void On_pushButton_ClearError_Clicked();

	void On_timeout();

    void On_ContactTimeOut();

    void On_received_contact_state(CONTACT_STATE);

signals:
    void FinishOneXuewei(int);

    void FinishOneSecond();

    void FinishOneGroup();

private:
    QImage cvMatToQImage(const cv::Mat& inMat);

    // 步进模式
    bool StepModel(PROTOCOL pro,int level);

    // 连续模式
    bool ContinueModel(PROTOCOL pro,int level);

    // 以贴近皮肤的方式移动到下一个穴位
    bool MoveToNextAcupoint(int group,int row,std::vector<double>& next);

    bool MoveToNextAcupointNew(int group, int row, DETECTED_XUEWEI currentxuewei,DETECTED_XUEWEI xuewei, std::vector<double>& next);

    bool GetNextAcupoint(int i, int j,  DETECTED_XUEWEI& xuewei,std::vector<double>& point);

    std::vector<double> m_vForces;

    std::vector<double> m_vRawForces;

    std::vector<double> m_vCurrentPos;
    
    // 对应的穴位法矢
    std::vector<double> m_vPosNormal;

    PROTOCOL m_eCurrProto;
    int m_iCurrIntensity;

    Point3D m_PreForce;
    Point3D m_CurrForce;

    Point3D m_PreForceDir;
    Point3D m_CurrForceDir;
    OPENBACK_MODEL m_eModel;
private:
	Ui::MyWindowClass ui;

	PhysicalTherapyRobot* m_pRobot;

    AdmittanceController* m_pAddmittance;

    Communicate* m_pCommunicate;

    AccountInfo* m_pAccountInfo;

	QTimer* m_pTimer;

    QTimer* m_pContactTimer;

    QSharedPointer<WifiList> m_pWifiWindow;

    cv::Mat m_mPlanImage;

    TreatType m_eTreatType;

	QSharedPointer<HomePage> m_pHomePage;
    QSharedPointer<TreatInstruction> m_pTreatInstruction;
	QSharedPointer<AutoTreat> m_pAutoTreat;
	QSharedPointer<IntensiveTreat> m_pIntensiveTreat;
	QSharedPointer<FinishOrganize> m_pFinishOrganize;
	QSharedPointer<AccoutManager> m_pAccoutManager;
	QSharedPointer<EquipInfo> m_pEquipInfo;
	QSharedPointer<SetUp> m_pSetUp;

    //QString ip = "192.168.5.1";
    QString ip = "192.168.100.6";
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

    void JointMovJ(double Jx, double J2, double J3, double J4, double J5, double J6);

    void MovJ(double X, double Y, double Z, double Rx, double Ry, double Rz);

    void MovL(double X, double Y, double Z, double Rx, double Ry, double Rz);

    void ServoP(double X, double Y, double Z, double Rx, double Ry, double Rz);

    void Tool(int tool);

    int pw = 0;
    int pausebit = 0;
    void GetSixForceData();

    void GetPose();

    void PositiveSolution(double J1, double J2, double J3, double J4, double J5, double J6, int User, int Tool);

    void qsleep(int msec); 
    void qsleep_pause(int msec);
    void InverseSolution(double X, double Y, double Z, double Rx, double Ry, double Rz, int User, int Tool, int isJointNear = 0, QString JointNear = "");
    void ServoJ(double J1, double J2, double J3, double J4, double J5, double J6, float t = 3600.0f, float lookahead_time = 100.0f, float gain = 200.0f);
    ////detecter dtt;
    void setip(QString ip);
    std::vector<RobotPoint> points;
    std::vector<std::vector<RobotPoint>> m_vXueweis;

    cv::Mat colorRawMat;
    //OBColorPoint* Colorpoint;
    int imageWidth = 1280;
    int imageHeight = 720;
    std::vector<cv::Point3d> convert_camera2arm(std::vector<Robot3d> pointsC);
    //cv::Point3d start_Camera_Point = cv::Point3d(-180.3657, -466.7497, 401.4063);//机械臂末端关节在拍照时的位置
    cv::Point3d start_Camera_Point = cv::Point3d(-136.4293, -486.0599, 347.5884);//机械臂末端关节在拍照时的位置 工具坐标系2
    //cv::Point3d start_Camera_Point = cv::Point3d(-180.3657, 466.7497, 401.4063);
    //cv::Point3d start_Camera_Point = cv::Point3d(-180.3657, -466.7497, 601.4063);
    enum ROBOT_MODE {
        ROBOT_MODE_UNKNOW,           //未知
        ROBOT_MODE_INIT,             //初始化
        ROBOT_MODE_BRAKE_OPEN,       //有任意关节的抱闸松开
        ROBOT_MODE_POWER_STATUS,     //本体未上电
        ROBOT_MODE_DISABLED,         //未使能(无抱闸松开)
        ROBOT_MODE_ENABLE,           //使能且空闲(无报警,未运行工程)
        ROBOT_MODE_BACKDRIVE,        //拖拽模式
        ROBOT_MODE_RUNNING,          //运行状态，包括轨迹复现/拟合中，机器人执行命令中，工程运行中
        ROBOT_MODE_RECORDING,        //轨迹录制模式
        ROBOT_MODE_ERROR,            //有未清除的报警。此状态优先级最高，无论机械臂处于什么状态，有报警时都返回9
        ROBOT_MODE_PAUSE,            //暂停状态
        ROBOT_MODE_JOG               //电动中
    };

    int RobotMode = 0;

    // 治疗被停止了
    bool m_bStoped;

    void Wait_ForTreat(int timeout = INT_MAX);

    void Wait_Done(int timeout = INT_MAX);

    void Wait_ForForces(int timeout = INT_MAX);

    void Wait_ForMove(int timeout = INT_MAX);

    void Wait_ForShort(int timeout = INT_MAX);

    double strToDouble(std::string str);

    cv::dnn::Net m_net;
};
