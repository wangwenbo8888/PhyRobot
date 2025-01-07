#include "AutoTreatSetTime.h"

#include "AutoTreat.h"

AutoTreatSetTime::AutoTreatSetTime(AutoTreat* treat, QWidget *parent)
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

AutoTreatSetTime::~AutoTreatSetTime()
{
}

void AutoTreatSetTime::on_Pushbutton_LastStep_Clicked()
{
	m_pAutoTreat->SetWidgetPrePrepare();
}

void AutoTreatSetTime::on_Pushbutton_NextStep_Clicked()
{
	m_pAutoTreat->SetWidgetAcupointRecog();
}