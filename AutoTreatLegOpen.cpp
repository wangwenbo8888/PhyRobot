#include "AutoTreatLegOpen.h"

#include "AutoTreat.h"
AutoTreatLegOpen::AutoTreatLegOpen(AutoTreat* treat,QWidget *parent)
	:m_pAutoTreat(treat)
	,QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	m_iLegOpenTime = 30;
	m_iLegOpenIntensity = 50;

	disconnect(ui.pushButton_IntensityDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityDecr_Clicked()));
	connect(ui.pushButton_IntensityDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityDecr_Clicked()));

	disconnect(ui.pushButton_IntensityIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityIncr_Clicked()));
	connect(ui.pushButton_IntensityIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityIncr_Clicked()));

	disconnect(ui.pushButton_TreatTimeDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_TreatTimeDecr_Clicked()));
	connect(ui.pushButton_TreatTimeDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_TreatTimeDecr_Clicked()));

	disconnect(ui.pushButton_TreatTimeIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_TreatTimeIncr_Clicked()));
	connect(ui.pushButton_TreatTimeIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_TreatTimeIncr_Clicked()));

	disconnect(ui.pushButton_BackToHome, SIGNAL(clicked()), this, SLOT(On_pushButton_BackToHome_Clicked()));
	connect(ui.pushButton_BackToHome, SIGNAL(clicked()), this, SLOT(On_pushButton_BackToHome_Clicked()));

	disconnect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(On_pushButton_LastStep_Clicked()));
	connect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(On_pushButton_LastStep_Clicked()));

	disconnect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(On_pushButton_Confirm_Clicked()));
	connect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(On_pushButton_Confirm_Clicked()));

	disconnect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(On_pushButton_NextStep_Clicked()));
	connect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(On_pushButton_NextStep_Clicked()));
}

AutoTreatLegOpen::~AutoTreatLegOpen()
{

}

void AutoTreatLegOpen::On_pushButton_IntensityDecr_Clicked()
{
	if (ui.label_Intenstiy->text().toInt() <= 1)
	{
		ui.label_Intenstiy->setText("1");
	}
	else
	{
		int value = ui.label_Intenstiy->text().toInt();
		ui.label_Intenstiy->setText(QString::number(value - 1));
	}
}

void AutoTreatLegOpen::On_pushButton_IntensityIncr_Clicked()
{
	if (ui.label_Intenstiy->text().toInt() >= 99)
	{
		ui.label_Intenstiy->setText("99");
	}
	else
	{
		int speed = ui.label_Intenstiy->text().toInt();
		ui.label_Intenstiy->setText(QString::number(speed + 1));
	}
}

void AutoTreatLegOpen::On_pushButton_TreatTimeDecr_Clicked()
{
	QString time = ui.label_Time->text();
	int minute = time.mid(3, 2).toInt();
	if (minute <= 5)
	{
		ui.label_Time->setText("00:05:00");
	}
	else if (minute<=10)
	{
		time = "00:0";
		time.append(QString::number(minute - 1));
		time.append(":00");
		ui.label_Time->setText(time);
	}
	else
	{
		time = "00:";
		time.append(QString::number(minute - 1));
		time.append(":00");
		ui.label_Time->setText(time);
	}
}

void AutoTreatLegOpen::On_pushButton_TreatTimeIncr_Clicked()
{
	QString time = ui.label_Time->text();
	int minute = time.mid(3, 2).toInt();
	if (minute >= 30)
	{
		ui.label_Time->setText("00:30:00");
	}
	else if (minute>=9)
	{
		time = "00:";
		time.append(QString::number(minute + 1));
		time.append(":00");
		ui.label_Time->setText(time);
	}
	else
	{
		time = "00:0";
		time.append(QString::number(minute + 1));
		time.append(":00");
		ui.label_Time->setText(time);
	}
}

void AutoTreatLegOpen::On_pushButton_BackToHome_Clicked()
{
}

void AutoTreatLegOpen::On_pushButton_LastStep_Clicked()
{

}

void AutoTreatLegOpen::On_pushButton_Confirm_Clicked()
{
	QString time = ui.label_Time->text();
	m_iLegOpenTime = time.mid(3, 2).toInt();
	m_iLegOpenIntensity = ui.label_Intenstiy->text().toInt();

}

void AutoTreatLegOpen::On_pushButton_NextStep_Clicked()
{
	ui.label_Time->setText("00:30:00");
	ui.label_Intenstiy->setText("50");
	m_pAutoTreat->SetWidgetTreatLegOpenBegin();
}
