#include "IntensiveTreatToleranceTest.h"

#include "IntensiveTreat.h"

IntensiveTreatToleranceTest::IntensiveTreatToleranceTest(IntensiveTreat* treat, QWidget *parent)
	: m_pIntensiveTreat(treat)
	, QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_LastStep_Clicked()));
	connect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_LastStep_Clicked()));

	disconnect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_NextStep_Clicked()));
	connect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_NextStep_Clicked()));
}

IntensiveTreatToleranceTest::~IntensiveTreatToleranceTest()
{
}

void IntensiveTreatToleranceTest::on_Pushbutton_LastStep_Clicked()
{
	m_pIntensiveTreat->SetWidgetAcupointRecog();
}

void IntensiveTreatToleranceTest::on_Pushbutton_NextStep_Clicked()
{
	m_pIntensiveTreat->SetWidgetBegin();
}
