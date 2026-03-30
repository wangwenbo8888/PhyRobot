#include "AutoTreatToleranceTest.h"

#include "AutoTreat.h"

AutoTreatToleranceTest::AutoTreatToleranceTest(AutoTreat* treat, QWidget *parent)
	: m_pAutoTreat(treat)
	, QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_LastStep_Clicked()));
	connect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_LastStep_Clicked()));

	disconnect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_NextStep_Clicked()));
	connect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_NextStep_Clicked()));
}

AutoTreatToleranceTest::~AutoTreatToleranceTest()
{
}

void AutoTreatToleranceTest::on_Pushbutton_LastStep_Clicked()
{
	m_pAutoTreat->SetWidgetAcupointRecog();
}

void AutoTreatToleranceTest::on_Pushbutton_NextStep_Clicked()
{
	//m_pAutoTreat->SetWidgetTreatBegin();
}
