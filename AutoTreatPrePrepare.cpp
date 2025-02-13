#include "AutoTreatPrePrepare.h"

#include "AutoTreat.h"

AutoTreatPrePrepare::AutoTreatPrePrepare(AutoTreat* treat, QWidget *parent)
	: m_pAutoTreat(treat)
	, QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);
	
	disconnect(ui.pushButton, SIGNAL(clicked()), this, SLOT(on_PushButton_Confirm()));
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(on_PushButton_Confirm()));

	disconnect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_LastStep_Clicked()));
	connect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_LastStep_Clicked()));

	disconnect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_NextStep_Clicked()));
	connect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_NextStep_Clicked()));

	m_eOperation = BLAM_TO_APPLY;
}

AutoTreatPrePrepare::~AutoTreatPrePrepare()
{
}

void AutoTreatPrePrepare::on_Pushbutton_NextStep_Clicked()
{
	if (m_eOperation == WORK_TEST)
	{
		m_pAutoTreat->SetWidgetSetTime();

		ui.label_WorkTest->setStyleSheet("background-color:white;border:2px solid rgb(101,158,213);");
		ui.label_BalmApply->setStyleSheet("background-color:rgb(101,158,213)");
		ui.label_Hook->setPixmap(QPixmap(":/GrayHook.png"));

		ui.pushButton_NextStep->setEnabled(false);

		m_eOperation = BLAM_TO_APPLY;
	}
	else if (m_eOperation== BLAM_TO_APPLY)
	{
		ui.label_BalmApply->setStyleSheet("background-color:white;border:2px solid rgb(101,158,213);");
		ui.label_MetalDetect->setStyleSheet("background-color:rgb(101,158,213)");
		ui.pushButton_NextStep->setEnabled(false);

		ui.label_Hook->setPixmap(QPixmap(":/GrayHook.png"));

		m_eOperation = METAL_DETECT;
	}
	else if (m_eOperation == METAL_DETECT)
	{
		ui.label_MetalDetect->setStyleSheet("background-color:white;border:2px solid rgb(101,158,213);");
		ui.label_OperDebug->setStyleSheet("background-color:rgb(101,158,213)");
		ui.pushButton_NextStep->setEnabled(false);

		ui.label_Hook->setPixmap(QPixmap(":/GrayHook.png"));

		m_eOperation = OPER_DEBUG;
	}
	else if (m_eOperation == OPER_DEBUG)
	{
		ui.label_OperDebug->setStyleSheet("background-color:white;border:2px solid rgb(101,158,213);");
		ui.label_WorkTest->setStyleSheet("background-color:rgb(101,158,213)");
		ui.pushButton_NextStep->setEnabled(false);

		ui.label_Hook->setPixmap(QPixmap(":/GrayHook.png"));

		m_eOperation = WORK_TEST;
	}
}


void AutoTreatPrePrepare::on_Pushbutton_LastStep_Clicked()
{
	m_pAutoTreat->SetWidgetInstruction();
}

void AutoTreatPrePrepare::on_PushButton_Confirm()
{
	ui.pushButton_NextStep->setEnabled(true);
	ui.label_Hook->setPixmap(QPixmap(":/GreenHook.png"));
}