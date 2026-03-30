#include "PathwayAdjust.h"

#include "AutoTreatOpenBack.h"

PathwayAdjust::PathwayAdjust(QWidget *parent)
	: m_pMyFatherWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_IntensityDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityDecr_Clicked()));
	connect(ui.pushButton_IntensityDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityDecr_Clicked()));

	disconnect(ui.pushButton_IntensityIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityIncr_Clicked()));
	connect(ui.pushButton_IntensityIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityIncr_Clicked()));

	disconnect(ui.pushButton_Cancel, SIGNAL(clicked()), this, SLOT(On_pushButton_Cancel_Clicked()));
	connect(ui.pushButton_Cancel, SIGNAL(clicked()), this, SLOT(On_pushButton_Cancel_Clicked()));

	disconnect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(On_pushButton_Confirm_Clicked()));
	connect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(On_pushButton_Confirm_Clicked()));
}

PathwayAdjust::~PathwayAdjust()
{
}

void PathwayAdjust::ResumePara()
{
	ui.label_IntensityValue->setText("1");
}

void PathwayAdjust::SetXueweiType(XUEWEI_TYPE type)
{
	m_eType = type;
}

void PathwayAdjust::On_pushButton_Cancel_Clicked()
{
	ResumePara();
	this->hide();
}

void PathwayAdjust::On_pushButton_Confirm_Clicked()
{
	int intensity = ui.label_IntensityValue->text().toInt();
	((AutoTreatOpenBack*)m_pMyFatherWidget)->SetTujingPara(m_eType, intensity);
	ResumePara();
	this->hide();
}

void PathwayAdjust::On_pushButton_IntensityDecr_Clicked()
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

void PathwayAdjust::On_pushButton_IntensityIncr_Clicked()
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
