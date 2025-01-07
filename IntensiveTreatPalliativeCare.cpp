#include "IntensiveTreatPalliativeCare.h"

#include "IntensiveTreat.h"

IntensiveTreatPalliativeCare::IntensiveTreatPalliativeCare(IntensiveTreat* treat, QWidget *parent)
	: m_pIntensiveTreat(treat)
	, QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_Back, SIGNAL(clicked()), this, SLOT(on_Pushbutton_Back_Clicked()));
	connect(ui.pushButton_Back, SIGNAL(clicked()), this, SLOT(on_Pushbutton_Back_Clicked()));

	disconnect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_NextStep_Clicked()));
	connect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_NextStep_Clicked()));
}

IntensiveTreatPalliativeCare::~IntensiveTreatPalliativeCare()
{
}

void IntensiveTreatPalliativeCare::on_Pushbutton_Back_Clicked()
{
	//m_pIntensiveTreat->SetWidgetSetTime();
}

void IntensiveTreatPalliativeCare::on_Pushbutton_NextStep_Clicked()
{
	m_pIntensiveTreat->SetWidgetSetTime();
}
