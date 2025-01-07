#include "AutoTreatPrePrepare.h"

#include "AutoTreat.h"

AutoTreatPrePrepare::AutoTreatPrePrepare(AutoTreat* treat, QWidget *parent)
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

AutoTreatPrePrepare::~AutoTreatPrePrepare()
{

}

void AutoTreatPrePrepare::on_Pushbutton_NextStep_Clicked()
{
	m_pAutoTreat->SetWidgetSetTime();
}


void AutoTreatPrePrepare::on_Pushbutton_LastStep_Clicked()
{
	m_pAutoTreat->SetWidgetInstruction();
}