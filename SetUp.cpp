#include "SetUp.h"

#include "MyWindow.h"

#include "QtWidgetsCommunicateTest.h"

SetUp::SetUp(MyWindow* window,QWidget *parent)
	: m_pWindow(window),
	QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_RobotStorage, SIGNAL(clicked()), this, SLOT(On_pushButton_RobotStorage_Clicked()));
	connect(ui.pushButton_RobotStorage, SIGNAL(clicked()), this, SLOT(On_pushButton_RobotStorage_Clicked()));

	disconnect(ui.pushButton_RobotResume, SIGNAL(clicked()), this, SLOT(On_pushButton_RobotResume_Clicked()));
	connect(ui.pushButton_RobotResume, SIGNAL(clicked()), this, SLOT(On_pushButton_RobotResume_Clicked()));

	disconnect(ui.pushButton_CommunicateTest, SIGNAL(clicked()), this, SLOT(On_pushButton_CommunicateTest_Clicked()));
	connect(ui.pushButton_CommunicateTest, SIGNAL(clicked()), this, SLOT(On_pushButton_CommunicateTest_Clicked()));

	disconnect(ui.pushButton_ReturnHome, SIGNAL(clicked()), this, SLOT(On_pushButton_ReturnHome_Clicked()));
	connect(ui.pushButton_ReturnHome, SIGNAL(clicked()), this, SLOT(On_pushButton_ReturnHome_Clicked()));
	
	m_pCommTestWidget = new QtWidgetsCommunicateTest(m_pWindow);
}

SetUp::~SetUp()
{
}

void SetUp::On_pushButton_RobotStorage_Clicked()
{
	m_pWindow->RobotStorage();
}

void SetUp::On_pushButton_RobotResume_Clicked()
{
	m_pWindow->RobotGoHome();
}

void SetUp::On_pushButton_CommunicateTest_Clicked()
{
	m_pCommTestWidget->SetCommunicate(m_pWindow->GetCommunicate());
	m_pCommTestWidget->show();
}

void SetUp::On_pushButton_ReturnHome_Clicked()
{
	m_pWindow->GetAutoTreat()->FinishReturn();
}