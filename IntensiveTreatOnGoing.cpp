#include "IntensiveTreatOnGoing.h"

#include "IntensiveTreat.h"

IntensiveTreatOnGoing::IntensiveTreatOnGoing(IntensiveTreat* treat, QWidget *parent)
	: m_pIntensiveTreat(treat)
	, QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_Back, SIGNAL(clicked()), this, SLOT(On_pushButton_Back_Clicked()));
	connect(ui.pushButton_Back, SIGNAL(clicked()), this, SLOT(On_pushButton_Back_Clicked()));

	disconnect(ui.pushButton_WorkContinue, SIGNAL(clicked()), this, SLOT(On_pushButton_WorkContinue_Clicked()));
	connect(ui.pushButton_WorkContinue, SIGNAL(clicked()), this, SLOT(On_pushButton_WorkContinue_Clicked()));
}

IntensiveTreatOnGoing::~IntensiveTreatOnGoing()
{
}

void IntensiveTreatOnGoing::On_pushButton_Back_Clicked()
{
	m_pIntensiveTreat->SetWidgetBegin();
}

void IntensiveTreatOnGoing::On_pushButton_WorkContinue_Clicked()
{
	m_pIntensiveTreat->SetWidgetFinish();
}
