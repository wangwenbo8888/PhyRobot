#include "MoveSpeedAdjust.h"

#include "AutoTreatOpenBack.h"

MoveSpeedAdjust::MoveSpeedAdjust(QWidget *parent)
	: m_pFatherWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_SpeedDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_SpeedDecr_Clicked()));
	connect(ui.pushButton_SpeedDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_SpeedDecr_Clicked()));

	disconnect(ui.pushButton_SpeedIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_SpeedIncr_Clicked()));
	connect(ui.pushButton_SpeedIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_SpeedIncr_Clicked()));

	disconnect(ui.pushButton_Cancel, SIGNAL(clicked()), this, SLOT(On_pushButton_Cancel_Clicked()));
	connect(ui.pushButton_Cancel, SIGNAL(clicked()), this, SLOT(On_pushButton_Cancel_Clicked()));

	disconnect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(On_pushButton_Confirm_Clicked()));
	connect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(On_pushButton_Confirm_Clicked()));
}

MoveSpeedAdjust::~MoveSpeedAdjust()
{

}

void MoveSpeedAdjust::ResumePara()
{
	ui.label_SpeedValue->setText("1");
}

void MoveSpeedAdjust::On_pushButton_Cancel_Clicked()
{
	ResumePara();
	this->hide();
}

void MoveSpeedAdjust::On_pushButton_Confirm_Clicked()
{
	int speed = ui.label_SpeedValue->text().toInt();
	((AutoTreatOpenBack*)m_pFatherWidget)->SetSpeed(speed);
	((AutoTreatOpenBack*)m_pFatherWidget)->SetContinueModelFlag();
	ResumePara();

	this->hide();
}

void MoveSpeedAdjust::On_pushButton_SpeedDecr_Clicked()
{
	if (ui.label_SpeedValue->text().toInt() <= 1)
	{
		ui.label_SpeedValue->setText("1");
	}
	else
	{
		int value = ui.label_SpeedValue->text().toInt();
		ui.label_SpeedValue->setText(QString::number(value - 1));
	}
}

void MoveSpeedAdjust::On_pushButton_SpeedIncr_Clicked()
{
	if (ui.label_SpeedValue->text().toInt() >= 5)
	{
		ui.label_SpeedValue->setText("5");
	}
	else
	{
		int speed = ui.label_SpeedValue->text().toInt();
		ui.label_SpeedValue->setText(QString::number(speed + 1));
	}
}
