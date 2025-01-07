#include "AutoTreatAcupointRecog.h"

#include "AutoTreat.h"

AutoTreatAcupointRecog::AutoTreatAcupointRecog(AutoTreat* treat, QWidget *parent)
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

AutoTreatAcupointRecog::~AutoTreatAcupointRecog()
{
}

void AutoTreatAcupointRecog::on_Pushbutton_LastStep_Clicked()
{
	m_pAutoTreat->SetWidgetSetTime();
}

void AutoTreatAcupointRecog::on_Pushbutton_NextStep_Clicked()
{
	m_pAutoTreat->SetWidgetToleranceTest();
}
