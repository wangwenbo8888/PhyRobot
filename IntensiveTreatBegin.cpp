#include "IntensiveTreatBegin.h"

#include "IntensiveTreat.h"

IntensiveTreatBegin::IntensiveTreatBegin(IntensiveTreat* treat, QWidget *parent)
	: m_pIntensiveTreat(treat)
	, QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_LastStep_Clicked()));
	connect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_LastStep_Clicked()));

	//disconnect(ui.pushButton_StartOrStop, SIGNAL(clicked()), this, SLOT(on_Pushbutton_StartOrStop_Clicked()));
	//connect(ui.pushButton_StartOrStop, SIGNAL(clicked()), this, SLOT(on_Pushbutton_StartOrStop_Clicked()));
}

IntensiveTreatBegin::~IntensiveTreatBegin()
{
}

void IntensiveTreatBegin::on_Pushbutton_LastStep_Clicked()
{
	m_pIntensiveTreat->SetWidgetToleranceTest();
}

void IntensiveTreatBegin::on_Pushbutton_StartOrStop_Clicked()
{
	m_pIntensiveTreat->SetWidgetOnGoing();
}
