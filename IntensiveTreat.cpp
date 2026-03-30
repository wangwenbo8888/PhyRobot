#include "IntensiveTreat.h"

#include "MyWindow.h"

IntensiveTreat::IntensiveTreat(MyWindow* window, QWidget *parent)
	: m_pWindow(window)
	, QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	m_pIntensiveTreatPalliativeCare.reset(new IntensiveTreatPalliativeCare(this));
	ui.stackedWidget_ManualTreat_Pages->addWidget(m_pIntensiveTreatPalliativeCare.get());

	m_pIntensiveTreatSetTime.reset(new IntensiveTreatSetTime(this));
	ui.stackedWidget_ManualTreat_Pages->addWidget(m_pIntensiveTreatSetTime.get());

	m_pIntensiveTreatAcupointRecog.reset(new IntensiveTreatAcupointRecog(this));
	ui.stackedWidget_ManualTreat_Pages->addWidget(m_pIntensiveTreatAcupointRecog.get());

	// 耐受力测试 wangwenbo 20250105
	m_pIntensiveTreatToleranceTest.reset(new IntensiveTreatToleranceTest(this));
	ui.stackedWidget_ManualTreat_Pages->addWidget(m_pIntensiveTreatToleranceTest.get());

	// 重点治疗开始
	m_pIntensiveTreatBegin.reset(new IntensiveTreatBegin(this));
	ui.stackedWidget_ManualTreat_Pages->addWidget(m_pIntensiveTreatBegin.get());

	// 重点治疗进行中
	m_pIntensiveTreatOnGoing.reset(new IntensiveTreatOnGoing(this));
	ui.stackedWidget_ManualTreat_Pages->addWidget(m_pIntensiveTreatOnGoing.get());

	// 重点治疗结束
	m_pIntensiveTreatFinish.reset(new IntensiveTreatFinish(this));
	ui.stackedWidget_ManualTreat_Pages->addWidget(m_pIntensiveTreatFinish.get());

	m_pManualTreatDragBegin.reset(new ManualTreatDragBegin(m_pWindow, this));
	ui.stackedWidget_ManualTreat_Pages->addWidget(m_pManualTreatDragBegin.get());

	m_pManualTreatHandleBegin.reset(new ManualTreatHandleBegin(m_pWindow, this));
	ui.stackedWidget_ManualTreat_Pages->addWidget(m_pManualTreatHandleBegin.get());

	m_pManualTreatFinish.reset(new ManualTreatFinish(this));
	ui.stackedWidget_ManualTreat_Pages->addWidget(m_pManualTreatFinish.get());

	SetWidgetAcupointRecog();
}

IntensiveTreat::~IntensiveTreat()
{
}

void IntensiveTreat::SetWidgetPalliativeCare()
{
	m_pIntensiveTreatPalliativeCare->InitInterface();
	ui.stackedWidget_ManualTreat_Pages->setCurrentWidget(m_pIntensiveTreatPalliativeCare.get());
}

void IntensiveTreat::SetWidgetSetTime()
{
	m_pIntensiveTreatSetTime->InitInterface();
	ui.stackedWidget_ManualTreat_Pages->setCurrentWidget(m_pIntensiveTreatSetTime.get());
}

void IntensiveTreat::SetWidgetAcupointRecog()
{
	ui.label_ModelSetFlag->show();
	ui.label_AdjStartFlag->hide();
	ui.label_FinishFlag->hide();

	ui.stackedWidget_ManualTreat_Pages->setCurrentWidget(m_pIntensiveTreatAcupointRecog.get());
}

void IntensiveTreat::SetWidgetToleranceTest()
{
	ui.stackedWidget_ManualTreat_Pages->setCurrentWidget(m_pIntensiveTreatToleranceTest.get());
}

MyWindow* IntensiveTreat::GetWindow()
{
    return m_pWindow;
}

void IntensiveTreat::SetWidgetBegin()
{
	ui.stackedWidget_ManualTreat_Pages->setCurrentWidget(m_pIntensiveTreatBegin.get());
}

void IntensiveTreat::SetWidgetOnGoing()
{
	m_pIntensiveTreatOnGoing->TimerStart();
	ui.stackedWidget_ManualTreat_Pages->setCurrentWidget(m_pIntensiveTreatOnGoing.get());
}

void IntensiveTreat::SetWidgetDragBegin()
{
	ui.label_ModelSetFlag->hide();
	ui.label_AdjStartFlag->show();
	ui.label_FinishFlag->hide();

	ui.stackedWidget_ManualTreat_Pages->setCurrentWidget(m_pManualTreatDragBegin.get());
}

void IntensiveTreat::SetWidgetHandleBegin()
{
	ui.label_ModelSetFlag->hide();
	ui.label_AdjStartFlag->show();
	ui.label_FinishFlag->hide();

	ui.stackedWidget_ManualTreat_Pages->setCurrentWidget(m_pManualTreatHandleBegin.get());
}

void IntensiveTreat::SetWidgetFinish()
{
	ui.label_ModelSetFlag->hide();
	ui.label_AdjStartFlag->hide();
	ui.label_FinishFlag->show();

	ui.stackedWidget_ManualTreat_Pages->setCurrentWidget(m_pManualTreatFinish.get());
}

void IntensiveTreat::FinishReturn()
{
	m_pWindow->SetWidgetHomePage();
}
