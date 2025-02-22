#include "IntensiveTreatSetTime.h"

#include "IntensiveTreat.h"

IntensiveTreatSetTime::IntensiveTreatSetTime(IntensiveTreat* treat, QWidget *parent)
	: m_pIntensiveTreat(treat)
	, QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_LastStep_Clicked()));
	connect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_LastStep_Clicked()));

	disconnect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_NextStep_Clicked()));
	connect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_NextStep_Clicked()));

	disconnect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(on_Pushbutton_Confirm_Clicked()));
	connect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(on_Pushbutton_Confirm_Clicked()));

	ui.pushButton_NextStep->setEnabled(false);
}

IntensiveTreatSetTime::~IntensiveTreatSetTime()
{}

void IntensiveTreatSetTime::on_Pushbutton_LastStep_Clicked()
{
	m_pIntensiveTreat->SetWidgetPalliativeCare();
}

void IntensiveTreatSetTime::on_Pushbutton_NextStep_Clicked()
{
	m_pIntensiveTreat->SetWidgetAcupointRecog();
}

void IntensiveTreatSetTime::on_Pushbutton_Confirm_Clicked()
{
	ui.label_Hook->setPixmap(QPixmap(":/GreenHook.png"));
	ui.pushButton_NextStep->setEnabled(true);
}

void IntensiveTreatSetTime::InitInterface()
{
	ui.label_Hook->setPixmap(QPixmap(":/GrayHook.png"));
	ui.pushButton_NextStep->setEnabled(false);
}