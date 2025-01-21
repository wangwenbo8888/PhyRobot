#include "AutoTreat.h"

#include "MyWindow.h"
AutoTreat::AutoTreat(MyWindow* window,QWidget *parent)
    : m_pWindow(window)
    , QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	m_pAutoTreatInstruction.reset(new AutoTreatInstruction(this));
	ui.stackedWidget_AutoTreat_Pages->addWidget(m_pAutoTreatInstruction.get());

	m_pAutoTreatPrePrepare.reset(new AutoTreatPrePrepare(this));
	ui.stackedWidget_AutoTreat_Pages->addWidget(m_pAutoTreatPrePrepare.get());

	m_pAutoTreatSetTime.reset(new AutoTreatSetTime(this));
	ui.stackedWidget_AutoTreat_Pages->addWidget(m_pAutoTreatSetTime.get());

	m_pAutoTreatAcupointRecog.reset(new AutoTreatAcupointRecog(this));
	ui.stackedWidget_AutoTreat_Pages->addWidget(m_pAutoTreatAcupointRecog.get());

	m_pAutoTreatToleranceTest.reset(new AutoTreatToleranceTest(this));
	ui.stackedWidget_AutoTreat_Pages->addWidget(m_pAutoTreatToleranceTest.get());

	m_pAutoTreatBegin.reset(new AutoTreatBegin(this));
	ui.stackedWidget_AutoTreat_Pages->addWidget(m_pAutoTreatBegin.get());

	m_pAutoTreatBegin.reset(new AutoTreatBegin(this));
	ui.stackedWidget_AutoTreat_Pages->addWidget(m_pAutoTreatBegin.get());
	
	m_pAutoTreatOnGoing.reset(new AutoTreatOnGoing(this));
	ui.stackedWidget_AutoTreat_Pages->addWidget(m_pAutoTreatOnGoing.get());

	m_pAutoTreatFinish.reset(new AutoTreatFinish(this));
	ui.stackedWidget_AutoTreat_Pages->addWidget(m_pAutoTreatFinish.get());

	ui.stackedWidget_AutoTreat_Pages->setCurrentWidget(m_pAutoTreatInstruction.get());
}

AutoTreat::~AutoTreat()
{
}

void AutoTreat::SetWidgetInstruction()
{
	ui.stackedWidget_AutoTreat_Pages->setCurrentWidget(m_pAutoTreatInstruction.get());
}

void AutoTreat::SetWidgetPrePrepare()
{
	ui.stackedWidget_AutoTreat_Pages->setCurrentWidget(m_pAutoTreatPrePrepare.get());
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

void AutoTreat::SetWidgetTreatBegin()
{
	ui.stackedWidget_AutoTreat_Pages->setCurrentWidget(m_pAutoTreatBegin.get());
}

void AutoTreat::SetWidgetTreatOnGoing()
{
	m_pAutoTreatOnGoing->TimerStart();
	ui.stackedWidget_AutoTreat_Pages->setCurrentWidget(m_pAutoTreatOnGoing.get());
}

void AutoTreat::SetWidgetTreatFinish()
{
	ui.stackedWidget_AutoTreat_Pages->setCurrentWidget(m_pAutoTreatFinish.get());
}

void AutoTreat::FinishReturn()
{
	m_pWindow->SetWidgetHomePage();
}