#include "AutoTreat.h"

#include "MyWindow.h"

AutoTreat::AutoTreat(MyWindow* window,QWidget *parent)
    : m_pWindow(window)
    , QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	m_pAutoTreatPrePrepare.reset(new AutoTreatPrePrepare(this));
	ui.stackedWidget_AutoTreat_Pages->addWidget(m_pAutoTreatPrePrepare.get());

	// 选择和设置
	m_pAutoTreatSelectAndSetting.reset(new AutoTreatSelectAndSetting(this));
	ui.stackedWidget_AutoTreat_Pages->addWidget(m_pAutoTreatSelectAndSetting.get());

	// 开背
	m_pAutoTreatOpenBack.reset(new AutoTreatOpenBack(this));
	ui.stackedWidget_AutoTreat_Pages->addWidget(m_pAutoTreatOpenBack.get());

	// 腿部初开
	m_pAutoTreatLegOpen.reset(new AutoTreatLegOpen(this));
	ui.stackedWidget_AutoTreat_Pages->addWidget(m_pAutoTreatLegOpen.get());

	// 腿部疏通
	m_pAutoTreatLegUnblock.reset(new AutoTreatLegUnblock(this));
	ui.stackedWidget_AutoTreat_Pages->addWidget(m_pAutoTreatLegUnblock.get());

	// 肩部疏通
	m_pAutoTreatShoulderUnblock.reset(new AutoTreatShoulderUnblock(this));
	ui.stackedWidget_AutoTreat_Pages->addWidget(m_pAutoTreatShoulderUnblock.get());

	m_pAutoTreatSetTime.reset(new AutoTreatSetTime(this));
	ui.stackedWidget_AutoTreat_Pages->addWidget(m_pAutoTreatSetTime.get());

	m_pAutoTreatAcupointRecog.reset(new AutoTreatAcupointRecog(this));
	ui.stackedWidget_AutoTreat_Pages->addWidget(m_pAutoTreatAcupointRecog.get());

	m_pAutoTreatToleranceTest.reset(new AutoTreatToleranceTest(this));
	ui.stackedWidget_AutoTreat_Pages->addWidget(m_pAutoTreatToleranceTest.get());

	m_pAutoTreatBegin.reset(new AutoTreatBegin(m_pWindow,this));
	ui.stackedWidget_AutoTreat_Pages->addWidget(m_pAutoTreatBegin.get());

	m_pAutoTreatLegOpenBegin.reset(new AutoTreatLegOpenBegin(m_pWindow,this));
	ui.stackedWidget_AutoTreat_Pages->addWidget(m_pAutoTreatLegOpenBegin.get());

	m_pAutoTreatLegUnblockBegin.reset(new AutoTreatLegUnblockBegin(m_pWindow,this));
	ui.stackedWidget_AutoTreat_Pages->addWidget(m_pAutoTreatLegUnblockBegin.get());

	m_pAutoTreatShoulderUnblockBegin.reset(new AutoTreatShoulderUnblockBegin(m_pWindow, this));
	ui.stackedWidget_AutoTreat_Pages->addWidget(m_pAutoTreatShoulderUnblockBegin.get());

	m_pAutoTreatOnGoing.reset(new AutoTreatOnGoing(this));
	ui.stackedWidget_AutoTreat_Pages->addWidget(m_pAutoTreatOnGoing.get());

	m_pAutoTreatFinish.reset(new AutoTreatFinish(this));
	ui.stackedWidget_AutoTreat_Pages->addWidget(m_pAutoTreatFinish.get());

	ui.label_PrepareFlag->show();
	ui.label_SelectFlag->hide();
	ui.label_BeginFlag->hide();
	ui.label_OrganizeFlag->hide();
	ui.stackedWidget_AutoTreat_Pages->setCurrentWidget(m_pAutoTreatPrePrepare.get());
}

AutoTreat::~AutoTreat()
{
}

void AutoTreat::SetWidgetInstruction()
{
	//ui.stackedWidget_AutoTreat_Pages->setCurrentWidget(m_pTreatInstruction.get());
}

void AutoTreat::SetWidgetPrePrepare()
{
	//m_pAutoTreatPrePrepare->SetFirstStep();
	ui.label_PrepareFlag->show();
	ui.label_SelectFlag->hide();
	ui.label_BeginFlag->hide();
	ui.label_OrganizeFlag->hide();

	ui.stackedWidget_AutoTreat_Pages->setCurrentWidget(m_pAutoTreatPrePrepare.get());
}

void AutoTreat::SetWidgetSelectAndSetting()
{
	ui.label_PrepareFlag->hide();
	ui.label_SelectFlag->show();
	ui.label_BeginFlag->hide();
	ui.label_OrganizeFlag->hide();

	ui.stackedWidget_AutoTreat_Pages->setCurrentWidget(m_pAutoTreatSelectAndSetting.get());
}

void AutoTreat::SetWidgetOpenBack()
{
	ui.stackedWidget_AutoTreat_Pages->setCurrentWidget(m_pAutoTreatOpenBack.get());
}

void AutoTreat::SetWidgetLegOpen()
{
	ui.stackedWidget_AutoTreat_Pages->setCurrentWidget(m_pAutoTreatLegOpen.get());
}

void AutoTreat::SetWidgetLegUnblock()
{
	ui.stackedWidget_AutoTreat_Pages->setCurrentWidget(m_pAutoTreatLegUnblock.get());
}

void AutoTreat::SetWidgetShoulderUnblock()
{
	ui.stackedWidget_AutoTreat_Pages->setCurrentWidget(m_pAutoTreatShoulderUnblock.get());
}

void AutoTreat::SetWidgetSetTime()
{  
	ui.stackedWidget_AutoTreat_Pages->setCurrentWidget(m_pAutoTreatSetTime.get());
}

void AutoTreat::SetWidgetAcupointRecog()
{
	ui.stackedWidget_AutoTreat_Pages->setCurrentWidget(m_pAutoTreatAcupointRecog.get());
}

void AutoTreat::SetWidgetToleranceTest()
{
	ui.stackedWidget_AutoTreat_Pages->setCurrentWidget(m_pAutoTreatToleranceTest.get());
}
void AutoTreat::SetWidgetTreatLegOpenBegin()
{
	ui.label_PrepareFlag->hide();
	ui.label_SelectFlag->hide();
	ui.label_BeginFlag->show();
	ui.label_OrganizeFlag->hide();

	m_pAutoTreatLegOpenBegin->SetLegOpen(m_pAutoTreatLegOpen.get());
	ui.stackedWidget_AutoTreat_Pages->setCurrentWidget(m_pAutoTreatLegOpenBegin.get());
}
void AutoTreat::SetWidgetTreatLegUnblockBegin()
{
	ui.label_PrepareFlag->hide();
	ui.label_SelectFlag->hide();
	ui.label_BeginFlag->show();
	ui.label_OrganizeFlag->hide();

	m_pAutoTreatLegUnblockBegin->SetLegUnblock(m_pAutoTreatLegUnblock.get());
	ui.stackedWidget_AutoTreat_Pages->setCurrentWidget(m_pAutoTreatLegUnblockBegin.get());
}
void AutoTreat::SetWidgetTreatShoulderUnblockBegin()
{
	ui.label_PrepareFlag->hide();
	ui.label_SelectFlag->hide();
	ui.label_BeginFlag->show();
	ui.label_OrganizeFlag->hide();

	m_pAutoTreatShoulderUnblockBegin->SetShoulderUnblock(m_pAutoTreatShoulderUnblock.get());
	ui.stackedWidget_AutoTreat_Pages->setCurrentWidget(m_pAutoTreatShoulderUnblockBegin.get());
}

void AutoTreat::SetWidgetTreatBegin(OPENBACK_MODEL model,XUEWEI_TYPE type,int time)
{
	ui.label_PrepareFlag->hide();
	ui.label_SelectFlag->hide();
	ui.label_BeginFlag->show();
	ui.label_OrganizeFlag->hide();

	if (model == MODEL_STEP)
	{
		m_pAutoTreatBegin->SetStepDistanceLabel();
	}

	m_pAutoTreatBegin->SetXuewei(type);
	m_pAutoTreatBegin->SetTime(time);
	m_pAutoTreatBegin->SetStarted(false);
	int speed = m_pAutoTreatOpenBack->GetSpeed();
	m_pAutoTreatBegin->SetSpeed(speed);
	m_pAutoTreatBegin->SetOpenBack(m_pAutoTreatOpenBack.get());
	ui.stackedWidget_AutoTreat_Pages->setCurrentWidget(m_pAutoTreatBegin.get());
}

void AutoTreat::SetWidgetTreatOnGoing()
{
	m_pAutoTreatOnGoing->TimerStart();
	m_pAutoTreatOnGoing->SetPlanImage();
	ui.stackedWidget_AutoTreat_Pages->setCurrentWidget(m_pAutoTreatOnGoing.get());
}

QSharedPointer<AutoTreatOnGoing> AutoTreat::GetWidgetTreatOnGoing()
{
	return m_pAutoTreatOnGoing;
}

void AutoTreat::SetCurrentProj(QString proj)
{
	m_pAutoTreatFinish->SetCurrentProj(proj);
}

void AutoTreat::SetWidgetTreatFinish()
{
	ui.label_PrepareFlag->hide();
	ui.label_SelectFlag->hide();
	ui.label_BeginFlag->hide();
	ui.label_OrganizeFlag->show();

	ui.stackedWidget_AutoTreat_Pages->setCurrentWidget(m_pAutoTreatFinish.get());
}

void AutoTreat::FinishReturn()
{
	m_pWindow->SetWidgetHomePage();
}