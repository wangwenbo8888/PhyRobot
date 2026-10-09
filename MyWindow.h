#pragma once

#include <QWidget>
#include "ui_MyWindow.h"

#include <QTimer>
#include <QSharedPointer>
#include <QThread>
#include <QSemaphore>
#include <QElapsedTimer>
#include <QMutex>

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

    // V4: 请求 TCP 控制权
    void RequestControl();

    // 拖拽示教模式
    void SetDragTeachMode(bool enable);
    bool IsDragTeachMode() const;

    // 末端负载(理疗头)参数读写
    double GetPayloadMass() const { return m_dPayloadMass; }
    double GetPayloadX()    const { return m_dPayloadX; }
    double GetPayloadY()    const { return m_dPayloadY; }
    double GetPayloadZ()    const { return m_dPayloadZ; }
    void SetPayloadMass(double v);
    void SetPayloadX(double v);
    void SetPayloadY(double v);
    void SetPayloadZ(double v);

    // 开始机械臂拖拽模式
    void StartDrag();

    // 停止机械臂拖拽模式
    void StopDrag();

    void ClearError();

    void RobotStorage();

    // 机械臂移动到默认的初始位置
    void MoveToNormalPos();

    // 拖拽专用待机位姿:立柱/上臂/前臂竖直向上,末端朝下,使负载沿重力方向作用,
    // J3(肘关节)力矩接近零,避免进入拖拽时 J3 因负载力矩过大触发硬件过流(报警8752)
    void MoveToDragReadyPos();

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

    // 只抓一张原图(不做运动/推理),供拖拽等场景使用;同样在采集线程执行
    bool captureRawImage(cv::Mat& colorRawMat);

    // 采集工作线程:实际抓拍+推理在独立线程执行,避免阻塞 GUI 线程
    void captureLoop();
    bool runCaptureFlow(cv::Mat& img);   // 完整流程:运动+抓拍+推理+3D
    bool runCaptureRaw(cv::Mat& img);    // 仅抓拍原图并保存
    bool submitCaptureJob(int jobType, cv::Mat& out);  // GUI 侧提交任务并协作等待

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

    // 供采集工作线程把 socket 写操作排到 GUI 线程执行
    void doWriteDashboard(QByteArray odr);

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

    QString ip = "192.168.5.1";
    //QString ip = "192.168.100.6";
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

    // 等待 Dashboard 对上一条命令的真实应答(带超时兜底),替代固定 500ms 空等
    bool waitDashboardAck(int timeoutMs);

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

    // V4: PositiveSolution → PositiveKin
    void PositiveKin(double J1, double J2, double J3, double J4, double J5, double J6, int user = -1, int tool = -1);

    void qsleep(int msec); 
    void qsleep_pause(int msec);
    // V4: InverseSolution → InverseKin
    void InverseKin(double X, double Y, double Z, double Rx, double Ry, double Rz, int user = -1, int tool = -1, int useJointNear = 0, QString jointNear = "");
    // V4: ServoJ 参数调整
    void ServoJ(double J1, double J2, double J3, double J4, double J5, double J6, float t = 0.1f, float aheadtime = 50.0f, float gain = 500.0f);

    // === V4 力控 API ===
    void EnableFTSensor();
    void SixForceHome();
    void GetForce();
    void ForceDriveMode(int axis, double value, int index = 0);
    void ForceDriveSpeed(int axis, double value, int index = 0);
    void FCForceMode(int mode);
    void FCSetDeviation(double x, double y, double z, double rx, double ry, double rz);
    void FCSetForceLimit(double x, double y, double z, double rx, double ry, double rz);
    void FCSetMass(double mass);
    void FCSetStiffness(double x, double y, double z, double rx, double ry, double rz);
    void FCSetDamping(double x, double y, double z, double rx, double ry, double rz);
    void FCOff();
    void FCSetForceSpeedLimit(double speed);
    void SetFCCollision(double force, double torque);
    void FCCollisionSwitch(int onoff);
    // V4: GetCurrentCommandId 用于同步
    void GetCurrentCommandId();
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

    // 已下发运动指令(由 JointMovJ/MovJ/MovL 置位,Wait_Done 消费)。
    // sendodr 改为等真实应答后,命令返回很快,不能再靠固定的 500ms 判断运动是否已开始。
    bool m_motionPending = false;

    // ===== 采集工作线程状态(getImage 在工作线程执行,避免卡死界面) =====
    QThread* m_pCaptureThread = nullptr;   // 常驻采集线程(相机 pipeline 只在该线程初始化)
    QSemaphore m_captureJobSem;            // GUI -> 工作线程 提交任务
    QSemaphore m_captureDoneSem;           // 工作线程 -> GUI 完成任务
    bool m_captureStop = false;
    bool m_bCapturing = false;
    int  m_captureJobType = 0;             // 0=完整流程, 1=仅抓原图
    bool m_captureResultOk = false;
    QString m_captureErr;
    cv::Mat m_captureImg;

    // Dashboard 应答同步(替代 sendodr 里的固定 500ms 空等)
    QSemaphore m_dashboardAckSem;
    int        m_cmdAckTimeoutMs = 1000;

    // sendodr 最小命令间隔: 连续过快下发时上一条应答会被本条误判(ACK错位), 这里做个保底限速
    QElapsedTimer m_cmdSendTimer;
    qint64        m_cmdMinGapMs = 20;
    QMutex        m_cmdSendMutex;

    // 治疗被停止了
    bool m_bStoped;

    // 拖拽示教模式（设置中勾选）
    bool m_bDragTeachMode = false;

    // 末端负载(理疗头)参数,从user.ini读取,进入拖拽时下发给控制器做重力补偿
    double m_dPayloadMass = 0.0;  // 质量(kg)
    double m_dPayloadX    = 0.0; // 质心X偏移(mm)
    double m_dPayloadY    = 0.0; // 质心Y偏移(mm)
    double m_dPayloadZ    = 0.0; // 质心Z偏移(mm)

    void Wait_ForTreat(int timeout = INT_MAX);

    void Wait_Done(int timeout = INT_MAX);

    void Wait_ForForces(int timeout = INT_MAX);

    void Wait_ForMove(int timeout = INT_MAX);

    void Wait_ForShort(int timeout = INT_MAX);

    double strToDouble(std::string str);

    cv::dnn::Net m_net;
};
