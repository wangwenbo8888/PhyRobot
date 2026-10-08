#include "SetUp.h"

#include "MyWindow.h"

#include "QtWidgetsCommunicateTest.h"

#include <QDoubleSpinBox>
#include <QCoreApplication>
#include <QSettings>
#include <QFile>

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

	disconnect(ui.checkBox_DragTeachMode, SIGNAL(stateChanged(int)), this, SLOT(On_checkBox_DragTeachMode_stateChanged(int)));
	connect(ui.checkBox_DragTeachMode, SIGNAL(stateChanged(int)), this, SLOT(On_checkBox_DragTeachMode_stateChanged(int)));

	// 同步配置文件中的拖拽治疗模式状态
	ui.checkBox_DragTeachMode->setChecked(m_pWindow->IsDragTeachMode());

	// 直接从 user.ini 加载末端负载参数，避免依赖 MyWindow 的加载顺序
	m_bUpdatingPayload = true;
	QString iniPath = QCoreApplication::applicationDirPath() + "/../../user.ini";
	if (!QFile::exists(iniPath)) {
		iniPath = QCoreApplication::applicationDirPath() + "/../user.ini";
	}
	QSettings settings(iniPath, QSettings::IniFormat);
	ui.spinBox_PayloadMass->setValue(settings.value("config/payloadMass", 0.0).toDouble());
	ui.spinBox_PayloadX->setValue(settings.value("config/payloadX", 0.0).toDouble());
	ui.spinBox_PayloadY->setValue(settings.value("config/payloadY", 0.0).toDouble());
	ui.spinBox_PayloadZ->setValue(settings.value("config/payloadZ", 0.0).toDouble());
	m_bUpdatingPayload = false;

	disconnect(ui.spinBox_PayloadMass, SIGNAL(valueChanged(double)), this, SLOT(On_spinBox_PayloadMass_valueChanged(double)));
	connect(ui.spinBox_PayloadMass, SIGNAL(valueChanged(double)), this, SLOT(On_spinBox_PayloadMass_valueChanged(double)));

	disconnect(ui.spinBox_PayloadX, SIGNAL(valueChanged(double)), this, SLOT(On_spinBox_PayloadX_valueChanged(double)));
	connect(ui.spinBox_PayloadX, SIGNAL(valueChanged(double)), this, SLOT(On_spinBox_PayloadX_valueChanged(double)));

	disconnect(ui.spinBox_PayloadY, SIGNAL(valueChanged(double)), this, SLOT(On_spinBox_PayloadY_valueChanged(double)));
	connect(ui.spinBox_PayloadY, SIGNAL(valueChanged(double)), this, SLOT(On_spinBox_PayloadY_valueChanged(double)));

	disconnect(ui.spinBox_PayloadZ, SIGNAL(valueChanged(double)), this, SLOT(On_spinBox_PayloadZ_valueChanged(double)));
	connect(ui.spinBox_PayloadZ, SIGNAL(valueChanged(double)), this, SLOT(On_spinBox_PayloadZ_valueChanged(double)));

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

void SetUp::On_checkBox_DragTeachMode_stateChanged(int state)
{
	m_pWindow->SetDragTeachMode(state == Qt::Checked);
}

void SetUp::On_spinBox_PayloadMass_valueChanged(double v)
{
	if (m_bUpdatingPayload) return;
	m_pWindow->SetPayloadMass(v);
}

void SetUp::On_spinBox_PayloadX_valueChanged(double v)
{
	if (m_bUpdatingPayload) return;
	m_pWindow->SetPayloadX(v);
}

void SetUp::On_spinBox_PayloadY_valueChanged(double v)
{
	if (m_bUpdatingPayload) return;
	m_pWindow->SetPayloadY(v);
}

void SetUp::On_spinBox_PayloadZ_valueChanged(double v)
{
	if (m_bUpdatingPayload) return;
	m_pWindow->SetPayloadZ(v);
}