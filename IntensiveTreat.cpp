#include "IntensiveTreat.h"

IntensiveTreat::IntensiveTreat(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	m_pIntensiveTreatInstruction.reset(new IntensiveTreatPalliativeCare(this));
	ui.stackedWidget_IntensiveTreat_Pages->addWidget(m_pIntensiveTreatInstruction.get());

	m_pIntensiveTreatSetTime.reset(new IntensiveTreatSetTime(this));
	ui.stackedWidget_IntensiveTreat_Pages->addWidget(m_pIntensiveTreatSetTime.get());

	m_pIntensiveTreatAcupointRecog.reset(new IntensiveTreatAcupointRecog(this));
	ui.stackedWidget_IntensiveTreat_Pages->addWidget(m_pIntensiveTreatAcupointRecog.get());

	// 耐受力测试 wangwenbo 20250105
	m_pIntensiveTreatToleranceTest.reset(new IntensiveTreatToleranceTest(this));
	ui.stackedWidget_IntensiveTreat_Pages->addWidget(m_pIntensiveTreatToleranceTest.get());

	// 重点治疗开始
	m_pIntensiveTreatBegin.reset(new IntensiveTreatBegin(this));
	ui.stackedWidget_IntensiveTreat_Pages->addWidget(m_pIntensiveTreatBegin.get());

	// 重点治疗进行中
	m_pIntensiveTreatOnGoing.reset(new IntensiveTreatOnGoing(this));
	ui.stackedWidget_IntensiveTreat_Pages->addWidget(m_pIntensiveTreatOnGoing.get());

	// 重点治疗结束
	m_pIntensiveTreatFinish.reset(new IntensiveTreatFinish(this));
	ui.stackedWidget_IntensiveTreat_Pages->addWidget(m_pIntensiveTreatFinish.get());

	SetWidgetPalliativeCare();
}

IntensiveTreat::~IntensiveTreat()
{
}

void IntensiveTreat::SetWidgetPalliativeCare()
{
	ui.stackedWidget_IntensiveTreat_Pages->setCurrentWidget(m_pIntensiveTreatInstruction.get());
}

void IntensiveTreat::SetWidgetSetTime()
{
	ui.stackedWidget_IntensiveTreat_Pages->setCurrentWidget(m_pIntensiveTreatSetTime.get());
}

void IntensiveTreat::SetWidgetAcupointRecog()
{
	ui.stackedWidget_IntensiveTreat_Pages->setCurrentWidget(m_pIntensiveTreatAcupointRecog.get());
}

void IntensiveTreat::SetWidgetToleranceTest()
{
	ui.stackedWidget_IntensiveTreat_Pages->setCurrentWidget(m_pIntensiveTreatToleranceTest.get());
}

void IntensiveTreat::SetWidgetBegin()
{
	ui.stackedWidget_IntensiveTreat_Pages->setCurrentWidget(m_pIntensiveTreatBegin.get());
}

void IntensiveTreat::SetWidgetOnGoing()
{
	ui.stackedWidget_IntensiveTreat_Pages->setCurrentWidget(m_pIntensiveTreatOnGoing.get());
}

void IntensiveTreat::SetWidgetFinish()
{
	ui.stackedWidget_IntensiveTreat_Pages->setCurrentWidget(m_pIntensiveTreatFinish.get());
}
