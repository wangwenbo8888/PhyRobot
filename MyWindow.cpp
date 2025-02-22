#include "MyWindow.h"
#include "PhysicalTherapyRobot.h"

#include <QTime>
#include <QStringLiteral>

MyWindow::MyWindow(PhysicalTherapyRobot* robot,QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	m_pTimer = new QTimer(this);

	m_pTimer->setInterval(1000);
	connect(m_pTimer,SIGNAL(timeout()),this,SLOT(On_timeout()));
	m_pTimer->start();

	m_pRobot = robot;
	disconnect(ui.pushButton_Exit, SIGNAL(clicked()), this, SLOT(On_PushButton_Exit_Clicked()));
	connect(ui.pushButton_Exit,SIGNAL(clicked()),this,SLOT(On_PushButton_Exit_Clicked()));
	
	disconnect(ui.pushButton_MainFrame, SIGNAL(clicked()),this,SLOT(On_pushButton_MainFrame_Clicked()));
	connect(ui.pushButton_MainFrame, SIGNAL(clicked()), this, SLOT(On_pushButton_MainFrame_Clicked()));

	disconnect(ui.pushButton_AutoTreat,SIGNAL(clicked()),this,SLOT(On_pushButton_AutoTreat_Clicked()));
	connect(ui.pushButton_AutoTreat, SIGNAL(clicked()), this, SLOT(On_pushButton_AutoTreat_Clicked()));

	disconnect(ui.pushButton_IntensiveTreat,SIGNAL(clicked()),this,SLOT(On_pushButton_IntensiveTreat_Clicked()));
	connect(ui.pushButton_IntensiveTreat, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensiveTreat_Clicked()));

	disconnect(ui.pushButton_FinishOrganize, SIGNAL(clicked()), this, SLOT(On_pushButton_FinishOrganize_Clicked()));
	connect(ui.pushButton_FinishOrganize, SIGNAL(clicked()), this, SLOT(On_pushButton_FinishOrganize_Clicked()));

	disconnect(ui.pushButton_AccountManage,SIGNAL(clicked()),this,SLOT(On_pushButton_AccountManage_Clicked()));
	connect(ui.pushButton_AccountManage, SIGNAL(clicked()), this, SLOT(On_pushButton_AccountManage_Clicked()));

	disconnect(ui.pushButton_EquipInfo,SIGNAL(clicked()),this,SLOT(On_pushButton_EquipInfo_Clicked()));
	connect(ui.pushButton_EquipInfo, SIGNAL(clicked()), this, SLOT(On_pushButton_EquipInfo_Clicked()));

	disconnect(ui.pushButton_SetUp,SIGNAL(clicked()),this,SLOT(On_pushButton_SetUp_Clicked()));
	connect(ui.pushButton_SetUp, SIGNAL(clicked()), this, SLOT(On_pushButton_SetUp_Clicked()));

	m_pHomePage.reset(new HomePage(this));
	ui.stackedWidget_Pags->addWidget(m_pHomePage.get());

	m_pAutoTreat.reset(new AutoTreat(this));
	ui.stackedWidget_Pags->addWidget(m_pAutoTreat.get());

	m_pIntensiveTreat.reset(new IntensiveTreat(this));
	ui.stackedWidget_Pags->addWidget(m_pIntensiveTreat.get());

	m_pFinishOrganize.reset(new FinishOrganize(this));
	ui.stackedWidget_Pags->addWidget(m_pFinishOrganize.get());

	m_pAccoutManager.reset(new AccoutManager(this));
	ui.stackedWidget_Pags->addWidget(m_pAccoutManager.get());

	m_pEquipInfo.reset(new EquipInfo(this));
	ui.stackedWidget_Pags->addWidget(m_pEquipInfo.get());

	m_pSetUp.reset(new SetUp(this));
	ui.stackedWidget_Pags->addWidget(m_pSetUp.get());

	ui.pushButton_MainFrame->click();
	//ui.stackedWidget_Pags->setCurrentWidget(m_pHomePage.get());

    connect(od.sktDashboard, &QTcpSocket::connected, this, &MyWindow::sktDashboard_connected);
    connect(od.sktDashboard, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &MyWindow::sktDashboard_error);
    connect(od.sktDashboard, &QTcpSocket::readyRead, this, &MyWindow::sktDashboard_readyRead);
    connect(od.sktDashboard, &QTcpSocket::disconnected, this, &MyWindow::sktDashboard_disconnected);

    connect(od.sktwork, &QTcpSocket::connected, this, &MyWindow::sktwork_connected);
    connect(od.sktwork, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &MyWindow::sktwork_error);
    connect(od.sktwork, &QTcpSocket::readyRead, this, &MyWindow::sktwork_readyRead);
    connect(od.sktwork, &QTcpSocket::disconnected, this, &MyWindow::sktwork_disconnected);

    connect(od.sktmsg8, &QTcpSocket::connected, this, &MyWindow::sktmsg8_connected);
    connect(od.sktmsg8, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &MyWindow::sktmsg8_error);
    connect(od.sktmsg8, &QTcpSocket::readyRead, this, &MyWindow::sktmsg8_readyRead);
    connect(od.sktmsg8, &QTcpSocket::disconnected, this, &MyWindow::sktmsg8_disconnected);

    connect(od.sktmsg200, &QTcpSocket::connected, this, &MyWindow::sktmsg200_connected);
    connect(od.sktmsg200, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &MyWindow::sktmsg200_error);
    connect(od.sktmsg200, &QTcpSocket::readyRead, this, &MyWindow::sktmsg200_readyRead);
    connect(od.sktmsg200, &QTcpSocket::disconnected, this, &MyWindow::sktmsg200_disconnected);

    connect(od.sktmsgreturn, &QTcpSocket::connected, this, &MyWindow::sktmsgreturn_connected);
    connect(od.sktmsgreturn, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &MyWindow::sktmsgreturn_error);
    connect(od.sktmsgreturn, &QTcpSocket::readyRead, this, &MyWindow::sktmsgreturn_readyRead);
    connect(od.sktmsgreturn, &QTcpSocket::disconnected, this, &MyWindow::sktmsgreturn_disconnected);

    MyWindow_connect();
}

MyWindow::~MyWindow()
{
	// m_pTimer->destroyed();
}

void MyWindow::On_PushButton_Exit_Clicked()
{
	this->close();
	m_pRobot->close();
}

void MyWindow::On_pushButton_MainFrame_Clicked()
{
	ui.pushButton_MainFrame->setDown(true);
	ui.pushButton_AutoTreat->setDown(false);
	ui.pushButton_IntensiveTreat->setDown(false);
	ui.pushButton_FinishOrganize->setDown(false);
	ui.pushButton_AccountManage->setDown(false);
	ui.pushButton_EquipInfo->setDown(false);
	ui.pushButton_SetUp->setDown(false);

	ui.stackedWidget_Pags->setCurrentWidget(m_pHomePage.get());
}

void MyWindow::On_pushButton_AutoTreat_Clicked()
{
	ui.pushButton_MainFrame->setDown(false);
	ui.pushButton_AutoTreat->setDown(true);
	ui.pushButton_IntensiveTreat->setDown(false);
	ui.pushButton_FinishOrganize->setDown(false);
	ui.pushButton_AccountManage->setDown(false);
	ui.pushButton_EquipInfo->setDown(false);
	ui.pushButton_SetUp->setDown(false);

	m_pAutoTreat->SetWidgetInstruction();
	ui.stackedWidget_Pags->setCurrentWidget(m_pAutoTreat.get());
}

void MyWindow::On_pushButton_IntensiveTreat_Clicked()
{
	ui.pushButton_MainFrame->setDown(false);
	ui.pushButton_AutoTreat->setDown(false);
	ui.pushButton_IntensiveTreat->setDown(true);
	ui.pushButton_FinishOrganize->setDown(false);
	ui.pushButton_AccountManage->setDown(false);
	ui.pushButton_EquipInfo->setDown(false);
	ui.pushButton_SetUp->setDown(false);

	m_pIntensiveTreat->SetWidgetPalliativeCare();
	ui.stackedWidget_Pags->setCurrentWidget(m_pIntensiveTreat.get());
}

void MyWindow::On_pushButton_FinishOrganize_Clicked()
{
	ui.pushButton_MainFrame->setDown(false);
	ui.pushButton_AutoTreat->setDown(false);
	ui.pushButton_IntensiveTreat->setDown(false);
	ui.pushButton_FinishOrganize->setDown(true);
	ui.pushButton_AccountManage->setDown(false);
	ui.pushButton_EquipInfo->setDown(false);
	ui.pushButton_SetUp->setDown(false);

    m_pFinishOrganize->InitState();
	ui.stackedWidget_Pags->setCurrentWidget(m_pFinishOrganize.get());
}

void MyWindow::On_pushButton_AccountManage_Clicked()
{
	ui.pushButton_MainFrame->setDown(false);
	ui.pushButton_AutoTreat->setDown(false);
	ui.pushButton_IntensiveTreat->setDown(false);
	ui.pushButton_FinishOrganize->setDown(false);
	ui.pushButton_AccountManage->setDown(true);
	ui.pushButton_EquipInfo->setDown(false);
	ui.pushButton_SetUp->setDown(false);

	ui.stackedWidget_Pags->setCurrentWidget(m_pAccoutManager.get());
}

void MyWindow::On_pushButton_EquipInfo_Clicked()
{
	ui.pushButton_MainFrame->setDown(false);
	ui.pushButton_AutoTreat->setDown(false);
	ui.pushButton_IntensiveTreat->setDown(false);
	ui.pushButton_FinishOrganize->setDown(false);
	ui.pushButton_AccountManage->setDown(false);
	ui.pushButton_EquipInfo->setDown(true);
	ui.pushButton_SetUp->setDown(false);

	ui.stackedWidget_Pags->setCurrentWidget(m_pEquipInfo.get());
}

void MyWindow::On_pushButton_SetUp_Clicked()
{
	ui.pushButton_MainFrame->setDown(false);
	ui.pushButton_AutoTreat->setDown(false);
	ui.pushButton_IntensiveTreat->setDown(false);
	ui.pushButton_FinishOrganize->setDown(false);
	ui.pushButton_AccountManage->setDown(false);
	ui.pushButton_EquipInfo->setDown(false);
	ui.pushButton_SetUp->setDown(true);

	ui.stackedWidget_Pags->setCurrentWidget(m_pSetUp.get());
}

void MyWindow::On_timeout()
{
	QTime currentTime = QTime::currentTime();
	QString timeString = currentTime.toString("hh:mm");

	// 获取当前日期
	QDate currentDate = QDate::currentDate();

	// 格式化输出日期
	QString dateString = currentDate.toString(QStringLiteral("yyyy年MM月dd日"));

	// 格式化输出星期
	//QString weekDayString = currentDate.toString("dddd");  // 返回英文星期
	//qDebug() << "今天是: " << weekDayString;

	// 使用自定义的星期表示
	const QString daysOfWeek[] = { "", QStringLiteral("星期一"),
		QStringLiteral("星期二"), QStringLiteral("星期三"), QStringLiteral("星期四"),
		QStringLiteral("星期五"), QStringLiteral("星期六"), QStringLiteral("星期日") };
	int dayOfWeek = currentDate.dayOfWeek();
	QString text = dateString;
	text.append(" ");
	text.append(daysOfWeek[dayOfWeek]);

	text.append(" ");
	text.append(timeString);
	ui.label_DateTime->setText(QObject::tr(text.toStdString().c_str()));
}

void MyWindow::SetWidgetHomePage()
{
    ui.stackedWidget_Pags->setCurrentWidget(m_pHomePage.get());
}

void MyWindow::MyWindow_connect()
{
    od.init(ip);
    qDebug() << "Connect to " << od.server << endl;
}

void MyWindow::pause()
{
    pausebit = 1;
}

void MyWindow::WorkContinue()
{
    pausebit = 0;
}

void MyWindow::sktDashboard_connected()
{
    cnt[pDashboard] = 1;
    qDebug() << "sktDashboard_connected" << endl;
}

void MyWindow::sktDashboard_readyRead()
{
    qDebug() << "sktDashboard_readyRead" << endl;
    QByteArray msg = od.sktDashboard->readAll();
    qDebug() << msg << endl;
}

void MyWindow::sktDashboard_error()
{
    qDebug() << "sktDashboard_error:" << od.sktDashboard->errorString() << endl;
    //emit robotmessage(od.sktDashboard->errorString());
}

void MyWindow::sktDashboard_disconnected()
{
    qDebug() << "sktDashboard_disconnected" << endl;
}

void MyWindow::sktwork_connected()
{
    cnt[pwork] = 1;
    qDebug() << "sktDashboard_connected" << endl;
}

void MyWindow::sktwork_error()
{
    qDebug() << "sktwork_error:" << od.sktwork->errorString() << endl;
}

void MyWindow::sktwork_readyRead()
{
    qDebug() << "sktwork_readyRead" << endl;
    QByteArray msg = od.sktwork->readAll();
     qDebug() << msg << endl;
}

void MyWindow::sktwork_disconnected()
{
    qDebug() << "sktwork_disconnected" << endl;
}

void MyWindow::sktmsg8_connected()
{
    cnt[pmsg8] = 1;
    qDebug() << "sktmsg8_connected" << endl;
}

void MyWindow::sktmsg8_error()
{
    qDebug() << "sktmsg8_error:" << od.sktmsg8->errorString() << endl;
}

void MyWindow::sktmsg8_readyRead()
{
    //qDebug() << "sktmsg8_readyRead" << endl;
    QByteArray msg = od.sktmsg8->readAll();
    //qDebug() << msg << endl;
}

void MyWindow::sktmsg8_disconnected()
{
    qDebug() << "sktmsg8_disconnected" << endl;
}

void MyWindow::sktmsg200_connected()
{
    cnt[pmsg200] = 1;
    qDebug() << "sktmsg200_connected" << endl;
}

void MyWindow::sktmsg200_error()
{
//    qDebug() << "sktmsg200_error:" << od.sktmsg200->errorString() << endl;
}

void MyWindow::sktmsg200_readyRead()
{
    //qDebug() << "sktmsg200_readyRead" << endl;
    QByteArray msg = od.sktmsg200->readAll();
    //qDebug() << msg << endl;
}

void MyWindow::sktmsg200_disconnected()
{
    qDebug() << "sktmsg200_disconnected" << endl;
}

void MyWindow::sktmsgreturn_connected()
{
    cnt[pmsgreturn] = 1;
    qDebug() << "sktmsgreturn_connected" << endl;
}

void MyWindow::sktmsgreturn_error()
{
    qDebug() << "sktmsgreturn_error:" << od.sktmsgreturn->errorString() << endl;
}

void MyWindow::sktmsgreturn_readyRead()
{
    //qDebug() << "sktmsgreturn_readyRead" << endl;
    QByteArray msg = od.sktmsgreturn->readAll();
    //qDebug() << msg << endl;
}

void MyWindow::sktmsgreturn_disconnected()
{
    qDebug() << "sktmsgreturn_disconnected" << endl;
}

void MyWindow::sendodr(QByteArray odr)
{
    if(cnt[pDashboard])
    {
        od.sktDashboard->write(odr);
        qDebug() << "Send Dash board oder: " << odr;
    }
    qsleep(2000);
}


void MyWindow::sendrunodr(QByteArray odr)
{
    if(cnt[pwork])
    {
        od.sktwork->write(odr);
        qDebug() << "Send run oder: " << odr;
    }
}

void MyWindow::GetSixForceData()
{
    sendodr("GetSixForceData()");
}

void MyWindow::PositiveSolution(double J1, double J2, double J3, double J4, double J5, double J6, int User, int Tool)
{
    sendodr("PositiveSolution(" + QByteArray::number(J1) + "," + QByteArray::number(J2) + "," + QByteArray::number(J3) + "," +
               QByteArray::number(J4) + "," + QByteArray::number(J5) + "," + QByteArray::number(J6) + "," +
               QByteArray::number(User) + "," + QByteArray::number(Tool) + ")");
}

void MyWindow::InverseSolution(double X, double Y, double Z, double Rx, double Ry, double Rz,
                          int User, int Tool, int isJointNear, QString JointNear)
{
    if(isJointNear)
    {
        sendodr("InverseSolution(" + QByteArray::number(X) + "," + QByteArray::number(Y) + "," + QByteArray::number(Z) + "," +
                   QByteArray::number(Rx) + "," + QByteArray::number(Ry) + "," + QByteArray::number(Rz) + "," +
                   QByteArray::number(User) + "," + QByteArray::number(Tool) + "," +
                   QByteArray::number(isJointNear) + "," + JointNear.toLatin1() + ")");
    } else {
        sendodr("InverseSolution(" + QByteArray::number(X) + "," + QByteArray::number(Y) + "," + QByteArray::number(Z) + "," +
                   QByteArray::number(Rx) + "," + QByteArray::number(Ry) + "," + QByteArray::number(Rz) + "," +
                   QByteArray::number(User) + "," + QByteArray::number(Tool) + ")");
    }
}

void MyWindow::ServoJ(double J1, double J2, double J3, double J4, double J5, double J6, float t, float lookahead_time, float gain) {
    sendrunodr("ServoJ(" + QByteArray::number(J1) + "," + QByteArray::number(J2) + "," + QByteArray::number(J3) + "," +
               QByteArray::number(J4) + "," + QByteArray::number(J5) + "," + QByteArray::number(J6) + "," +
               QByteArray::number(double(t)) + "," + QByteArray::number(double(lookahead_time)) + "," + QByteArray::number(double(gain)) + ")");
}


void MyWindow::MovJ(double X, double Y, double Z, double Rx, double Ry, double Rz)
{
    sendrunodr("MovJ(" + QByteArray::number(X) + "," + QByteArray::number(Y) + "," + QByteArray::number(Z) + "," +
               QByteArray::number(Rx) + "," + QByteArray::number(Ry) + "," + QByteArray::number(Rz) + ")");
}

void MyWindow::go()
{
//    p3d toArmiarm20p7e = toArm(iarm[2][0].p[7].e/* + iarm[0][0].p[7].e*/) / mm;        //去除中心点偏移
//    p3d iarm20p7pd = p3d(iarm[2][0].p[7].d[0].s, iarm[2][0].p[7].d[1].s, iarm[2][0].p[7].d[2].s);
//    p3d toArmiarm20p7pd = toArmR(iarm20p7pd);
//    MovJ(toArmiarm20p7e.x, toArmiarm20p7e.y, toArmiarm20p7e.z,
//    toArmiarm20p7pd.x, toArmiarm20p7pd.y, toArmiarm20p7pd.z);
    //MovJ(47.68, -371.8399, 1135.8821, -90, -0.281, -175.8179);
    //qsleep_pause(7000);



//    MovJ(-160, -775.3, 291.8, -178, 0, 179.5);
//    qsleep_pause(7000);
//    MovJ(-160, -775.3, 91.8, -178, 0, 179.5);
//    qsleep_pause(8000);
//    MovJ(-160, -675.3, 91.8, -178, 0, 179.5);
//    qsleep_pause(8000);
//    MovJ(-160, -875.3, 91.8, -178, 0, 179.5);
//    qsleep_pause(7000);
//    MovJ(-160, -775.3, 91.8, -178, 0, 179.5);
//    qsleep_pause(7000);
//    MovJ(-360, -775.3, 91.8, -178, 0, 179.5);
//    qsleep_pause(7000);
//    MovJ(240, -775.3, 91.8, -178, 0, 179.5);
//    qsleep_pause(7000);
//    MovJ(-160, -775.3, 91.8, -178, 0, 179.5);
//    qsleep_pause(7000);

    for (uint i = 0; i < points.size(); ++i)
    {
        MovJ(points[i].x, points[i].y, points[i].z, -178, 0, 179.5);   //联调     //
        qsleep_pause(7000);
        qDebug() << "Move to:" << points[i].x << ", " << points[i].y << ", " << points[i].z;
    }

    MovJ(-20, -376, 1134, -90, 2, 179.5);
    qsleep_pause(25000);

    //MovJ(47.68, -371.8399, 1135.8821, -90, -0.281, -175.8179);
    //qsleep_pause(7000);

    m_pAutoTreat->SetWidgetTreatFinish();

////    ServoJ(0, 0, 0, 0, 0, 0);
////    qsleep(30000);
//    for (int i = 0; i < workmap_arm.size(); ++i) {
//        p3d p = workmap_arm[i];
//        //@@@ MovJ(p.x, p.y, p.z, 0, 0, 0);                                   //穴位3D坐标，需要谨慎验证。
//        qsleep(3000);
//    }
}

std::vector<cv::Point> MyWindow::detect(cv::Mat img, std::string ModelPath)
{
    DCSP_INIT_PARAM params;
    params.ModelPath = ModelPath;
    params.ModelType = YOLO_POSE_V8;
    params.classesNum = 1;
    params.RectConfidenceThreshold = 0.6f;
    params.iouThreshold = 0.5f;
    params.CudaEnable = false;
    params.LogSeverityLevel = 3;
    params.imgSize = {640, 640};

    DCSP_CORE* p1 = new DCSP_CORE;
    char* ret = p1->CreateSession(params);

    //std::cout << img_path << std::endl;
    //cv::Mat img = cv::imread(img);
    //cv::imshow("TEST_ORIGIN", img);
    std::vector<cv::Point> re;
    std::vector<DCSP_RESULT> res;
    p1->RunSession(img, res);
    for (int i = 0; i < res.size(); i++)
    {
        cv::rectangle(img, res.at(i).box, cv::Scalar(125, 123, 0), 3);
        //cv::putText(img, std::to_string(i), res.at(i).box.tl() + cv::Point(3, 3), 1, 1, cv::Scalar(0), 1);
        re.push_back(cv::Point(res.at(i).box.x, res.at(i).box.y));
    }
#if 1
    cv::Mat img2;
    cv::resize(img, img2, img.size());
    cv::namedWindow("TEST_ORIGIN", cv::WINDOW_AUTOSIZE);
    cv::imshow("TEST_ORIGIN", img2);
    //cv::destroyAllWindows();
#endif //1
    return re;
    //cv::imwrite("E:\\output\\" + std::to_string(k) + ".png", img);

//    PositiveSolution(iarm[2][0].p[0].d[1].s, iarm[2][0].p[1].d[0].s, iarm[2][0].p[2].d[0].s,
//            iarm[2][0].p[3].d[0].s, iarm[2][0].p[4].d[1].s, iarm[2][0].p[5].d[0].s, 0, 0);
//    InverseSolution(iarm[2][0].p[0].d[1].s, iarm[2][0].p[1].d[0].s, iarm[2][0].p[2].d[0].s,
//            iarm[2][0].p[3].d[0].s, iarm[2][0].p[4].d[1].s, iarm[2][0].p[5].d[0].s, 0, 0);

////    orb.start();
//    qsleep(10000);                                                            //扫描10秒钟。
////    orb.stop();

////    workmap = orb.getPosition(dtt.forward(orb.colorRawMat));
//    workmap_arm.clear();
//    gm44d m_orb2arm = getm(p3d(0,0,0), iarm[0][0].p[7].e);                    //相机坐标系转机械臂坐标系的矩阵   //
//    for (int i = 0; i < workmap.size(); ++i)
//    {
//        workmap_arm << m_orb2arm * workmap[i];
//    }
}

void MyWindow::stop()
{
    MovJ(-160, -775.3, 291.8, -178, 0, 179.5);
    qsleep_pause(17000);
    MovJ(-20, -376, 1134, -90, 2, 179.5);
    qsleep_pause(17000);
    sendodr("DisableRobot()");
}

void MyWindow::setip(QString ip)
{
    this->ip = ip;
}

void MyWindow::qsleep(int msec)
{
    QTimer t;
    t.setInterval(msec);
    t.start();
    QEventLoop loop;
    connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    loop.exec();
}

void MyWindow::qsleep_pause(int msec)
{
    qsleep(msec);
    while(pausebit)
    {
        qsleep(100);
    }
}

void MyWindow::poweron()
{
    pw = 1;
    //sendodr("PowerOn()");
    //qsleep(10000);
    sendodr("DisableRobot");
    sendodr("EnableRobot(1.5,0,0,0)");
    //sendodr("BrakeControl(1,1)");
    sendodr("SpeedFactor(20)");
    sendodr("RobotMode()");
    pw = 0;
}
std::vector<cv::Point3d> MyWindow::convert_camera2arm(std::vector<cv::Point3d> pointsC)
{
    std::vector<cv::Point3d> tmp_points;
    for (uint i = 0; i < pointsC.size(); ++i) {
        tmp_points.push_back(cv::Point3d( start_Camera_Point.x - pointsC[i].x,
                                          start_Camera_Point.y + pointsC[i].y,
                                          start_Camera_Point.z - pointsC[i].z));
    }
    return tmp_points;
}

void MyWindow::getImage(/*std::vector<cv::Point3d>& points,cv::Mat& colorRawMat*/)
{
    poweron();
    qsleep_pause(2000);
    MovJ(-20, -376, 1134, -90, 2, 179.5);
    qsleep_pause(25000);
    MovJ(start_Camera_Point.x, start_Camera_Point.y, start_Camera_Point.z, 179.8, -0.1425, 89.8);
    qsleep_pause(20000);

    std::vector<OBColorPoint> pointCloud_frame_data;
    obCapture(colorRawMat,pointCloud_frame_data);
    cv::imwrite("colorRawMat.jpg", colorRawMat);
    std::string path = "E://workspace//PhysicalTherapyRobot//x64//Release//yolov8_640_640_v15.onnx";
    cv::Mat colorRawMatR = colorRawMat.t();
    cv::rotate(colorRawMat, colorRawMatR, cv::ROTATE_90_CLOCKWISE);
    std::vector<cv::Point> base0 = detect(colorRawMatR, path/*u8"debug/yolov8_640_640_v15.onnx"*/);
    std::vector<cv::Point> base;
    for (uint i = 0; i < base0.size(); ++i) {
        base.push_back(cv::Point(base0[i].y, colorRawMat.rows - base0[i].x));
    }
    std::vector<cv::Point3d> pointsC = get3Dpoints(base, pointCloud_frame_data) /* *m */;   //需要一个变换矩阵m
    points = convert_camera2arm(pointsC);
    //todo: 点的顺序  //
    qDebug() << "points: " ;
    for (uint i = 0; i < points.size(); ++i) {
        qDebug() << i << points[i].x << points[i].y << points[i].z;

        cv::circle(colorRawMat, base[i], 2, cv::Scalar(125, 123, 0), 3);
        std::string msg = std::to_string(i) + " ("
                + std::to_string(int(points[i].x)) + ", "
                + std::to_string(int(points[i].y)) + ", "
                + std::to_string(int(points[i].z)) + ")";
        cv::putText(colorRawMat,
                    msg,
                    base[i] + cv::Point(10, 10), 1, 0.6, cv::Scalar(255, 255), 1);

        std::string msg2 = std::to_string(i) + " ("
                + std::to_string(int(base[i].x)) + ", "
                + std::to_string(int(base[i].y)) + ")";

        cv::putText(colorRawMat,
                    msg2,
                    base[i] + cv::Point(10, 20), 1, 0.6, cv::Scalar(255), 1);
    }
    cv::namedWindow("colorRawMat", cv::WINDOW_AUTOSIZE);
    cv::imshow("colorRawMat", colorRawMat);
}

std::vector<cv::Point3d> MyWindow::get3Dpoints (std::vector<cv::Point> base,std::vector<OBColorPoint> pointCloud_frame_data)
{
//    std::vector<cv::Point3d> points;
//    for (int i = 0; i < 720 * 1280; ++i)
//    {
//        OBColorPoint* pointA = Colorpoint + i;
//        qDebug() << i << ": " << pointA->x << ", " << pointA->y << ", " << pointA->z
//                     << ", " << pointA->r << ", " << pointA->g << ", " << pointA->b;
//    }

    qDebug()<<"Current vector size is "<< pointCloud_frame_data.size();
    for (int i = 0; i < 100 ;++i)
    {
        OBColorPoint& pointA = pointCloud_frame_data[i];
        qDebug() << i << ": " << pointA.x << ", " << pointA.y << ", " << pointA.z
                             << ", " << pointA.r << ", " << pointA.g << ", " << pointA.b;
    }

    for (uint i = 0; i < base.size(); ++i)
    {
        OBColorPoint& pointA = pointCloud_frame_data[int(base[i].y) * imageWidth + int(base[i].x)] ;                                     //宽0->1279，高0-719
        points.push_back(cv::Point3d(double(pointA.x), double(pointA.y), double(pointA.z)));
    }
    return points;
}
