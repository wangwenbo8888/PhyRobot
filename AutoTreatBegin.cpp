#include "AutoTreatBegin.h"

#include "AutoTreat.h"
#include "MyWindow.h"

AutoTreatBegin::AutoTreatBegin(AutoTreat* treat, QWidget *parent)
	: m_pAutoTreat(treat)
	, QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_LastStep_Clicked()));
	connect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_LastStep_Clicked()));

	disconnect(ui.pushButton_WorkBegin, SIGNAL(clicked()), this, SLOT(on_Pushbutton_WorkBegin_Clicked()));
	connect(ui.pushButton_WorkBegin, SIGNAL(clicked()), this, SLOT(on_Pushbutton_WorkBegin_Clicked()));
}

AutoTreatBegin::~AutoTreatBegin()
{
}

void AutoTreatBegin::on_Pushbutton_LastStep_Clicked()
{
	m_pAutoTreat->SetWidgetToleranceTest();
}

void AutoTreatBegin::on_Pushbutton_WorkBegin_Clicked()
{
    m_pAutoTreat->GetWindow()->poweron();

    m_pAutoTreat->SetWidgetTreatOnGoing();

    m_pAutoTreat->GetWindow()->go();

	// m_pAutoTreat->SetWidgetTreatFinish();
}

