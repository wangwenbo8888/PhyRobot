#include "IntensiveTreatAcupointRecog.h"

#include "IntensiveTreat.h"

IntensiveTreatAcupointRecog::IntensiveTreatAcupointRecog(IntensiveTreat* treat, QWidget *parent)
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

IntensiveTreatAcupointRecog::~IntensiveTreatAcupointRecog()
{
}

void IntensiveTreatAcupointRecog::on_Pushbutton_LastStep_Clicked()
{
	m_pIntensiveTreat->SetWidgetSetTime();
}

void IntensiveTreatAcupointRecog::on_Pushbutton_NextStep_Clicked()
{
	m_pIntensiveTreat->SetWidgetToleranceTest();
}
