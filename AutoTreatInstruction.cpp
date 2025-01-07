#include "AutoTreatInstruction.h"

#include "AutoTreat.h"

AutoTreatInstruction::AutoTreatInstruction(AutoTreat* treat,QWidget *parent)
	: m_pAutoTreat(treat)
	, QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(On_pushButton_NextStep_Clicked()));
	connect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(On_pushButton_NextStep_Clicked()));
}

AutoTreatInstruction::~AutoTreatInstruction()
{
}

void AutoTreatInstruction::On_pushButton_NextStep_Clicked()
{
	m_pAutoTreat->SetWidgetPrePrepare();
}
