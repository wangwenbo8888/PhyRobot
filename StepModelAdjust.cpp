#include "StepModelAdjust.h"

#include "AutoTreatOpenBack.h"

StepModelAdjust::StepModelAdjust(QWidget *parent)
	: m_pMyFatherWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_DistanceDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_DistanceDecr_Clicked()));
	connect(ui.pushButton_DistanceDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_DistanceDecr_Clicked()));

	disconnect(ui.pushButton_DistanceIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_DistanceIncr_Clicked()));
	connect(ui.pushButton_DistanceIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_DistanceIncr_Clicked()));

	disconnect(ui.pushButton_StandbyTimeDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_StandbyTimeDecr_Clicked()));
	connect(ui.pushButton_StandbyTimeDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_StandbyTimeDecr_Clicked()));

	disconnect(ui.pushButton_StandbyTimeIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_StandbyTimeIncr_Clicked()));
	connect(ui.pushButton_StandbyTimeIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_StandbyTimeIncr_Clicked()));

	disconnect(ui.pushButton_IntensityDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityDecr_Clicked()));
	connect(ui.pushButton_IntensityDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityDecr_Clicked()));

	disconnect(ui.pushButton_IntensityIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityIncr_Clicked()));
	connect(ui.pushButton_IntensityIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityIncr_Clicked()));

	disconnect(ui.pushButton_Cancel, SIGNAL(clicked()), this, SLOT(On_pushButton_Cancel_Clicked()));
	connect(ui.pushButton_Cancel, SIGNAL(clicked()), this, SLOT(On_pushButton_Cancel_Clicked()));

	disconnect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(On_pushButton_Confirm_Clicked()));
	connect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(On_pushButton_Confirm_Clicked()));

}

StepModelAdjust::~StepModelAdjust()
{

}

void StepModelAdjust::ResumePara()
{
	ui.label_StepDistanceValue->setText("2");
	ui.label_StandbyTimeValue->setText("00:03:00");
	ui.label_IntensityValue->setText("30");
}

void StepModelAdjust::On_pushButton_DistanceDecr_Clicked()
{
	if (ui.label_StepDistanceValue->text().toInt() <= 1)
	{
		ui.label_StepDistanceValue->setText("1");
	}
	else
	{
		int value = ui.label_StepDistanceValue->text().toInt();
		ui.label_StepDistanceValue->setText(QString::number(value - 1));
	}
}
void StepModelAdjust::On_pushButton_DistanceIncr_Clicked()
{
	if (ui.label_StepDistanceValue->text().toInt() >= 5)
	{
		ui.label_StepDistanceValue->setText("5");
	}
	else
	{
		int speed = ui.label_StepDistanceValue->text().toInt();
		ui.label_StepDistanceValue->setText(QString::number(speed + 1));
	}
}

void StepModelAdjust::On_pushButton_StandbyTimeDecr_Clicked()
{
	QString time = ui.label_StandbyTimeValue->text();
	int minute = time.mid(4, 1).toInt();
	if (minute <= 1)
	{
		ui.label_StandbyTimeValue->setText("00:01:00");
	}
	else
	{
		time = "00:0";
		time.append(QString::number(minute - 1));
		time.append(":00");
		ui.label_StandbyTimeValue->setText(time);
	}
}

void StepModelAdjust::On_pushButton_StandbyTimeIncr_Clicked()
{
	QString time = ui.label_StandbyTimeValue->text();
	int minute = time.mid(4, 1).toInt();
	if (minute >= 3)
	{
		ui.label_StandbyTimeValue->setText("00:03:00");
	}
	else
	{
		time = "00:0";
		time.append(QString::number(minute + 1));
		time.append(":00");
		ui.label_StandbyTimeValue->setText(time);
	}
}

void StepModelAdjust::On_pushButton_IntensityDecr_Clicked()
{
	if (ui.label_IntensityValue->text().toInt() <= 1)
	{
		ui.label_IntensityValue->setText("1");
	}
	else
	{
		int value = ui.label_IntensityValue->text().toInt();
		ui.label_IntensityValue->setText(QString::number(value - 1));
	}
}
void StepModelAdjust::On_pushButton_IntensityIncr_Clicked()
{
	if (ui.label_IntensityValue->text().toInt() >= 99)
	{
		ui.label_IntensityValue->setText("99");
	}
	else
	{
		int value = ui.label_IntensityValue->text().toInt();
		ui.label_IntensityValue->setText(QString::number(value + 1));
	}
}

void StepModelAdjust::On_pushButton_Cancel_Clicked()
{
	ResumePara();
}

void StepModelAdjust::On_pushButton_Confirm_Clicked()
{
	int dist = ui.label_StepDistanceValue->text().toInt();
	QString time = ui.label_StandbyTimeValue->text();
	int minute = time.mid(4, 1).toInt();
	int intensity = ui.label_IntensityValue->text().toInt();
	((AutoTreatOpenBack*)m_pMyFatherWidget)->SetStepPara(dist,minute,intensity);
	((AutoTreatOpenBack*)m_pMyFatherWidget)->SetStepModelFlag();
	ResumePara();

	this->hide();
}
