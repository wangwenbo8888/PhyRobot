#include "AutoTreatOnGoing.h"

#include "AutoTreat.h"

AutoTreatOnGoing::AutoTreatOnGoing(AutoTreat* treat, QWidget *parent)
	: m_pAutoTreat(treat)
	, QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_Back, SIGNAL(clicked()), this, SLOT(On_pushButton_Back_Clicked()));
	connect(ui.pushButton_Back, SIGNAL(clicked()), this, SLOT(On_pushButton_Back_Clicked()));

	disconnect(ui.pushButton_WorkContinue, SIGNAL(clicked()), this, SLOT(On_pushButton_WorkContinue_Clicked()));
	connect(ui.pushButton_WorkContinue, SIGNAL(clicked()), this, SLOT(On_pushButton_WorkContinue_Clicked()));

	disconnect(ui.pushButton_WorkPause, SIGNAL(clicked()), this, SLOT(On_pushButton_Pause_Clicked()));
	connect(ui.pushButton_WorkPause, SIGNAL(clicked()), this, SLOT(On_pushButton_Pause_Clicked()));

	disconnect(ui.pushButton_WorkStop, SIGNAL(clicked()), this, SLOT(On_pushButton_Stop_Clicked()));
	connect(ui.pushButton_WorkStop, SIGNAL(clicked()), this, SLOT(On_pushButton_Stop_Clicked()));
}

AutoTreatOnGoing::~AutoTreatOnGoing()
{
}

void AutoTreatOnGoing::On_pushButton_Back_Clicked()
{
	m_pAutoTreat->SetWidgetTreatBegin();
}

void AutoTreatOnGoing::On_pushButton_WorkContinue_Clicked()
{
	//m_pAutoTreat->SetWidgetTreatFinish();
}

void AutoTreatOnGoing::On_pushButton_Pause_Clicked()
{

}

void AutoTreatOnGoing::On_pushButton_Stop_Clicked()
{

}
