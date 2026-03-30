#include "ZhengjixueweiAdj.h"

#include "AutoTreatLegUnblock.h"
#include "AutoTreatShoulderUnblock.h"

ZhengjixueweiAdj::ZhengjixueweiAdj(QWidget *parent)
	: m_pMyFatherWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_Cancel, SIGNAL(clicked()), this, SLOT(On_pushButton_Cancel_Clicked()));
	connect(ui.pushButton_Cancel, SIGNAL(clicked()), this, SLOT(On_pushButton_Cancel_Clicked()));

	disconnect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(On_pushButton_Confirm_Clicked()));
	connect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(On_pushButton_Confirm_Clicked()));

	disconnect(ui.pushButton_IntensityDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityDecr_Clicked()));
	connect(ui.pushButton_IntensityDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityDecr_Clicked()));

	disconnect(ui.pushButton_IntensityIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityIncr_Clicked()));
	connect(ui.pushButton_IntensityIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityIncr_Clicked()));

	disconnect(ui.pushButton_TimerIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_TimerIncr_Clicked()));
	connect(ui.pushButton_TimerIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_TimerIncr_Clicked()));

	disconnect(ui.pushButton_TimerDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_TimerDecr_Clicked()));
	connect(ui.pushButton_TimerDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_TimerDecr_Clicked()));
}

ZhengjixueweiAdj::~ZhengjixueweiAdj()
{

}

void ZhengjixueweiAdj::SetXueweiType(XUEWEI_TYPE type)
{
	m_eType = type;
}

void ZhengjixueweiAdj::SetWidgetType(WIDGET_TYPE type)
{
	m_eWidget = type;
}

void ZhengjixueweiAdj::ResumePara()
{
	ui.label_TimerValue->setText("00:01:00");
	ui.label_IntensityValue->setText("1");
	ui.label->setText(QStringLiteral("开始穴位治疗调节"));
}

void ZhengjixueweiAdj::On_pushButton_TimerDecr_Clicked()
{
	QString time = ui.label_TimerValue->text();
	int minute = time.mid(4, 1).toInt();
	if (minute <= 1)
	{
		ui.label_TimerValue->setText("00:01:00");
	}
	else
	{
		time = "00:0";
		time.append(QString::number(minute - 1));
		time.append(":00");
		ui.label_TimerValue->setText(time);
	}
}

void ZhengjixueweiAdj::On_pushButton_TimerIncr_Clicked()
{
	QString time = ui.label_TimerValue->text();
	int minute = time.mid(4, 1).toInt();
	if (minute >= 5)
	{
		ui.label_TimerValue->setText("00:05:00");
	}
	else
	{
		time = "00:0";
		time.append(QString::number(minute + 1));
		time.append(":00");
		ui.label_TimerValue->setText(time);
	}
}

void ZhengjixueweiAdj::On_pushButton_IntensityDecr_Clicked()
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

void ZhengjixueweiAdj::On_pushButton_IntensityIncr_Clicked()
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

void ZhengjixueweiAdj::On_pushButton_Cancel_Clicked()
{
	this->hide();
	ResumePara();
}

void ZhengjixueweiAdj::On_pushButton_Confirm_Clicked()
{
	QString time = ui.label_TimerValue->text();
	int minute = time.mid(4, 1).toInt();
	int intensity = ui.label_IntensityValue->text().toInt();
	if (m_eWidget == LEG_UNBLOCK)
	{
		((AutoTreatLegUnblock*)m_pMyFatherWidget)->SetZhengjiPara(m_eType, minute, intensity);
	}
	else if (m_eWidget == SHOULDER_UNBLOCK)
	{
		((AutoTreatShoulderUnblock*)m_pMyFatherWidget)->SetZhengjixueweiPara(m_eType, minute, intensity);
	}

	this->hide();
	ResumePara();
}
