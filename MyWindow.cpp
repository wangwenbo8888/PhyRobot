#include "MyWindow.h"
#include "PhysicalTherapyRobot.h"

#include "Communicate.h"

#include <QTime>
#include <QStringLiteral>

#include "AdmittanceControl.h"

#include <iostream>
#include <string>
#include <vector>
#include <regex>

#include <QDir>
#include <QString>
#include <QMessageBox>

#include "PCLAlgo.h"

MyWindow::MyWindow(PhysicalTherapyRobot* robot, QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	m_bStoped = false;
	m_eTreatType = Treat_Unknown;

	m_pAccountInfo = new AccountInfo(this);

	m_pTimer = new QTimer(this);

	m_pTimer->setInterval(1000);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(On_timeout()));
	m_pTimer->start();

	m_pContactTimer = new QTimer(this);
	m_pContactTimer->setInterval(1500);
	m_pContactTimer->setSingleShot(true);
	connect(m_pContactTimer, SIGNAL(timeout()), this, SLOT(On_ContactTimeOut()));

	m_pWifiWindow.reset(new WifiList());

	m_pRobot = robot;
	//disconnect(ui.pushButton_Exit, SIGNAL(clicked()), this, SLOT(On_PushButton_Exit_Clicked()));
	//connect(ui.pushButton_Exit,SIGNAL(clicked()),this,SLOT(On_PushButton_Exit_Clicked()));

	//disconnect(ui.pushButton_MainFrame, SIGNAL(clicked()),this,SLOT(On_pushButton_MainFrame_Clicked()));
	//connect(ui.pushButton_MainFrame, SIGNAL(clicked()), this, SLOT(On_pushButton_MainFrame_Clicked()));

	//disconnect(ui.pushButton_AutoTreat,SIGNAL(clicked()),this,SLOT(On_pushButton_AutoTreat_Clicked()));
	//connect(ui.pushButton_AutoTreat, SIGNAL(clicked()), this, SLOT(On_pushButton_AutoTreat_Clicked()));

	//disconnect(ui.pushButton_IntensiveTreat,SIGNAL(clicked()),this,SLOT(On_pushButton_IntensiveTreat_Clicked()));
	//connect(ui.pushButton_IntensiveTreat, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensiveTreat_Clicked()));

	//disconnect(ui.pushButton_FinishOrganize, SIGNAL(clicked()), this, SLOT(On_pushButton_FinishOrganize_Clicked()));
	//connect(ui.pushButton_FinishOrganize, SIGNAL(clicked()), this, SLOT(On_pushButton_FinishOrganize_Clicked()));

	//disconnect(ui.pushButton_AccountManage,SIGNAL(clicked()),this,SLOT(On_pushButton_AccountManage_Clicked()));
	//connect(ui.pushButton_AccountManage, SIGNAL(clicked()), this, SLOT(On_pushButton_AccountManage_Clicked()));

	//disconnect(ui.pushButton_EquipInfo,SIGNAL(clicked()),this,SLOT(On_pushButton_EquipInfo_Clicked()));
	//connect(ui.pushButton_EquipInfo, SIGNAL(clicked()), this, SLOT(On_pushButton_EquipInfo_Clicked()));

	//disconnect(ui.pushButton_SetUp,SIGNAL(clicked()),this,SLOT(On_pushButton_SetUp_Clicked()));
	//connect(ui.pushButton_SetUp, SIGNAL(clicked()), this, SLOT(On_pushButton_SetUp_Clicked()));

	disconnect(ui.pushButton_Account, SIGNAL(clicked()), this, SLOT(On_pushButton_Account_Clicked()));
	connect(ui.pushButton_Account, SIGNAL(clicked()), this, SLOT(On_pushButton_Account_Clicked()));

	disconnect(ui.pushButton_Wifi, SIGNAL(clicked()), this, SLOT(On_pushButton_Wifi_Clicked()));
	connect(ui.pushButton_Wifi, SIGNAL(clicked()), this, SLOT(On_pushButton_Wifi_Clicked()));

	disconnect(ui.pushButton_ClearError, SIGNAL(clicked()), this, SLOT(On_pushButton_ClearError_Clicked()));
	connect(ui.pushButton_ClearError, SIGNAL(clicked()), this, SLOT(On_pushButton_ClearError_Clicked()));

	m_pHomePage.reset(new HomePage(this));
	ui.stackedWidget_Pags->addWidget(m_pHomePage.get());

	m_pTreatInstruction.reset(new TreatInstruction(this));
	ui.stackedWidget_Pags->addWidget(m_pTreatInstruction.get());

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

	//ui.pushButton_MainFrame->click();
	ui.stackedWidget_Pags->setCurrentWidget(m_pHomePage.get());

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

	//std::string modelPath = "last_0923.onnx";  // 替换为你的模型路径
	std::string modelPath = "last1021.onnx";
	m_net = cv::dnn::readNet(modelPath);
	m_net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
	m_net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

	// 检查是否加载成功
	if (m_net.empty())
	{
		std::cerr << "Failed to load model: " << modelPath << std::endl;
	}

	AdmittanceParams params;
	params.M = Eigen::MatrixXd::Identity(6, 6) * 10; // 质量矩阵，这里设置为对角矩阵并乘以10
	params.D = Eigen::MatrixXd::Identity(6, 6) * 1.0; // 阻尼矩阵，这里设置为对角矩阵并乘以0.5
	params.K.setZero(6, 6); // 刚度矩阵设置为零（纯导纳控制）

	// 控制周期
	double dt = 0.01; // 10ms

	// 创建导纳控制器实例
	m_pAddmittance = new AdmittanceController(params, dt, this);
	m_PreForceDir = Point3D(0.0, 0.0, -1.0);

	m_pCommunicate = new Communicate();
	qDebug() << "My window communicate is " << m_pCommunicate;

	m_pCommunicate->openPort();

	disconnect(m_pCommunicate, SIGNAL(sendState(CONTACT_STATE)), this, SLOT(On_received_contact_state(CONTACT_STATE)));
	connect(m_pCommunicate, SIGNAL(sendState(CONTACT_STATE)), this, SLOT(On_received_contact_state(CONTACT_STATE)));

	MyWindow_connect();

	poweron();
	//JointMovJ(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	RobotStorage();
	Wait_Done();
}

MyWindow::~MyWindow()
{
	// m_pTimer->destroyed();

	if (m_pAddmittance != NULL)
	{
		delete m_pAddmittance;
		m_pAddmittance = NULL;
	}

	delete m_pCommunicate;  // 此时 worker 已在主线程（因为 wait() 后 moveToThread 不生效？）
}

Communicate* MyWindow::GetCommunicate()
{
	return m_pCommunicate;
}

void MyWindow::RobotGoHome()
{
	//JointMovJ(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	RobotStorage();
	Wait_Done();
}

void MyWindow::On_PushButton_Exit_Clicked()
{
	this->close();
	m_pRobot->close();
}

void MyWindow::On_pushButton_MainFrame_Clicked()
{
	//ui.pushButton_MainFrame->setDown(true);
	//ui.pushButton_AutoTreat->setDown(false);
	//ui.pushButton_IntensiveTreat->setDown(false);
	//ui.pushButton_FinishOrganize->setDown(false);
	//ui.pushButton_AccountManage->setDown(false);
	//ui.pushButton_EquipInfo->setDown(false);
	//ui.pushButton_SetUp->setDown(false);

	ui.stackedWidget_Pags->setCurrentWidget(m_pHomePage.get());
}

void MyWindow::SetWidgetInstruction(TreatType type)
{
	m_eTreatType = type;
	ui.stackedWidget_Pags->setCurrentWidget(m_pTreatInstruction.get());
}

void MyWindow::On_pushButton_AutoTreat_Clicked()
{
	//ui.pushButton_MainFrame->setDown(false);
	//ui.pushButton_AutoTreat->setDown(true);
	//ui.pushButton_IntensiveTreat->setDown(false);
	//ui.pushButton_FinishOrganize->setDown(false);
	//ui.pushButton_AccountManage->setDown(false);
	//ui.pushButton_EquipInfo->setDown(false);
	//ui.pushButton_SetUp->setDown(false);

	m_pAutoTreat->SetWidgetInstruction();
	ui.stackedWidget_Pags->setCurrentWidget(m_pAutoTreat.get());
}

void MyWindow::On_pushButton_IntensiveTreat_Clicked()
{
	//ui.pushButton_MainFrame->setDown(false);
	//ui.pushButton_AutoTreat->setDown(false);
	//ui.pushButton_IntensiveTreat->setDown(true);
	//ui.pushButton_FinishOrganize->setDown(false);
	//ui.pushButton_AccountManage->setDown(false);
	//ui.pushButton_EquipInfo->setDown(false);
	//ui.pushButton_SetUp->setDown(false);

	m_pIntensiveTreat->SetWidgetPalliativeCare();
	ui.stackedWidget_Pags->setCurrentWidget(m_pIntensiveTreat.get());
}

void MyWindow::On_pushButton_FinishOrganize_Clicked()
{
	//ui.pushButton_MainFrame->setDown(false);
	//ui.pushButton_AutoTreat->setDown(false);
	//ui.pushButton_IntensiveTreat->setDown(false);
	//ui.pushButton_FinishOrganize->setDown(true);
	//ui.pushButton_AccountManage->setDown(false);
	//ui.pushButton_EquipInfo->setDown(false);
	//ui.pushButton_SetUp->setDown(false);

	m_pFinishOrganize->InitState();
	ui.stackedWidget_Pags->setCurrentWidget(m_pFinishOrganize.get());
}

void MyWindow::On_pushButton_AccountManage_Clicked()
{
	//ui.pushButton_MainFrame->setDown(false);
	//ui.pushButton_AutoTreat->setDown(false);
	//ui.pushButton_IntensiveTreat->setDown(false);
	//ui.pushButton_FinishOrganize->setDown(false);
	//ui.pushButton_AccountManage->setDown(true);
	//ui.pushButton_EquipInfo->setDown(false);
	//ui.pushButton_SetUp->setDown(false);

	ui.stackedWidget_Pags->setCurrentWidget(m_pAccoutManager.get());
}

void MyWindow::On_pushButton_EquipInfo_Clicked()
{
	//ui.pushButton_MainFrame->setDown(false);
	//ui.pushButton_AutoTreat->setDown(false);
	//ui.pushButton_IntensiveTreat->setDown(false);
	//ui.pushButton_FinishOrganize->setDown(false);
	//ui.pushButton_AccountManage->setDown(false);
	//ui.pushButton_EquipInfo->setDown(true);
	//ui.pushButton_SetUp->setDown(false);

	ui.stackedWidget_Pags->setCurrentWidget(m_pEquipInfo.get());
}

void MyWindow::On_pushButton_SetUp_Clicked()
{
	//ui.pushButton_MainFrame->setDown(false);
	//ui.pushButton_AutoTreat->setDown(false);
	//ui.pushButton_IntensiveTreat->setDown(false);
	//ui.pushButton_FinishOrganize->setDown(false);
	//ui.pushButton_AccountManage->setDown(false);
	//ui.pushButton_EquipInfo->setDown(false);
	//ui.pushButton_SetUp->setDown(true);

	ui.stackedWidget_Pags->setCurrentWidget(m_pSetUp.get());
}
void MyWindow::On_pushButton_Wifi_Clicked()
{
	AppInit::Instance()->start();
	m_pWifiWindow->show();
}

void MyWindow::On_pushButton_ClearError_Clicked()
{
	ClearError();

	// 发生碰撞，清除计划穴位
	for (int j = 0; j < m_vXueweis.size(); ++j)
	{
		m_vXueweis[j].clear();
	}
}

void MyWindow::On_pushButton_Account_Clicked()
{
	m_pAccountInfo->show();
}

void MyWindow::On_received_contact_state(CONTACT_STATE state)
{
	if (state == CONTACT_OK)
	{
		ui.label_State->setPixmap(QPixmap(":/Contact_OK.png"));
	}
	else if (state == CONTACT_ERROR)
	{
		ui.label_State->setPixmap(QPixmap(":/Contact_Error.png"));

		if (m_pContactTimer->isActive())
		{
			m_pContactTimer->stop();
		}
		m_pContactTimer->start();
	}
}

void MyWindow::On_ContactTimeOut()
{
	m_pContactTimer->stop();
	ui.label_State->setPixmap(QPixmap(":/Contact_OK.png"));
}

void MyWindow::On_timeout()
{
	QTime currentTime = QTime::currentTime();
	QString timeString = currentTime.toString("hh:mm");
	ui.label_Time->setText(QObject::tr(timeString.toStdString().c_str()));

	// 获取当前日期
	QDate currentDate = QDate::currentDate();

	// 格式化输出日期
	QString dateString = currentDate.toString(QStringLiteral("MM月dd日"));

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

	//text.append(" ");
	//text.append(timeString);
	ui.label_Date->setText(QObject::tr(text.toStdString().c_str()));
}

void MyWindow::SetWidgetHomePage()
{
	ui.stackedWidget_Pags->setCurrentWidget(m_pHomePage.get());
}

void MyWindow::SetWidgetSetUp()
{
	ui.stackedWidget_Pags->setCurrentWidget(m_pSetUp.get());
}

void MyWindow::SetWidgetAfterInstruction()
{
	if (m_eTreatType == Treat_Auto)
	{
		m_pAutoTreat->SetWidgetPrePrepare();
		ui.stackedWidget_Pags->setCurrentWidget(m_pAutoTreat.get());
	}
	else if (m_eTreatType == Treat_Manual)
	{
		m_pIntensiveTreat->SetWidgetAcupointRecog();
		ui.stackedWidget_Pags->setCurrentWidget(m_pIntensiveTreat.get());
	}
	else
	{

	}
}

void MyWindow::MyWindow_connect()
{
	od.init(ip);
	qDebug() << "Connect to " << od.server << endl;
}

void MyWindow::pause()
{
	sendrunodr("Pause()");
	//pausebit = 1;
}

void MyWindow::WorkContinue()
{
	sendrunodr("Continue()");
	//pausebit = 0;
}

void MyWindow::sktDashboard_connected()
{
	cnt[pDashboard] = 1;
	qDebug() << "sktDashboard_connected" << endl;
}

std::vector<std::string> extractContent(const std::string& input)
{
	std::vector<std::string> contents;
	try {
		// 定义正则表达式
		std::regex pattern(R"(\{([^}]*)\})");
		std::smatch match;

		// 查找匹配项
		std::string temp = input;
		while (std::regex_search(temp, match, pattern)) {
			// 获取括号内的内容（第一组捕获）
			contents.push_back(match[1].str());
			// 更新字符串以继续查找下一个匹配项
			temp = match.suffix().str();
		}
	}
	catch (const std::regex_error& e) {
		std::cerr << "Regex error: " << e.what() << std::endl;
	}
	return contents;
}

std::vector<std::string> split(const std::string& str, char delimiter)
{
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string token;
	while (std::getline(ss, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}

std::vector<std::vector<std::string>> extractAndSplit(const std::string& input, char delimiter)
{
	std::vector<std::vector<std::string>> results;
	std::vector<std::string> rawContents = extractContent(input);

	for (const auto& content : rawContents)
	{
		results.push_back(split(content, delimiter));
	}

	return results;
}

void MyWindow::sktDashboard_readyRead()
{
	qDebug() << "sktDashboard_readyRead" << endl;
	QByteArray msg = od.sktDashboard->readAll();
	QString str(msg);

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
	QString str(msg);

	return;
	std::vector<std::vector<std::string>> numbers;
	if (str.contains("GetSixForceData()"))
	{
		numbers = extractAndSplit(str.toStdString(), ',');

		if (!numbers.empty())
		{
			std::vector<std::string> forces;
			for (int i = 0; i < numbers.size(); ++i)
			{
				if (numbers[i].size() == 9)
				{
					forces = numbers[i];
				}
			}

			m_vForces.clear();
			for (int i = 0; i < forces.size(); ++i)
			{
				double value = strToDouble(forces[i]);
				m_vForces.push_back(value);
				//qDebug() << "value is " << value << endl;
			}

			//if (Forces.size()==9)
			//{
			//    Eigen::VectorXd force_feedback(6);
			//    force_feedback << Forces[0], Forces[1], Forces[2], Forces[3], Forces[4], Forces[5];
			//    m_pAddmittance->update(force_feedback);
			//    Wait_Done();
			//}

			//GetSixForceData();
		}
	}
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
	//qDebug() <<"Msg 8 is " << msg << endl;

	// --- 核心提取逻辑 ---
	qint64 start_pos = 1304;
	qint64 end_pos = 1351;

	// **重要：边界检查**
	if (start_pos < 0 || end_pos >= msg.size() || start_pos > end_pos) {
		qWarning() << "Error out of range ！";
		return;
	}

	// 计算起始位置和要提取的长度
	qint64 length = end_pos - start_pos + 1; // 1351 - 1304 + 1 = 48 字节

	// 使用 mid() 函数提取数据
	// mid(start_pos, length) 从 start_pos 开始，提取 length 个字节
	QByteArray extractedData = msg.mid(start_pos, length);

	// --- 输出结果 ---
	//qDebug() << "Exect success " << extractedData.size() << " byte !";
	// 存储转换结果的容器
	std::vector<double> doubleValues;

	// 使用 QDataStream 从 QByteArray 读取数据
	QDataStream stream(&extractedData, QIODevice::ReadOnly);
	stream.setByteOrder(QDataStream::LittleEndian); // 明确设置为小端模式

	// 读取 6 个 double 值
	for (int i = 0; i < 6; ++i) {
		double value;
		stream >> value; // QDataStream 会自动按小端格式读取 8 字节并转换为 double
		doubleValues.push_back(value);
	}

	// qDebug() << "Real time force is " << doubleValues[0]<<" "<< doubleValues[1]<<" "<< doubleValues[2]<<" " << doubleValues[3]<<" "<< doubleValues[4]<<" "<<doubleValues[5];

	m_vForces.clear();
	m_vForces = doubleValues;
	//qDebug() << "sktwork_readyRead" << endl;
	//QByteArray msg = od.sktwork->readAll();
	//qDebug() << msg << endl;
#if 0
	QString str(msg);

	std::vector<std::vector<std::string>> numbers;
	if (str.contains("GetSixForceData()"))
	{
		numbers = extractAndSplit(str.toStdString(), ',');

		if (!numbers.empty())
		{
			std::vector<std::string> forces;
			for (int i = 0; i < numbers.size(); ++i)
			{
				if (numbers[i].size() == 9)
				{
					forces = numbers[i];
				}
			}

			m_vForces.clear();
			for (int i = 0; i < forces.size(); ++i)
			{
				double value = strToDouble(forces[i]);
				m_vForces.push_back(value);
				//qDebug() << "value is " << value << endl;
			}
		}
	}

#endif
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
	//qDebug() << int(msg[24]) << endl;
	RobotMode = msg[24];
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
	if (cnt[pDashboard])
	{
		od.sktDashboard->write(odr);
		qDebug() << "Send Dash board oder: " << odr;
	}
	qsleep(2000);
}


void MyWindow::sendrunodr(QByteArray odr)
{
	if (cnt[pwork])
	{
		od.sktwork->write(odr);
		qDebug() << "Send run oder: " << odr;
	}
}

// 开始机械臂拖拽模式
void MyWindow::StartDrag()
{
	sendrunodr("StartDrag()");

	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_time = current_date_time.toString("hh:mm:ss.zzz");
	qDebug() << "send start drag time is " << current_time << endl;
}
void MyWindow::RobotStorage()
{
	JointMovJ(90.0, 0.0, 159.0, -60.0, -90.0, 250.0);
	Wait_Done();
}

void MyWindow::ClearError()
{
	sendodr("ClearError()");
	// 按摩头重量 20250721 0.5是公斤
	sendodr("EnableRobot(0.5,0,0,0)");
	//JointMovJ(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	RobotStorage();
	Wait_Done();
}

// 停止机械臂拖拽模式
void MyWindow::StopDrag()
{
	sendrunodr("StopDrag()");

	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_time = current_date_time.toString("hh:mm:ss.zzz");
	qDebug() << "send stop drag time is " << current_time << endl;
}

void MyWindow::GetPose()
{
	sendrunodr("GetPose()");

	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_time = current_date_time.toString("hh:mm:ss.zzz");
	qDebug() << "Get pose send time is " << current_time << endl;
}

void MyWindow::GetSixForceData()
{
	//sendodr("GetSixForceData()");
	sendrunodr("GetSixForceData()");

	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_time = current_date_time.toString("hh:mm:ss.zzz");
	qDebug() << "send time is " << current_time << endl;
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
	if (isJointNear)
	{
		sendodr("InverseSolution(" + QByteArray::number(X) + "," + QByteArray::number(Y) + "," + QByteArray::number(Z) + "," +
			QByteArray::number(Rx) + "," + QByteArray::number(Ry) + "," + QByteArray::number(Rz) + "," +
			QByteArray::number(User) + "," + QByteArray::number(Tool) + "," +
			QByteArray::number(isJointNear) + "," + JointNear.toLatin1() + ")");
	}
	else {
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

void MyWindow::JointMovJ(double J1, double J2, double J3, double J4, double J5, double J6)
{
	sendrunodr("JointMovJ(" + QByteArray::number(J1) + "," + QByteArray::number(J2) + "," + QByteArray::number(J3) + "," +
		QByteArray::number(J4) + "," + QByteArray::number(J5) + "," + QByteArray::number(J6) + ")");
}

void MyWindow::Sync()
{
	sendrunodr("Sync()");
}

void MyWindow::Tool(int tool)
{
	sendrunodr("Tool(" + QByteArray::number(tool) + ")");
}

void MyWindow::ServoP(double X, double Y, double Z, double Rx, double Ry, double Rz)
{
	sendrunodr("ServoP(" + QByteArray::number(X) + "," + QByteArray::number(Y) + "," + QByteArray::number(Z) + "," +
		QByteArray::number(Rx) + "," + QByteArray::number(Ry) + "," + QByteArray::number(Rz) + ")");
}
void MyWindow::MovL(double X, double Y, double Z, double Rx, double Ry, double Rz)
{
	sendrunodr("MovL(" + QByteArray::number(X) + "," + QByteArray::number(Y) + "," + QByteArray::number(Z) + "," +
		QByteArray::number(Rx) + "," + QByteArray::number(Ry) + "," + QByteArray::number(Rz) + ")");
}

void MyWindow::MovJ(double X, double Y, double Z, double Rx, double Ry, double Rz)
{
	sendrunodr("MovJ(" + QByteArray::number(X) + "," + QByteArray::number(Y) + "," + QByteArray::number(Z) + "," +
		QByteArray::number(Rx) + "," + QByteArray::number(Ry) + "," + QByteArray::number(Rz) + ")");
}

void MyWindow::MovJInterface(double x, double y, double z, double Rx, double Ry, double Rz)
{
	MovJ(x, y, z, Rx, Ry, Rz);
}

bool MyWindow::AdmittanceControlNew(int group, int row)
{
	if (m_bStoped)
	{
		return true;
	}

	int circle_time = 33;
	// 设置导纳控制参数
	//m_pAddmittance->setPos(pos);
	// 模拟主循环
	//GetSixForceData();
	qsleep(15);
	//Wait_ForForces();
	//m_vRawForces = m_vForces;

	double maxForce = 20.0;
	double midForce = 10.0;
	double touchForce = 5.0;

	m_CurrForceDir = Point3D(m_vForces[0] - m_vRawForces[0], m_vForces[1] - m_vRawForces[1], m_vForces[2] - m_vRawForces[2]);
	qDebug() << "Base force is " << m_vRawForces[0] << " " << m_vRawForces[1] << " " << m_vRawForces[2];
	qDebug() << "Detected force is " << m_vForces[0] << " " << m_vForces[1] << " " << m_vForces[2];
	qDebug() << "Pure force is " << m_CurrForceDir.x() << " " << m_CurrForceDir.y() << " " << m_CurrForceDir.z();
	double mag = m_CurrForceDir.Magnitude();
	m_CurrForceDir.Normalize();

	CRotation rot(m_PreForceDir, m_CurrForceDir);
	double y, p, r;
	rot.getYawPitchRoll(y, p, r);
	qDebug() << "Force normal is " << m_CurrForceDir.x() << " " << m_CurrForceDir.y() << " " << m_CurrForceDir.z();
	qDebug() << "Theata is " << y << " " << p << " " << r;
	qDebug() << "Mag is " << mag << group << " group " << row << " row !";
	if (mag > midForce && mag <= maxForce && fabs(m_CurrForceDir.z()) > 0.9)
	{
		m_vForces.clear();
		m_PreForceDir = Point3D(0.0, 0.0, -1.0);

		for (int i = 0; i < 6; ++i)
		{
			qDebug() << "Force is " << i << " " << m_vForces[i] - m_vRawForces[i];
		}

		return true;
	}
	else if (mag <= touchForce)
	{
		qDebug() << "Pos normal is " << m_vPosNormal[0] << " " << m_vPosNormal[1] << " " << m_vPosNormal[2];
		m_CurrForceDir = Point3D(m_vPosNormal[0], m_vPosNormal[1], m_vPosNormal[2]);
		m_vForces.clear();
		CRotation rot(m_PreForceDir, m_CurrForceDir);
		double y, p, r;
		rot.getYawPitchRoll(y, p, r);
		m_PreForceDir = m_CurrForceDir;

		GetPose();

		qDebug() << "Theata is " << y << " " << p << " " << r;
		qDebug() << "Aim angle is  " << m_vCurrentPos[3] - y << " " << m_vCurrentPos[4] - p << " " << m_vCurrentPos[5] - r;
		//MovJ(m_vCurrentPos[0], m_vCurrentPos[1], m_vCurrentPos[2] - 2.0,
		//    m_vCurrentPos[3]-y, m_vCurrentPos[4]-p, m_vCurrentPos[5]-r);
		ServoP(m_vCurrentPos[0] + 1.0 * m_vPosNormal[0],
			m_vCurrentPos[1] + 1.0 * m_vPosNormal[1],
			m_vCurrentPos[2] + 1.0 * m_vPosNormal[2],
			m_vCurrentPos[3] - y, m_vCurrentPos[4] - p, m_vCurrentPos[5] - r);

		m_vCurrentPos[0] += 1.0 * m_vPosNormal[0];
		m_vCurrentPos[1] += 1.0 * m_vPosNormal[1];
		m_vCurrentPos[2] += 1.0 * m_vPosNormal[2];
		m_vCurrentPos[3] -= y;
		m_vCurrentPos[4] -= p;
		m_vCurrentPos[5] -= r;
		//Wait_Done();
		// 
		qsleep(circle_time);
		//Wait_ForMove();             

		return false;
	}
	else if (mag > touchForce && mag <= midForce)
	{
		m_vForces.clear();
		m_PreForceDir = m_CurrForceDir;

		//MovJ(m_vCurrentPos[0], m_vCurrentPos[1], m_vCurrentPos[2] - 0.5,
		//    m_vCurrentPos[3]-y, m_vCurrentPos[4]-p, m_vCurrentPos[5]-r);

		ServoP(m_vCurrentPos[0], m_vCurrentPos[1], m_vCurrentPos[2] - 0.2,
			m_vCurrentPos[3] - y, m_vCurrentPos[4] - p, m_vCurrentPos[5] - r);

		m_vCurrentPos[2] -= 0.2;
		m_vCurrentPos[3] -= y;
		m_vCurrentPos[4] -= p;
		m_vCurrentPos[5] -= r;

		//Wait_Done();
		qsleep(circle_time);
		//Wait_ForMove();

		return false;
	}
	else if (mag > maxForce)
	{
		m_vForces.clear();
		m_PreForceDir = m_CurrForceDir;

		//MovJ(m_vCurrentPos[0], m_vCurrentPos[1], m_vCurrentPos[2] + 0.5,
		//    m_vCurrentPos[3]-y, m_vCurrentPos[4]-p, m_vCurrentPos[5]-r);

		ServoP(m_vCurrentPos[0], m_vCurrentPos[1], m_vCurrentPos[2] + 0.2,
			m_vCurrentPos[3] - y, m_vCurrentPos[4] - p, m_vCurrentPos[5] - r);
		m_vCurrentPos[2] += 0.2;
		m_vCurrentPos[3] -= y;
		m_vCurrentPos[4] -= p;
		m_vCurrentPos[5] -= r;
		//Wait_Done();
		qsleep(circle_time);
		//Wait_ForMove();

		return false;
	}
}

bool MyWindow::AdmittanceControl()
{
	if (m_bStoped)
	{
		return true;
	}

	// 设置导纳控制参数
	//m_pAddmittance->setPos(pos);
	// 模拟主循环
	GetSixForceData();
	Wait_ForForces();
	//m_vRawForces = m_vForces;

	double maxForce = 15.0;
	double minForce = 10.0;
	if (fabs(m_vForces[2]) > minForce && fabs(m_vForces[2]) < maxForce)
	{
		m_vForces.clear();

		for (int i = 0; i < 6; ++i)
		{
			qDebug() << "Force is " << i << " " << m_vForces[i] - m_vRawForces[i];
		}

		return true;
	}
	else if (fabs(m_vForces[2]) < minForce)
	{
		m_vForces.clear();
		MovJ(m_vCurrentPos[0], m_vCurrentPos[1], m_vCurrentPos[2] - 0.5,
			m_vCurrentPos[3], m_vCurrentPos[4], m_vCurrentPos[5]);
		m_vCurrentPos[2] -= 0.5;
		//Wait_Done();
		Wait_ForMove();

		return false;
	}
	else if (fabs(m_vForces[2]) > maxForce)
	{
		m_vForces.clear();
		MovJ(m_vCurrentPos[0], m_vCurrentPos[1], m_vCurrentPos[2] + 0.5,
			m_vCurrentPos[3], m_vCurrentPos[4], m_vCurrentPos[5]);
		m_vCurrentPos[2] += 0.5;
		//Wait_Done();
		Wait_ForMove();

		return false;
	}
}

// 模拟获取六维力反馈的函数
Eigen::VectorXd MyWindow::getForceFeedback()
{
	// 这里应该调用实际的力传感器接口获取数据
	// 返回一个6维向量，包含三个线性分量和三个角分量
	GetSixForceData();

	Eigen::VectorXd force_feedback(6);

	while (true)
	{
		if (m_vForces.empty())
		{
			//std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1 * 1000)));
		}
		else
		{
			double a = m_vForces[0];
			double b = m_vForces[1];
			double c = m_vForces[2];
			double d = m_vForces[3];
			double e = m_vForces[4];
			double f = m_vForces[5];
			force_feedback << a, b, c, d, e, f;
			return force_feedback; // 示例中生成模拟数据
		}
	}
}

bool MyWindow::SetXuewei(const std::vector<std::vector<XUEWEI_INFO>>& xueweis)
{
	if (points.empty())
	{
		return false;
	}

	m_vXueweis.clear();
	for (int j = 0; j < xueweis.size(); ++j)
	{
		const std::vector<XUEWEI_INFO>& xuewei = xueweis[j];
		std::vector<RobotPoint> temp;
		for (int i = 0; i < xuewei.size(); ++i)
		{
			for (int k = 0; k < points.size(); ++k)
			{
				if (points[k].xuewei == xuewei[i].xuewei)
				{
					RobotPoint point = points[k];
					point.time = xuewei[i].time;
					point.intensity = xuewei[i].intensity;
					temp.push_back(point);
					break;
				}
			}
		}

		m_vXueweis.push_back(temp);
	}
}

void MyWindow::ResetRobot()
{
	sendodr("ResetRobot()");
	Wait_Done();
}


// 设置当前项目
void MyWindow::SetCurrentProj(QString str)
{
	m_pAutoTreat->SetCurrentProj(str);
}

void MyWindow::SetModel(OPENBACK_MODEL model)
{
	m_eModel = model;
}

// 步进模式
bool MyWindow::StepModel(PROTOCOL pro,int level)
{
	const double saft_hight = -50.0;
	// 获取初始校零减掉的力
	//GetSixForceData();
	//Wait_ForForces();
	bool bFirst = true;
	qsleep(15);
	m_vRawForces = m_vForces;

	for (int j = 0; j < m_vXueweis.size(); ++j)
	{
		for (int i = 0; i < m_vXueweis[j].size(); ++i)
		{
			if (m_bStoped)
			{
				return true;
			}

			cv::Point3d& p = m_vXueweis[j][i].p3d;
			cv::Point3d& n = m_vXueweis[j][i].n3d;
			//m_pAutoTreat->GetWidgetTreatOnGoing()->SetLabelTreating(i);

			// 防止撞击人体，分两步，先水平方向，再竖直方向
			sendodr("TCPSpeed(40)");
			MovL(p.x, p.y, saft_hight, -178, 0, NORMAL_ANGLE);   //联调     //
			//MovJ(points[i].x, points[i].y, 0.0, -178, 0, 179.5);   //联调     //
			Wait_Done();
			//MovJ(p.x, p.y, p.z, -178, 0, 179.5);   //联调     //
			MovL(p.x, p.y, p.z, -178, 0, NORMAL_ANGLE);   //联调     //
			//MovJ(points[i].x, points[i].y, 0.0, -178, 0, 179.5);   //联调     //
			Wait_Done();

			sendodr("TCPSpeedEnd()");

			Eigen::VectorXd pos(6);
			pos << p.x, p.y, p.z, -178, 0, NORMAL_ANGLE;
			m_vCurrentPos.clear();
			m_vCurrentPos.push_back(p.x);
			m_vCurrentPos.push_back(p.y);
			m_vCurrentPos.push_back(p.z);
			m_vCurrentPos.push_back(-178);
			m_vCurrentPos.push_back(0);
			m_vCurrentPos.push_back(NORMAL_ANGLE);

			qDebug() << "pos normal:" << n.x << ", " << n.y << ", " << n.z;

			m_vPosNormal.clear();
			m_vPosNormal.push_back(n.x);
			m_vPosNormal.push_back(n.y);
			m_vPosNormal.push_back(n.z);

			qDebug() << "Begin to: " << j << " group " << i << " pos " << m_vCurrentPos[0] << ", " << m_vCurrentPos[1] << ", " << m_vCurrentPos[2];
			while (!AdmittanceControlNew(j, i))
			{
				qDebug() << "Continue to: " << j << " group " << i << " pos " << m_vCurrentPos[0] << ", " << m_vCurrentPos[1] << ", " << m_vCurrentPos[2];
			}

			//while (!AdmittanceControl())
			//{
			//    qDebug() << "Move to:" << m_vCurrentPos[0] << ", " << m_vCurrentPos[1] << ", " << m_vCurrentPos[2];
			//}

			if (bFirst && pro == OPENBACK)
			{
				bFirst = false;
				QByteArray data;
				int begin = 1;
				int run = m_vXueweis[j][i].intensity;
				m_pCommunicate->setData(OPENBACK, begin, run, true, data);
				qDebug() << "Open back send message " << data.toHex().toUpper();
				m_pCommunicate->sendData(data);
				m_eCurrProto = OPENBACK;
				m_iCurrIntensity = run;
			}
			else if (pro == OPENLEG)
			{
				bFirst = false;
				QByteArray data;
				int begin = 1;
				int run = m_vXueweis[j][i].intensity;
				m_pCommunicate->setData(OPENLEG, begin, run, true, data);
				qDebug() << "Open leg send message " << data.toHex().toUpper();
				m_pCommunicate->sendData(data);
				m_eCurrProto = OPENLEG;
				m_iCurrIntensity = run;
			}
			else if (pro == LEGUNBLOCK_RIGHTLEG1 && j == 0 && i == 0)
			{
				//bFirst = false;
				QByteArray data;
				int begin = 1;
				int run = m_vXueweis[j][i].intensity;
				m_pCommunicate->setData(LEGUNBLOCK_RIGHTLEG1, begin, run, true, data);
				qDebug() << "Left unblock right1 send message " << data.toHex().toUpper();
				m_pCommunicate->sendData(data);
				m_eCurrProto = LEGUNBLOCK_RIGHTLEG1;
				m_iCurrIntensity = run;
			}
			else if (pro == LEGUNBLOCK_RIGHTLEG1 && j == 1 && i == 0)
			{
				//bFirst = false;
				QByteArray data;
				int begin = 1;
				int run = m_vXueweis[j][i].intensity;
				m_pCommunicate->setData(LEGUNBLOCK_LEFTLEG1, begin, run, true, data);
				qDebug() << "Leg unblock left1 send message " << data.toHex().toUpper();
				m_pCommunicate->sendData(data);
				m_eCurrProto = LEGUNBLOCK_LEFTLEG1;
				m_iCurrIntensity = run;
			}
			else if (pro == LEGUNBLOCK_RIGHTLEG1 && j == 2 && i == 0)
			{
				//bFirst = false;
				QByteArray data;
				int begin = 1;
				int run = m_vXueweis[j][i].intensity;
				m_pCommunicate->setData(LEGUNBLOCK_RIGHTLEG2, begin, run, true, data);
				qDebug() << "Leg unblock right2 send message " << data.toHex().toUpper();
				m_pCommunicate->sendData(data);
				m_eCurrProto = LEGUNBLOCK_RIGHTLEG2;
				m_iCurrIntensity = run;
			}
			else if (pro == LEGUNBLOCK_RIGHTLEG1 && j == 3 && i == 0)
			{
				//bFirst = false;
				QByteArray data;
				int begin = 1;
				int run = m_vXueweis[j][i].intensity;
				m_pCommunicate->setData(LEGUNBLOCK_LEFTLEG2, begin, run, true, data);
				qDebug() << "Leg unblock left2 send message " << data.toHex().toUpper();
				m_pCommunicate->sendData(data);
				m_eCurrProto = LEGUNBLOCK_LEFTLEG2;
				m_iCurrIntensity = run;
			}
			else if (bFirst && pro == SHOULDERUNBLOCK)
			{
				QByteArray data;
				int begin = 1;
				int run = m_vXueweis[j][i].intensity;
				m_pCommunicate->setData(SHOULDERUNBLOCK, begin, run, true, data);
				qDebug() << "Shoulder unblock send message " << data.toHex().toUpper();
				m_pCommunicate->sendData(data);
				m_eCurrProto = SHOULDERUNBLOCK;
				m_iCurrIntensity = run;
			}

			if (m_vXueweis[j][i].time > 0)
			{
				Wait_ForTreat(m_vXueweis[j][i].time * 1000 * DEBUG_TREAT_TIME);
			}

			emit FinishOneXuewei(m_vXueweis[j][i].time);

			if (m_eModel == MODEL_STEP)
			{
				//MovJ(p.x, p.y, p.z, -178, 0, 179.5);   //     //
				//MovJ(m_vCurrentPos[0], m_vCurrentPos[1], p.z, -178, 0, 179.5);   //     //
				MovL(m_vCurrentPos[0], m_vCurrentPos[1], saft_hight, -178, 0, NORMAL_ANGLE);   //     //
				Wait_Done();
			}

		}

		if (pro == LEGUNBLOCK_RIGHTLEG1 && j == 0)
		{
			//bFirst = false;
			QByteArray data;
			int begin = 1;
			int run = 1;
			m_pCommunicate->setData(LEGUNBLOCK_RIGHTLEG1, begin, run, false, data);
			qDebug() << "Left unblock right1 send message " << data.toHex().toUpper();
			m_pCommunicate->sendData(data);
		}
		else if (pro == LEGUNBLOCK_RIGHTLEG1 && j == 1)
		{
			//bFirst = false;
			QByteArray data;
			int begin = 1;
			int run = 1;
			m_pCommunicate->setData(LEGUNBLOCK_LEFTLEG1, begin, run, false, data);
			qDebug() << "Leg unblock left1 send message " << data.toHex().toUpper();
			m_pCommunicate->sendData(data);
		}
		else if (pro == LEGUNBLOCK_RIGHTLEG1 && j == 2)
		{
			//bFirst = false;
			QByteArray data;
			int begin = 1;
			int run = 1;
			m_pCommunicate->setData(LEGUNBLOCK_RIGHTLEG2, begin, run, false, data);
			qDebug() << "Leg unblock right2 send message " << data.toHex().toUpper();
			m_pCommunicate->sendData(data);
		}
		else if (pro == LEGUNBLOCK_RIGHTLEG1 && j == 3)
		{
			//bFirst = false;
			QByteArray data;
			int begin = 1;
			int run = 1;
			m_pCommunicate->setData(LEGUNBLOCK_LEFTLEG2, begin, run, false, data);
			qDebug() << "Leg unblock left2 send message " << data.toHex().toUpper();
			m_pCommunicate->sendData(data);
		}

		emit FinishOneGroup();
	}

	if (pro == OPENBACK)
	{
		QByteArray data;
		int begin = 1;
		int run = 1;
		m_pCommunicate->setData(OPENBACK, begin, run, false, data);
		qDebug() << "Open back end send message " << data.toHex().toUpper();
		m_pCommunicate->sendData(data);
	}
	else if (pro == OPENLEG)
	{
		QByteArray data;
		int begin = 1;
		int run = 1;
		m_pCommunicate->setData(OPENLEG, begin, run, false, data);
		qDebug() << "Open leg end send message " << data.toHex().toUpper();
		m_pCommunicate->sendData(data);
	}
	else if (bFirst && pro == SHOULDERUNBLOCK)
	{
		QByteArray data;
		int begin = 1;
		int run = 1;
		m_pCommunicate->setData(SHOULDERUNBLOCK, begin, run, false, data);
		qDebug() << "Shoulder unblock end send message  " << data.toHex().toUpper();
		m_pCommunicate->sendData(data);
	}

	return true;
}

bool MyWindow::GetNextAcupoint(int i, int j, DETECTED_XUEWEI& nextxuewei, std::vector<double>& point)
{
	if (m_vXueweis.size() <= i)
	{
		return false;
	}

	if (m_vXueweis[i].size() <= j)
	{
		return false;
	}

	point.clear();
	if (j + 1 < m_vXueweis[i].size())
	{
		point.push_back(m_vXueweis[i][j + 1].p3d.x);
		point.push_back(m_vXueweis[i][j + 1].p3d.y);
		point.push_back(m_vXueweis[i][j + 1].p3d.z);
		point.push_back(m_vXueweis[i][j + 1].n3d.x);
		point.push_back(m_vXueweis[i][j + 1].n3d.y);
		point.push_back(m_vXueweis[i][j + 1].n3d.z);

		nextxuewei = m_vXueweis[i][j + 1].xuewei;

		return true;
	}
	else if (j + 1 == m_vXueweis[i].size())
	{
		return false;
		if (i + 1 < m_vXueweis.size())
		{
			// 到下一行不是连续过去，跳跃过去
			return false;
			//point.push_back(m_vXueweis[i+1][0].p3d.x);
			//point.push_back(m_vXueweis[i+1][0].p3d.y);
			//point.push_back(m_vXueweis[i+1][0].p3d.z);
			//point.push_back(m_vXueweis[i+1][0].n3d.x);
			//point.push_back(m_vXueweis[i+1][0].n3d.y);
			//point.push_back(m_vXueweis[i+1][0].n3d.z);

			//return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool MyWindow::MoveToNextAcupointNew(int group, int row, DETECTED_XUEWEI currentxuewei,
	DETECTED_XUEWEI nextxuewei, std::vector<double>& next)
{
	// 此穴位第一次调节，设置成指定姿态
	bool bFirst = true;
	double maxForce = 20.0;
	double midForce = 10.0;
	double touchForce = 5.0;
	double moment = 0.1;
	//sendodr("TCPSpeed(40)");

	//double step = 8.0;
	double step = 4.0;
	double dis2D = sqrt((next[0] - m_vCurrentPos[0]) * (next[0] - m_vCurrentPos[0]) + (next[1] - m_vCurrentPos[1]) * (next[1] - m_vCurrentPos[1]));
	int divid = dis2D / step;
	double deltaZ = 0.0;
	double currentZ = m_vCurrentPos[2];
	double xRaw = m_vCurrentPos[0];
	double yRaw = m_vCurrentPos[1];
	for (int i = 1; i < divid; ++i)
	{
		double xPos = xRaw + (next[0] - xRaw) * i / divid;
		double yPos = yRaw + (next[1] - yRaw) * i / divid;
		currentZ += deltaZ;
		GetPose();

		//m_CurrForce = Point3D(m_vForces[0] - m_vRawForces[0], m_vForces[1] - m_vRawForces[1], m_vForces[2] - m_vRawForces[2]);
		double y = 0.0;
		double p = 0.0;
		double r = 0.0;
		if (m_CurrForce.x() > 10.0)
		{
			//y = 5.0;
			y = abs(m_CurrForce.x()) * 0.5;
		}
		else if (m_CurrForce.x() > 1.0)
		{
			//y = 2.0;
			y = abs(m_CurrForce.x()) * 0.5;
		}
		else if (m_CurrForce.x() < -10.0)
		{
			//y = -5.0;
			y = abs(m_CurrForce.x()) * -0.5;
		}
		else if (m_CurrForce.x() < -1.0)
		{
			//y = -2.0;
			y = abs(m_CurrForce.x()) * -0.5;
		}

		if (m_CurrForce.y() > 10.0)
		{
			//p = -5.0;
			p = abs(m_CurrForce.y()) * -0.5;
		}
		else if (m_CurrForce.y() > 1.0)
		{
			//p = -2.0;
			p = abs(m_CurrForce.y()) * -0.5;
		}
		else if (m_CurrForce.y() < -10.0)
		{
			//p = 5.0;
			p = abs(m_CurrForce.y()) * 0.5;
		}
		else if (m_CurrForce.y() < -1.0)
		{
			//p = 2.0;
			p = abs(m_CurrForce.y()) * 0.5;
		}

		double a = 0.0;
		double b = 0.0;
		/*        if (m_vForces[3]<-0.5)
				{
					a = -5.0;
				}
				else*/ if (m_vForces[3] < -0.05)
				{
					a = -20.0 * abs(m_vForces[3]);
				}
				/*        else if (m_vForces[3]>0.5)
						{
							a = 5.0;
						}
						else*/ if (m_vForces[3] > 0.05)
						{
							a = 10.0 * abs(m_vForces[3]);
						}

						/*        if (m_vForces[4] < -0.5)
								{
									b = 5.0;
								}
								else*/ if (m_vForces[4] < -0.05)
								{
									b = 20.0 * abs(m_vForces[4]);
								}
								/*        else if (m_vForces[4] > 0.5)
										{
											b = -5.0;
										}
										else*/ if (m_vForces[4] > 0.05)
										{
											b = -10.0 * abs(m_vForces[4]);
										}

										double xAngle = m_vCurrentPos[3] - y - a;
										double yAngle = m_vCurrentPos[4] - p - b;
										double zAngle = m_vCurrentPos[5] - r;

										if (xAngle < -200.0) // 190
										{
											xAngle = -200.0;
										}
										if (xAngle > -160.0) //170
										{
											xAngle = -160.0;
										}

										if (yAngle < -25.0) // -10
										{
											yAngle = -25.0;
										}
										if (yAngle > 25.0) // 10
										{
											yAngle = 25.0;
										}

										if (zAngle > 210.0) // 190
										{
											zAngle = 210.0;
										}
										if (zAngle < 150.0)
										{
											zAngle = 150.0;
										}

										// 到至阳，理疗头竖起来
										//if (currentxuewei == ZHIYANG_DETECTED
										//    && bFirst)
										//{
										//    xAngle = -178.0;
										//    yAngle = 0.0;
										//    bFirst = false;
										//}
										qDebug() << "Move to next: " << xPos << " " << yPos << " " << currentZ << " " << xAngle << " " << yAngle << " " << zAngle;
										//MovL(xPos,yPos,currentZ, -178, 0, 179.5);
										//ServoP(xPos, yPos, currentZ, xAngle,yAngle, NORMAL_ANGLE);
										MovL(xPos, yPos, currentZ, xAngle, yAngle, NORMAL_ANGLE);

										//Wait_ForShort();
										Wait_Done();
										m_vCurrentPos[0] = xPos;
										m_vCurrentPos[1] = yPos;
										m_vCurrentPos[2] = currentZ;
										m_vCurrentPos[3] = xAngle;
										m_vCurrentPos[4] = yAngle;
										m_vCurrentPos[5] = zAngle;

										//qsleep(310);
										//Wait_Done();
										m_CurrForce = Point3D(m_vForces[0] - m_vRawForces[0], m_vForces[1] - m_vRawForces[1], m_vForces[2] - m_vRawForces[2]);
										qDebug() << "Force mag is : " << m_CurrForce.Magnitude();
										qDebug() << "Detected force is : " << m_vForces[0] << " " << m_vForces[1] << " " << m_vForces[2] << " " << group << " " << row;
										if (m_CurrForce.Magnitude() > maxForce)
										{
											if (m_CurrForce.Magnitude() - maxForce > 20)
											{
												deltaZ = 9.0;
											}
											else if (m_CurrForce.Magnitude() - maxForce > 10)
											{
												deltaZ = 6.0;
											}
											else
											{
												deltaZ = 3.0;
											}
										}
										else if (m_CurrForce.Magnitude() < midForce)
										{
											if (midForce - m_PreForce.Magnitude() > 20)
											{
												deltaZ = -20.0;
											}
											else if (midForce - m_PreForce.Magnitude() > 10)
											{
												deltaZ = -6.0;
											}
											else
											{
												deltaZ = -3.0;
											}
										}

										m_PreForce = m_CurrForce;
	}
	// 刷新Z值
	next[2] = currentZ;

	//sendodr("TCPSpeedEnd()");
	qDebug() << "Finish from one acu to other !";

	return true;

}

bool MyWindow::MoveToNextAcupoint(int group, int row, std::vector<double>& next)
{
	double maxForce = 20.0;
	double midForce = 10.0;
	double touchForce = 5.0;
	//sendodr("TCPSpeed(40)");

	//double step = 8.0;
	double step = 8.0;
	double dis2D = sqrt((next[0] - m_vCurrentPos[0]) * (next[0] - m_vCurrentPos[0]) + (next[1] - m_vCurrentPos[1]) * (next[1] - m_vCurrentPos[1]));
	int divid = dis2D / step;
	double deltaZ = 0.0;
	double currentZ = m_vCurrentPos[2];
	double xRaw = m_vCurrentPos[0];
	double yRaw = m_vCurrentPos[1];
	for (int i = 1; i < divid; ++i)
	{
		double xPos = xRaw + (next[0] - xRaw) * i / divid;
		double yPos = yRaw + (next[1] - yRaw) * i / divid;
		currentZ += deltaZ;
		GetPose();

		//m_CurrForce = Point3D(m_vForces[0] - m_vRawForces[0], m_vForces[1] - m_vRawForces[1], m_vForces[2] - m_vRawForces[2]);
		m_CurrForceDir = m_CurrForce;
		m_CurrForceDir.Normalize();
		CRotation rot(m_PreForceDir, m_CurrForceDir);
		m_PreForceDir = m_CurrForceDir;
		double y, p, r;
		rot.getYawPitchRoll(y, p, r);

		double xAngle = m_vCurrentPos[3] - y;
		double yAngle = m_vCurrentPos[4] - p;
		double zAngle = m_vCurrentPos[5] - r;

		if (xAngle < -200.0) // 190
		{
			xAngle = -200.0;
		}
		if (xAngle > -160.0) //170
		{
			xAngle = -160.0;
		}

		if (yAngle < -20.0) // -10
		{
			yAngle = -20.0;
		}
		if (yAngle > 20.0) // 10
		{
			yAngle = 20.0;
		}

		if (zAngle > 210.0) // 190
		{
			zAngle = 210.0;
		}
		if (zAngle < 150.0)
		{
			zAngle = 150.0;
		}
		qDebug() << "Move to next: " << xPos << " " << yPos << " " << currentZ << " " << xAngle << " " << yAngle << " " << zAngle;
		//MovL(xPos,yPos,currentZ, -178, 0, 179.5);
		//ServoP(xPos, yPos, currentZ, -178, 0, 179.5);
		MovL(xPos, yPos, currentZ, xAngle, yAngle, NORMAL_ANGLE);

		//Wait_ForShort();
		Wait_Done();
		m_vCurrentPos[0] = xPos;
		m_vCurrentPos[1] = yPos;
		m_vCurrentPos[2] = currentZ;
		m_vCurrentPos[3] = xAngle;
		m_vCurrentPos[4] = yAngle;
		m_vCurrentPos[5] = zAngle;

		//qsleep(310);
		//Wait_Done();
		m_CurrForce = Point3D(m_vForces[0] - m_vRawForces[0], m_vForces[1] - m_vRawForces[1], m_vForces[2] - m_vRawForces[2]);
		qDebug() << "Force mag is : " << m_CurrForce.Magnitude();
		qDebug() << "Detected force is : " << m_vForces[0] << " " << m_vForces[1] << " " << m_vForces[2] << " " << group << " " << row;
		if (m_CurrForce.Magnitude() > maxForce)
		{
			if (m_CurrForce.Magnitude() - maxForce > 20)
			{
				deltaZ = 9.0;
			}
			else if (m_CurrForce.Magnitude() - maxForce > 10)
			{
				deltaZ = 6.0;
			}
			else
			{
				deltaZ = 3.0;
			}
		}
		else if (m_CurrForce.Magnitude() < midForce)
		{
			if (midForce - m_PreForce.Magnitude() > 20)
			{
				deltaZ = -20.0;
			}
			else if (midForce - m_PreForce.Magnitude() > 10)
			{
				deltaZ = -6.0;
			}
			else
			{
				deltaZ = -3.0;
			}
		}

		m_PreForce = m_CurrForce;
	}
	// 刷新Z值
	next[2] = currentZ;

	//sendodr("TCPSpeedEnd()");
	qDebug() << "Finish from one acu to other !";

	return true;
}

// 连续模式
bool MyWindow::ContinueModel(PROTOCOL pro,int level)
{
	const double saft_hight = -130.0;
	bool bFirstAcu = true;
	// 获取初始校零减掉的力
	//GetSixForceData();
	//Wait_ForForces();
	qsleep(15);
	m_vRawForces = m_vForces;
	std::vector<double> next;
	sendodr("TCPSpeed(10)");

	DETECTED_XUEWEI currentxuewei;
	for (int j = 0; j < m_vXueweis.size(); ++j)
	{
		for (int i = 0; i < m_vXueweis[j].size(); ++i)
		{
			if (m_bStoped)
			{
				return true;
			}

			if (bFirstAcu)
			{
				bFirstAcu = false;

				cv::Point3d& p = m_vXueweis[j][i].p3d;
				cv::Point3d& n = m_vXueweis[j][i].n3d;
				//m_pAutoTreat->GetWidgetTreatOnGoing()->SetLabelTreating(i);
				currentxuewei = m_vXueweis[j][i].xuewei;

				double xAngle = -178.0;
				double yAngle = 0.0;

				// 初始状态
				if (currentxuewei == FENGFU_DETECTED
					|| FENGFU_DETECTED == currentxuewei
					|| ZUOJIANJING_DETECTED == currentxuewei)
				{
					xAngle = -150.0;
					yAngle = 25.0;
				}
				else if (YOUJIANJING_DETECTED == currentxuewei)
				{
					xAngle = -150.0;
					yAngle = 10.0;
				}

				// 防止撞击人体，分两步，先水平方向，再竖直方向
				sendodr("TCPSpeed(40)");
				MovL(p.x, p.y, saft_hight, -178.0, 0.0, NORMAL_ANGLE);   //联调     //
				Wait_Done();
				MovL(p.x, p.y, p.z, xAngle, yAngle, NORMAL_ANGLE);   //联调     //
				//MovJ(points[i].x, points[i].y, 0.0, -178, 0, 179.5);   //联调     //
				Wait_Done();

				sendodr("TCPSpeedEnd()");

				Eigen::VectorXd pos(6);
				pos << p.x, p.y, p.z, xAngle, yAngle, NORMAL_ANGLE;
				m_vCurrentPos.clear();
				m_vCurrentPos.push_back(p.x);
				m_vCurrentPos.push_back(p.y);
				m_vCurrentPos.push_back(p.z);
				m_vCurrentPos.push_back(xAngle);
				m_vCurrentPos.push_back(yAngle);
				m_vCurrentPos.push_back(NORMAL_ANGLE);


				qDebug() << "pos normal:" << n.x << ", " << n.y << ", " << n.z;

				m_vPosNormal.clear();
				m_vPosNormal.push_back(n.x);
				m_vPosNormal.push_back(n.y);
				m_vPosNormal.push_back(n.z);

				qDebug() << "Begin to: " << j << " group " << i << " pos " << m_vCurrentPos[0] << ", " << m_vCurrentPos[1] << ", " << m_vCurrentPos[2];
				while (!AdmittanceControlNew(j, i))
				{
					qDebug() << "Continue to: " << j << " group " << i << " pos " << m_vCurrentPos[0] << ", " << m_vCurrentPos[1] << ", " << m_vCurrentPos[2];
				}

				if (pro == OPENBACK)
				{
					QByteArray data;
					int begin = 1;
					int run = m_vXueweis[j][i].intensity;
					m_pCommunicate->setData(OPENBACK, begin, run, true, data);
					qDebug() << "Open back send message " << data.toHex().toUpper();
					m_pCommunicate->sendData(data);
				}
				else if (pro == OPENLEG)
				{
					QByteArray data;
					int begin = 1;
					int run = m_vXueweis[j][i].intensity;
					m_pCommunicate->setData(OPENLEG, begin, run, true, data);
					qDebug() << "Open leg send message " << data.toHex().toUpper();
					m_pCommunicate->sendData(data);
				}
				else if (pro == LEGUNBLOCK_RIGHTLEG1 && j == 0 && i == 0)
				{
					//bFirst = false;
					QByteArray data;
					int begin = 1;
					int run = m_vXueweis[j][i].intensity;
					m_pCommunicate->setData(LEGUNBLOCK_RIGHTLEG1, begin, run, true, data);
					qDebug() << "Left unblock right1 send message " << data.toHex().toUpper();
					m_pCommunicate->sendData(data);
				}
				else if (pro == LEGUNBLOCK_RIGHTLEG1 && j == 1 && i == 0)
				{
					//bFirst = false;
					QByteArray data;
					int begin = 1;
					int run = m_vXueweis[j][i].intensity;
					m_pCommunicate->setData(LEGUNBLOCK_LEFTLEG1, begin, run, true, data);
					qDebug() << "Leg unblock left1 send message " << data.toHex().toUpper();
					m_pCommunicate->sendData(data);
				}
				else if (pro == LEGUNBLOCK_RIGHTLEG1 && j == 2 && i == 0)
				{
					//bFirst = false;
					QByteArray data;
					int begin = 1;
					int run = m_vXueweis[j][i].intensity;
					m_pCommunicate->setData(LEGUNBLOCK_RIGHTLEG2, begin, run, true, data);
					qDebug() << "Leg unblock right2 send message " << data.toHex().toUpper();
					m_pCommunicate->sendData(data);
				}
				else if (pro == LEGUNBLOCK_RIGHTLEG1 && j == 3 && i == 0)
				{
					//bFirst = false;
					QByteArray data;
					int begin = 1;
					int run = m_vXueweis[j][i].intensity;
					m_pCommunicate->setData(LEGUNBLOCK_LEFTLEG2, begin, run, true, data);
					qDebug() << "Leg unblock left2 send message " << data.toHex().toUpper();
					m_pCommunicate->sendData(data);
				}
				else if (pro == SHOULDERUNBLOCK)
				{
					QByteArray data;
					int begin = 1;
					int run = m_vXueweis[j][i].intensity;
					m_pCommunicate->setData(SHOULDERUNBLOCK, begin, run, true, data);
					qDebug() << "Shoulder unblock send message " << data.toHex().toUpper();
					m_pCommunicate->sendData(data);
				}

				sendodr("TCPSpeed(6)"); // 6
				m_PreForceDir = Point3D(n.x, n.y, n.z);

				// 设置界面上接触良好标志
				On_received_contact_state(CONTACT_OK);
			}
			else
			{

			}

			//while (!AdmittanceControl())
			//{
			//    qDebug() << "Move to:" << m_vCurrentPos[0] << ", " << m_vCurrentPos[1] << ", " << m_vCurrentPos[2];
			//}


			//Wait_Done();
			if (m_vXueweis[j][i].time > 0)
			{
				Wait_ForTreat(m_vXueweis[j][i].time * 1000 * DEBUG_TREAT_TIME);
			}

			emit FinishOneXuewei(m_vXueweis[j][i].time);

			DETECTED_XUEWEI nextxuewei;
			if (GetNextAcupoint(j, i, nextxuewei, next))
			{
				qDebug() << "From pos : " << j << " group " << i << " pos " << m_vCurrentPos[0] << " " << m_vCurrentPos[1] << " " << m_vCurrentPos[2] << " " << m_vCurrentPos[3] << " " << m_vCurrentPos[4] << " " << m_vCurrentPos[5];
				qDebug() << "Move to: " << j << " group " << i << " pos " << next[0] << " " << next[1] << " " << next[2] << " " << next[3] << " " << next[4] << " " << next[5];
				//MoveToNextAcupointNew(j, i, currentxuewei, nextxuewei, next);
				//MoveToNextAcupointNew_ImprovedV6(j, i, currentxuewei, nextxuewei, next,level);
				MoveToNextAcupointNew_ImprovedV7(j, i, currentxuewei, nextxuewei, next, level);
				m_vCurrentPos[0] = next[0];
				m_vCurrentPos[1] = next[1];
				m_vCurrentPos[2] = next[2];

				currentxuewei = nextxuewei;
			}
		}

		MovL(m_vCurrentPos[0], m_vCurrentPos[1], saft_hight, -178, 0, NORMAL_ANGLE);   //     //

		Wait_Done();

		if (pro == OPENBACK)
		{
			QByteArray data;
			int begin = 1;
			int run = 1;
			m_pCommunicate->setData(OPENBACK, begin, run, false, data);
			qDebug() << "Open back send message " << data.toHex().toUpper();
			m_pCommunicate->sendData(data);
		}
		else if (pro == OPENLEG)
		{
			QByteArray data;
			int begin = 1;
			int run = 1;
			m_pCommunicate->setData(OPENLEG, begin, run, false, data);
			qDebug() << "Open leg send message " << data.toHex().toUpper();
			m_pCommunicate->sendData(data);
		}
		else if (pro == LEGUNBLOCK_RIGHTLEG1 && j == 0)
		{
			//bFirst = false;
			QByteArray data;
			int begin = 1;
			int run = 1;
			m_pCommunicate->setData(LEGUNBLOCK_RIGHTLEG1, begin, run, false, data);
			qDebug() << "Left unblock right1 send message " << data.toHex().toUpper();
			m_pCommunicate->sendData(data);
		}
		else if (pro == LEGUNBLOCK_RIGHTLEG1 && j == 1)
		{
			//bFirst = false;
			QByteArray data;
			int begin = 1;
			int run = 1;
			m_pCommunicate->setData(LEGUNBLOCK_LEFTLEG1, begin, run, false, data);
			qDebug() << "Leg unblock left1 send message " << data.toHex().toUpper();
			m_pCommunicate->sendData(data);
		}
		else if (pro == LEGUNBLOCK_RIGHTLEG1 && j == 2)
		{
			//bFirst = false;
			QByteArray data;
			int begin = 1;
			int run = 1;
			m_pCommunicate->setData(LEGUNBLOCK_RIGHTLEG2, begin, run, false, data);
			qDebug() << "Leg unblock right2 send message " << data.toHex().toUpper();
			m_pCommunicate->sendData(data);
		}
		else if (pro == LEGUNBLOCK_RIGHTLEG1 && j == 3)
		{
			//bFirst = false;
			QByteArray data;
			int begin = 1;
			int run = 1;
			m_pCommunicate->setData(LEGUNBLOCK_LEFTLEG2, begin, run, false, data);
			qDebug() << "Leg unblock left2 send message " << data.toHex().toUpper();
			m_pCommunicate->sendData(data);
		}
		else if (pro == SHOULDERUNBLOCK)
		{
			QByteArray data;
			int begin = 1;
			int run = 1;
			m_pCommunicate->setData(SHOULDERUNBLOCK, begin, run, false, data);
			qDebug() << "Shoulder unblock send message " << data.toHex().toUpper();
			m_pCommunicate->sendData(data);
		}

		bFirstAcu = true;
		emit FinishOneGroup();
	}

	sendodr("TCPSpeedEnd()");

	return true;
}

void MyWindow::Run(PROTOCOL pro, int level)
{
	if (m_eModel == MODEL_STEP)
	{
		StepModel(pro,level);
	}
	else if (m_eModel == MODEL_CONTINUE)
	{
		ContinueModel(pro,level);
	}

	//JointMovJ(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	RobotStorage();
	Wait_Done();
	m_pAutoTreat->SetWidgetTreatFinish();
}

void MyWindow::go()
{
	for (uint i = 0; i < points.size(); ++i)
	{
		cv::Point3d& p = points[i].p3d;
		m_pAutoTreat->GetWidgetTreatOnGoing()->SetLabelTreating(i);
		MovJ(p.x, p.y, p.z, -178, 0, NORMAL_ANGLE);   //联调     //
		//MovJ(points[i].x, points[i].y, 0.0, -178, 0, 179.5);   //联调     //
		Wait_Done();

		Eigen::VectorXd pos(6);
		pos << p.x, p.y, p.z, -178, 0, NORMAL_ANGLE;
		m_vCurrentPos.clear();
		m_vCurrentPos.push_back(p.x);
		m_vCurrentPos.push_back(p.y);
		m_vCurrentPos.push_back(p.z);
		m_vCurrentPos.push_back(-178.0);
		m_vCurrentPos.push_back(0.0);
		m_vCurrentPos.push_back(NORMAL_ANGLE);

		while (!AdmittanceControl())
		{
			qDebug() << "Move to:" << m_vCurrentPos[0] << ", " << m_vCurrentPos[1] << ", " << m_vCurrentPos[2];
		}

		m_pAutoTreat->GetWidgetTreatOnGoing()->SetLabelTreated(i);
	}

	//JointMovJ(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	RobotStorage();
	Wait_Done();
	m_pAutoTreat->SetWidgetTreatFinish();
}

std::vector<cv::Point> MyWindow::detect(cv::Mat img, std::string ModelPath, cv::Mat& outImg)
{
	DCSP_INIT_PARAM params;
	params.ModelPath = ModelPath;
	params.ModelType = YOLO_POSE_V8;
	params.classesNum = 1;
	params.RectConfidenceThreshold = 0.6f;
	params.iouThreshold = 0.5f;
	params.CudaEnable = false;
	params.LogSeverityLevel = 3;
	params.imgSize = { 640, 640 };

	//DCSP_CORE* p1 = new DCSP_CORE;
	//char* ret = p1->CreateSession(params);

	//std::cout << img_path << std::endl;
	//cv::Mat img = cv::imread(img);
	//cv::imshow("TEST_ORIGIN", img);
	std::vector<cv::Point> re;
	std::vector<DCSP_RESULT> res;
	//p1->RunSession(img, res);
	for (uint i = 0; i < res.size(); i++)
	{
		cv::rectangle(img, res.at(i).box, cv::Scalar(125, 123, 0), 3);
		//cv::putText(img, std::to_string(i), res.at(i).box.tl() + cv::Point(3, 3), 1, 1, cv::Scalar(0), 1);
		re.push_back(cv::Point(res.at(i).box.x, res.at(i).box.y));
	}
#if 1
	//cv::Mat img2;
	//cv::resize(img, outImg, img.size());
	outImg = img.clone();
	//cv::namedWindow("TEST_ORIGIN", cv::WINDOW_AUTOSIZE);
	//cv::imshow("TEST_ORIGIN", outImg);
	//cv::destroyAllWindows();
#endif //1
	return re;
	//cv::imwrite("E:\\output\\" + std::to_string(k) + ".png", img);
}
void MyWindow::SetStoped(bool flag)
{
	m_bStoped = flag;
}

int MyWindow::GetIntensity()
{
	return m_iCurrIntensity;
}

void MyWindow::IncrIntensity()
{
	if (m_iCurrIntensity > 99)
	{
		return;
	}

	int begin = 1;
	int run = m_iCurrIntensity + 1;
	QByteArray data;
	m_pCommunicate->setData(m_eCurrProto, begin, run, true, data);
	qDebug() << "IncrIntensity send message " << data.toHex().toUpper();
	m_pCommunicate->sendData(data);
	m_iCurrIntensity = run;
}

void MyWindow::DecrIntensity()
{
	if (m_iCurrIntensity < 1)
	{
		return;
	}

	int begin = 1;
	int run = m_iCurrIntensity - 1;
	QByteArray data;
	m_pCommunicate->setData(m_eCurrProto, begin, run, true, data);
	qDebug() << "DecrIntensity send message " << data.toHex().toUpper();
	m_pCommunicate->sendData(data);
	m_iCurrIntensity = run;
}

void MyWindow::stop()
{
	// 清理存储的穴位点
	points.clear();
	sendodr("ResetRobot()");
	Wait_Done();
	//JointMovJ(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	RobotStorage();
	Wait_Done();
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
	while (pausebit)
	{
		qsleep(100);
	}
}

void MyWindow::Wait_ForMove(int timeout)
{
	qsleep(30);
	int time_c = 0;
	while (RobotMode != ROBOT_MODE_ENABLE || time_c > timeout)
	{
		qsleep(5);
		time_c += 5;
	}
}

void MyWindow::Wait_ForForces(int timeout)
{
	qsleep(10);
	int time_c = 0;
	while (m_vForces.size() != 9 || time_c > timeout)
	{
		qsleep(5);
		time_c += 5;
	}

	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_time = current_date_time.toString("hh:mm:ss.zzz");
	qDebug() << "Receive force time is " << current_time << endl;
}

void MyWindow::Wait_ForTreat(int timeout)
{
	qsleep(300);
	int time_c = 0;
	while (/*RobotMode == ROBOT_MODE_ENABLE &&*/ time_c < timeout)
	{
		qsleep(1000);
		time_c += 1000;
		emit FinishOneSecond();
	}
}

void MyWindow::Wait_ForShort(int timeout)
{
	int time = 50;
	qsleep(time);
	int time_c = 0;
	while (RobotMode != ROBOT_MODE_ENABLE || time_c > timeout)
	{
		qsleep(time);
		time_c += time;
	}
}

void MyWindow::Wait_Done(int timeout)
{
	qsleep(300);
	int time_c = 0;
	while (RobotMode != ROBOT_MODE_ENABLE || time_c > timeout)
	{
		qsleep(100);
		time_c += 100;
	}
}

void MyWindow::poweron()
{
	if (pw == 1) return;
	pw = 1;
	//sendodr("PowerOn()");
	//qsleep(10000);
	sendodr("DisableRobot()");
	// 按摩头重量 20250721 1.5是公斤
	sendodr("EnableRobot(0.5,0,0,0)");
	//sendodr("BrakeControl(1,1)");
	// 机械臂速度 20250721
	sendodr("SpeedFactor(25)");
	//sendodr("RobotMode()");
	Tool(2);
}
std::vector<cv::Point3d> MyWindow::convert_camera2arm(std::vector<Robot3d> pointsC)
{
	std::vector<cv::Point3d> tmp_points;
	for (uint i = 0; i < pointsC.size(); ++i)
	{
		//cv::Point3d point = cv::Point3d(start_Camera_Point.x - pointsC[i].p3d.x,
		//    start_Camera_Point.y + pointsC[i].p3d.y - 155.0,     // 减去摄像头和按摩头的距离
		//    200.0 + start_Camera_Point.z - pointsC[i].p3d.z);

		cv::Point3d point = cv::Point3d(start_Camera_Point.x + 11 - pointsC[i].p3d.x/*+70.0*/,
			start_Camera_Point.y - 105.0 + pointsC[i].p3d.y/*-20.0*/,     // 减去摄像头和按摩头的距离
			start_Camera_Point.z + 168.45 - pointsC[i].p3d.z);

		if (point.z < -240.0 || point.z>-130.0)  // -50 -120？
		{
			point.z = -240.0;
		}

		tmp_points.push_back(point); // 加上理疗头高度：z轴向上
	}

	return tmp_points;
}

void MyWindow::setPlanPoints(std::vector<RobotPoint> allPoint)
{
	points = allPoint;
}

std::vector<RobotPoint>& MyWindow::GetPlanPoints()
{
	return points;
}

cv::Mat& MyWindow::getPlanImage()
{
	return m_mPlanImage;
}

QImage MyWindow::cvMatToQImage(const cv::Mat& inMat)
{
	switch (inMat.type())
	{
		// 8-bit, 4 channel
	case CV_8UC4:
	{
		QImage image(inMat.data,
			inMat.cols, inMat.rows,
			static_cast<int>(inMat.step),
			QImage::Format_ARGB32);

		return image;
	}

	// 8-bit, 3 channel
	case CV_8UC3:
	{
		QImage image(inMat.data,
			inMat.cols, inMat.rows,
			static_cast<int>(inMat.step),
			QImage::Format_RGB888);

		return image.rgbSwapped();
	}

	// 8-bit, 1 channel
	case CV_8UC1:
	{
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
		QImage image(inMat.data,
			inMat.cols, inMat.rows,
			static_cast<int>(inMat.step),
			QImage::Format_Grayscale8);//Format_Alpha8 and Format_Grayscale8 were added in Qt 5.5
#else//这里还有一种写法，最后给出
		static QVector<QRgb>  sColorTable;

		// only create our color table the first time
		if (sColorTable.isEmpty())
		{
			sColorTable.resize(256);

			for (int i = 0; i < 256; ++i)
			{
				sColorTable[i] = qRgb(i, i, i);
			}
		}

		QImage image(inMat.data,
			inMat.cols, inMat.rows,
			static_cast<int>(inMat.step),
			QImage::Format_Indexed8);

		image.setColorTable(sColorTable);
#endif

		return image;
	}

	default:
		qWarning() << "CVS::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
		break;
	}

	return QImage();
}

QPixmap MyWindow::cvMatToQPixmap(const cv::Mat& inMat)
{
	return QPixmap::fromImage(cvMatToQImage(inMat));
}

// 机械臂移动到默认的初始位置
void MyWindow::MoveToNormalPos()
{
	sendodr("EnableRobot(0.5,0,0,0)");

	MovJ(start_Camera_Point.x, start_Camera_Point.y, start_Camera_Point.z, 180, 0, 45);
	Wait_Done();
}

bool MyWindow::getImage(cv::Mat& img/*std::vector<cv::Point3d>& points,cv::Mat& colorRawMat*/)
{
	//JointMovJ(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	//Sync();
	sendodr("EnableRobot(0.5,0,0,0)");

	MovJ(start_Camera_Point.x, start_Camera_Point.y, start_Camera_Point.z, 180, 0, NORMAL_ANGLE);
	Wait_Done();

	std::vector<OBColorPoint> pointCloud_frame_data;
	obCapture(colorRawMat, pointCloud_frame_data);

	if (colorRawMat.rows == 0 || colorRawMat.cols == 0)
	{
		QMessageBox::information(NULL, "Info", "Capture image failed !", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return false;
	}

	SaveImage(colorRawMat);

	colorRawMat = cv::imread("111.jpg");
	//std::string path = /*E://workspace//PhysicalTherapyRobot//x64//Release//*/ "yolov8_640_640_v15.onnx";
	//std::string path = /*E://workspace//PhysicalTherapyRobot//x64//Release//*/ /*"back_keypoints_0616.onnx"*/"last_0923.onnx";
	//std::string path = "last0923.onnx";
	cv::Mat colorRawMatR = colorRawMat.t();
	cv::rotate(colorRawMat, colorRawMatR, cv::ROTATE_90_CLOCKWISE);
	//std::vector<cv::Point> base0 = detect(colorRawMatR, path /*u8"debug/yolov8_640_640_v15.onnx"*/, img);
	//img = colorRawMatR.clone();

	bool hasKeypoints;
	cv::Rect_<float> out_bbox;
	std::vector<Keypoint> base0;
	processFrame(colorRawMatR, m_net, modelScoreThreshold, modelNMSThreshold,
		modelShape.width, modelShape.height, 15,
		hasKeypoints, out_bbox, base0);

	if (!hasKeypoints)
	{
		return false;
	}

	float ratio_x, ratio_y;
	ratio_x = colorRawMatR.cols / (float)modelShape.width;
	ratio_y = colorRawMatR.rows / (float)modelShape.height;
	img.copyTo(m_mPlanImage);
	std::vector<cv::Point> base;
	for (uint i = 0; i < base0.size(); ++i)
	{
		base.push_back(cv::Point(base0[i].position.y * ratio_y, (colorRawMat.rows - 1) - base0[i].position.x * ratio_x));
		qDebug() << "2d index " << i << "y " << base0[i].position.y * ratio_y << " x " << (colorRawMat.rows - 1) - base0[i].position.x * ratio_x;
	}

	std::vector<Robot3d> pointsC = get3Dpoints(base, pointCloud_frame_data) /* *m */;   //需要一个变换矩阵m

	// points = convert_camera2arm(pointsC);
	std::vector<cv::Point3d> tempPoints = convert_camera2arm(pointsC);
#if 1
	tempPoints.clear();
	tempPoints.push_back(cv::Point3d(222.571, -472.06, -240.0));
	tempPoints.push_back(cv::Point3d(233.571, -492.06, -240));
	tempPoints.push_back(cv::Point3d(153.571, -483.06, -195.962));
	tempPoints.push_back(cv::Point3d(150.571, -569.06, -183.962));
	tempPoints.push_back(cv::Point3d(139.571, -390.06, -174.962));
	tempPoints.push_back(cv::Point3d(84.5707, -486.06, -183.962));
	tempPoints.push_back(cv::Point3d(15.5707, -490.06, -178.962));
	tempPoints.push_back(cv::Point3d(-337.429, -514.06, -240));
	tempPoints.push_back(cv::Point3d(-302.429, -502.06, -156.962));
	tempPoints.push_back(cv::Point3d(-200.429, -497.06, -165.962));
	tempPoints.push_back(cv::Point3d(-199.429, -530.06, -168.962));
	tempPoints.push_back(cv::Point3d(-232.429, -537.06, -167.962));
	tempPoints.push_back(cv::Point3d(-235.429, -467.06, -157.962));
	tempPoints.push_back(cv::Point3d(-244.429, -621.06, -185.962));
	tempPoints.push_back(cv::Point3d(-293.429, -320.06, -240));
#endif

	points.clear();
	for (int i = 0; i < tempPoints.size(); ++i)
	{
		RobotPoint p;
		p.p2d = base0[i].position;
		p.p3d = tempPoints[i];
		p.n3d = pointsC[i].n3d;
		p.xuewei = (DETECTED_XUEWEI)(i);

		qDebug() << "3d index " << i << " x " << tempPoints[i].x << " y " << tempPoints[i].y << " z " << tempPoints[i].z;
		points.push_back(p);
	}

	//todo: 点的顺序  //
	qDebug() << QString("points size is %1 ").arg(points.size());
	for (uint i = 0; i < tempPoints.size(); ++i) {
		qDebug() << i << tempPoints[i].x << tempPoints[i].y << tempPoints[i].z;

		cv::circle(colorRawMat, base[i], 2, cv::Scalar(125, 123, 0), 3);
		std::string msg = std::to_string(i) + " ("
			+ std::to_string(int(tempPoints[i].x)) + ", "
			+ std::to_string(int(tempPoints[i].y)) + ", "
			+ std::to_string(int(tempPoints[i].z)) + ")";
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
	//cv::rectangle(colorRawMat, out_bbox, cv::Scalar(0, 255, 255));
	img = colorRawMat.clone();

	cv::namedWindow("colorRawMat", cv::WINDOW_AUTOSIZE);
	cv::imshow("colorRawMat", colorRawMat);

	return true;
}

std::vector<Robot3d> MyWindow::get3Dpoints(std::vector<cv::Point> base, std::vector<OBColorPoint> pointCloud_frame_data)
{
	//    std::vector<cv::Point3d> points;
	//    for (int i = 0; i < 720 * 1280; ++i)
	//    {
	//        OBColorPoint* pointA = Colorpoint + i;
	//        qDebug() << i << ": " << pointA->x << ", " << pointA->y << ", " << pointA->z
	//                     << ", " << pointA->r << ", " << pointA->g << ", " << pointA->b;
	//    }

	qDebug() << "Current vector size is " << pointCloud_frame_data.size();
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	cloud->reserve(pointCloud_frame_data.size());
	for (int i = 0; i < pointCloud_frame_data.size(); ++i)
	{
		OBColorPoint& pointA = pointCloud_frame_data[i];
		pcl::PointXYZ point(pointA.x, pointA.y, pointA.z);
		cloud->push_back(point);
		//qDebug() << i << ": " << pointA.x << ", " << pointA.y << ", " << pointA.z
		//                     << ", " << pointA.r << ", " << pointA.g << ", " << pointA.b;
	}

	pcl::PointCloud<pcl::Normal>::Ptr normals;
	GetCloudsNormals(cloud, normals);

	std::vector<Robot3d> Points;
	for (uint i = 0; i < base.size(); ++i)
	{
		OBColorPoint& pointA = pointCloud_frame_data[int(base[i].y) * imageWidth + int(base[i].x)];                                     //宽0->1279，高0-719
		pcl::Normal& normal = normals->at(int(base[i].y) * imageWidth + int(base[i].x));
		Robot3d point;
		point.n3d = cv::Point3d(normal.normal_x, normal.normal_y, normal.normal_z);
		if (std::isnan(point.n3d.x) || std::isnan(point.n3d.y) || std::isnan(point.n3d.z))
		{
			point.n3d = cv::Point3d(0.0, 0.0, -1.0);
		}

		point.p3d = cv::Point3d(double(pointA.x), double(pointA.y), double(pointA.z));
		Points.push_back(point);
	}

	return Points;
}

double MyWindow::strToDouble(std::string str)
{ // string转double
	char* ch = new char[str.length()];
	double d;
	for (int i = 0; i != str.length(); i++)
	{
		ch[i] = str[i];
	}
	d = atof(ch);

	delete[]ch;

	return d;
}

// 创建文件夹的函数
void CreateFolder(const QString& folderPath)
{
	QDir dir(folderPath);
	if (!dir.exists()) {
		dir.mkdir(folderPath);
	}
}

QString GetCurrentTimeSecond()
{
	QDateTime time = QDateTime::currentDateTime();
	QString dateTime = time.toString("yyyy_MM_dd_hh_mm");
	QString str = QString("%1").arg(dateTime);

	return str;
}

void SaveImage(const cv::Mat& img)
{
	QString fold("E:\\images");
	CreateFolder(fold);
	QString file = fold;
	file.append("\\");
	file.append(GetCurrentTimeSecond());
	file.append(".jpg");
	cv::imwrite(file.toStdString().c_str(), img);
}