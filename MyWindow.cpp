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