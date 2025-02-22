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

	disconnect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(on_Pushbutton_Confirm_Clicked()));
	connect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(on_Pushbutton_Confirm_Clicked()));

	ui.pushButton_NextStep->setEnabled(false);
}

IntensiveTreatPalliativeCare::~IntensiveTreatPalliativeCare()
{
}

void IntensiveTreatPalliativeCare::InitInterface()
{
	ui.pushButton_NextStep->setEnabled(false);
	ui.label_Hook->setPixmap(QPixmap(":/GrayHook.png"));
}

void IntensiveTreatPalliativeCare::on_Pushbutton_Back_Clicked()
{
	//m_pIntensiveTreat->SetWidgetSetTime();
}

void IntensiveTreatPalliativeCare::on_Pushbutton_NextStep_Clicked()
{
	m_pIntensiveTreat->SetWidgetSetTime();
}

void IntensiveTreatPalliativeCare::on_Pushbutton_Confirm_Clicked()
{
	ui.label_Hook->setPixmap(QPixmap(":/GreenHook.png"));
	ui.pushButton_NextStep->setEnabled(true);
}