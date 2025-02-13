#include "FinishOrganize.h"

#include "MyWindow.h"

FinishOrganize::FinishOrganize(MyWindow* window,QWidget *parent)
	: m_pWindow(window),
	QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(on_PushButton_NextStep_Clicked()));
	connect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(on_PushButton_NextStep_Clicked()));

	disconnect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(on_PushButton_Confirm()));
	connect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(on_PushButton_Confirm()));

	disconnect(ui.pushButton_FinishBack, SIGNAL(clicked()), this, SLOT(on_PushButton_FinishBack()));
	connect(ui.pushButton_FinishBack, SIGNAL(clicked()), this, SLOT(on_PushButton_FinishBack()));

	ui.pushButton_NextStep->setEnabled(false);
	ui.pushButton_FinishBack->setEnabled(false);

	m_eOperation = CLEAR;
	ui.label_Hook->setPixmap(QPixmap(":/GrayHook.png"));
}

FinishOrganize::~FinishOrganize()
{
	
}

void FinishOrganize::on_PushButton_FinishBack()
{
	m_pWindow->On_pushButton_MainFrame_Clicked();
}

void FinishOrganize::on_PushButton_Confirm()
{
	if (m_eOperation == CLEAR)
	{
		ui.pushButton_Confirm->setText(QStringLiteral("确认清理"));
		ui.label_Hook->setPixmap(QPixmap(":/GreenHook.png"));

		ui.pushButton_NextStep->setEnabled(true);
	}
	else if (m_eOperation == SHUTDOWN)
	{
		ui.pushButton_Confirm->setText(QStringLiteral("确认回收"));
		ui.label_Hook->setPixmap(QPixmap(":/GreenHook.png"));

		ui.pushButton_NextStep->setEnabled(true);
	}
	else if (m_eOperation == HOMING)
	{
		ui.pushButton_Confirm->setText(QStringLiteral("确认归位"));
		ui.label_Hook->setPixmap(QPixmap(":/GreenHook.png"));

		ui.pushButton_FinishBack->setEnabled(true);
	}
}

void FinishOrganize::InitState()
{
	m_eOperation = CLEAR;
	ui.pushButton_NextStep->setEnabled(false);
	ui.pushButton_FinishBack->setEnabled(false);

	ui.label_Hook->setPixmap(QPixmap(":/GrayHook.png"));
	ui.label_CleanGuest->setStyleSheet("background-color:rgb(101,158,213)");
	ui.label_PowerOff->setStyleSheet("background-color:white;border:2px solid rgb(101,158,213);");
	ui.label_ArmReturn->setStyleSheet("background-color:white;border:2px solid rgb(101,158,213);");
}

void FinishOrganize::on_PushButton_NextStep_Clicked()
{
	if (m_eOperation == CLEAR)
	{
		ui.label_CleanGuest->setStyleSheet("background-color:white;border:2px solid rgb(101,158,213);");
		ui.label_PowerOff->setStyleSheet("background-color:rgb(101,158,213)");
		//ui.label_Hook->setPixmap(QPixmap(":/灰勾.png"));
		ui.label_Hook->setPixmap(QPixmap(":/GrayHook.png"));

		ui.pushButton_NextStep->setEnabled(false);

		m_eOperation = SHUTDOWN;
	}
	else if (m_eOperation == SHUTDOWN)
	{
		ui.label_PowerOff->setStyleSheet("background-color:white;border:2px solid rgb(101,158,213);");
		ui.label_ArmReturn->setStyleSheet("background-color:rgb(101,158,213)");
		ui.label_Hook->setPixmap(QPixmap(":/GrayHook.png"));

		ui.pushButton_NextStep->setEnabled(false);

		m_eOperation = HOMING;
	}
	else if (m_eOperation == HOMING)
	{
		//ui.label_MetalDetect->setStyleSheet("background-color:white;border:2px solid rgb(101,158,213);");
		//ui.label_OperDebug->setStyleSheet("background-color:rgb(101,158,213)");
		ui.pushButton_NextStep->setEnabled(false);
		ui.label_Hook->setPixmap(QPixmap(":/GrayHook.png"));

		m_eOperation = CLEAR;
	}
}
