#include "AutoTreatShoulderUnblock.h"

#include "AutoTreat.h"

AutoTreatShoulderUnblock::AutoTreatShoulderUnblock(AutoTreat* treat,QWidget *parent)
	:m_pAutoTreat(treat)
	,QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	m_iFengfu1Time = 0;
	m_iFengchi1Time = 0;
	m_iFengfu2Time = 0;
	m_iFengchi2Time = 0;

	m_pZhengjixueweiAdj.reset(new ZhengjixueweiAdj(this));
	m_pZhengjixueweiAdj->SetWidgetType(SHOULDER_UNBLOCK);
	m_pZhengjixueweiAdj->hide();

	disconnect(ui.pushButton_Fengfu1, SIGNAL(clicked()), this, SLOT(On_pushButton_Fengfu1_Clicked()));
	connect(ui.pushButton_Fengfu1, SIGNAL(clicked()), this, SLOT(On_pushButton_Fengfu1_Clicked()));

	disconnect(ui.pushButton_Fengchi1, SIGNAL(clicked()), this, SLOT(On_pushButton_Fengchi1_Clicked()));
	connect(ui.pushButton_Fengchi1, SIGNAL(clicked()), this, SLOT(On_pushButton_Fengchi1_Clicked()));

	disconnect(ui.pushButton_Youneiguan1, SIGNAL(clicked()), this, SLOT(On_pushButton_Youneiguan1_Clicked()));
	connect(ui.pushButton_Youneiguan1, SIGNAL(clicked()), this, SLOT(On_pushButton_Youneiguan1_Clicked()));

	disconnect(ui.pushButton_Youwaiguan1, SIGNAL(clicked()), this, SLOT(On_pushButton_Youwaiguan1_Clicked()));
	connect(ui.pushButton_Youwaiguan1, SIGNAL(clicked()), this, SLOT(On_pushButton_Youwaiguan1_Clicked()));

	disconnect(ui.pushButton_Fengfu2, SIGNAL(clicked()), this, SLOT(On_pushButton_Fengfu2_Clicked()));
	connect(ui.pushButton_Fengfu2, SIGNAL(clicked()), this, SLOT(On_pushButton_Fengfu2_Clicked()));

	disconnect(ui.pushButton_Fengchi2, SIGNAL(clicked()), this, SLOT(On_pushButton_Fengchi2_Clicked()));
	connect(ui.pushButton_Fengchi2, SIGNAL(clicked()), this, SLOT(On_pushButton_Fengchi2_Clicked()));

	disconnect(ui.pushButton_Zuoneiguan1, SIGNAL(clicked()), this, SLOT(On_pushButton_Zuoneiguan1_Clicked()));
	connect(ui.pushButton_Zuoneiguan1, SIGNAL(clicked()), this, SLOT(On_pushButton_Zuoneiguan1_Clicked()));

	disconnect(ui.pushButton_Zuowaiguan1, SIGNAL(clicked()), this, SLOT(On_pushButton_Zuowaiguan1_Clicked()));
	connect(ui.pushButton_Zuowaiguan1, SIGNAL(clicked()), this, SLOT(On_pushButton_Zuowaiguan1_Clicked()));

	disconnect(ui.pushButton_TimeDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_TimeDecr_Clicked()));
	connect(ui.pushButton_TimeDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_TimeDecr_Clicked()));

	disconnect(ui.pushButton_TimeIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_TimeIncr_Clicked()));
	connect(ui.pushButton_TimeIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_TimeIncr_Clicked()));

	disconnect(ui.pushButton_SpeedDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_SpeedDecr_Clicked()));
	connect(ui.pushButton_SpeedDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_SpeedDecr_Clicked()));

	disconnect(ui.pushButton_SpeedIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_SpeedIncr_Clicked()));
	connect(ui.pushButton_SpeedIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_SpeedIncr_Clicked()));

	disconnect(ui.pushButton_BackToHome, SIGNAL(clicked()), this, SLOT(On_pushButton_BackToHome_Clicked()));
	connect(ui.pushButton_BackToHome, SIGNAL(clicked()), this, SLOT(On_pushButton_BackToHome_Clicked()));

	disconnect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(On_pushButton_LastStep_Clicked()));
	connect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(On_pushButton_LastStep_Clicked()));

	disconnect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(On_pushButton_Confirm_Clicked()));
	connect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(On_pushButton_Confirm_Clicked()));

	disconnect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(On_pushButton_NextStep_Clicked()));
	connect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(On_pushButton_NextStep_Clicked()));

	ui.pushButton_TimeDecr->hide();
	ui.pushButton_TimeIncr->hide();

}

AutoTreatShoulderUnblock::~AutoTreatShoulderUnblock()
{

}

void AutoTreatShoulderUnblock::SetZhengjixueweiPara(XUEWEI_TYPE type, int time, int intensity)
{
	if (type == FENGFU1)
	{
		m_iFengfu1Time = time;
		m_iFengfu1Intensity = intensity;
	}
	else if (type == FENGCHI1)
	{
		m_iFengchi1Time = time;
		m_iFengchi1Intensity = intensity;
	}
	else if (type == FENGFU2)
	{
		m_iFengfu2Time = time;
		m_iFengfu2Intensity = intensity;
	}
	else if (type == FENGCHI2)
	{
		m_iFengchi2Time = time;
		m_iFengchi2Intensity = intensity;
	}

	m_iTotalTime = m_iFengfu1Time + m_iFengchi1Time + m_iFengfu2Time + m_iFengchi2Time;
	QString str("00:");
	if (m_iTotalTime >= 10)
	{
		str = QString::number(m_iTotalTime);
	}
	else
	{
		str = "0";
		str.append(QString::number(m_iTotalTime));
	}
	str.append(":00");
	ui.label_Time->setText(str);
}

void AutoTreatShoulderUnblock::On_pushButton_Fengfu1_Clicked()
{
	ui.pushButton_Fengfu1->setIcon(QIcon(":/FengfuPressed.png"));
	m_pZhengjixueweiAdj->move(1284, 322);
	m_pZhengjixueweiAdj->setWindowModality(Qt::ApplicationModal);
	m_pZhengjixueweiAdj->SetXueweiType(FENGFU1);
	m_pZhengjixueweiAdj->show();
}

void AutoTreatShoulderUnblock::On_pushButton_Fengchi1_Clicked()
{
	ui.pushButton_Fengchi1->setIcon(QIcon(":/FengchiPushed.png"));
	m_pZhengjixueweiAdj->move(1284, 322);
	m_pZhengjixueweiAdj->setWindowModality(Qt::ApplicationModal);
	m_pZhengjixueweiAdj->SetXueweiType(FENGCHI1);
	m_pZhengjixueweiAdj->show();
}

void AutoTreatShoulderUnblock::On_pushButton_Youneiguan1_Clicked()
{

}
void AutoTreatShoulderUnblock::On_pushButton_Youwaiguan1_Clicked()
{

}
void AutoTreatShoulderUnblock::On_pushButton_Fengfu2_Clicked()
{
	ui.pushButton_Fengfu2->setIcon(QIcon(":/FengfuPressed.png"));
	m_pZhengjixueweiAdj->move(1284, 322);
	m_pZhengjixueweiAdj->setWindowModality(Qt::ApplicationModal);
	m_pZhengjixueweiAdj->SetXueweiType(FENGFU2);
	m_pZhengjixueweiAdj->show();
}
void AutoTreatShoulderUnblock::On_pushButton_Fengchi2_Clicked()
{
	ui.pushButton_Fengchi2->setIcon(QIcon(":/FengchiPushed.png"));
	m_pZhengjixueweiAdj->move(1284, 322);
	m_pZhengjixueweiAdj->setWindowModality(Qt::ApplicationModal);
	m_pZhengjixueweiAdj->SetXueweiType(FENGCHI2);
	m_pZhengjixueweiAdj->show();
}
void AutoTreatShoulderUnblock::On_pushButton_Zuoneiguan1_Clicked()
{

}
void AutoTreatShoulderUnblock::On_pushButton_Zuowaiguan1_Clicked()
{

}

void AutoTreatShoulderUnblock::On_pushButton_TimeDecr_Clicked()
{
	QString time = ui.label_Time->text();
	int minute = time.mid(4, 1).toInt();
	if (minute <= 1)
	{
		ui.label_Time->setText("00:01:00");
	}
	else
	{
		time = "00:0";
		time.append(QString::number(minute - 1));
		time.append(":00");
		ui.label_Time->setText(time);
	}
}
void AutoTreatShoulderUnblock::On_pushButton_TimeIncr_Clicked()
{
	QString time = ui.label_Time->text();
	int minute = time.mid(4, 1).toInt();
	if (minute >= 3)
	{
		ui.label_Time->setText("00:03:00");
	}
	else
	{
		time = "00:0";
		time.append(QString::number(minute + 1));
		time.append(":00");
		ui.label_Time->setText(time);
	}
}

void AutoTreatShoulderUnblock::On_pushButton_SpeedDecr_Clicked()
{
	if (ui.label_Speed_2->text().toInt() <= 1)
	{
		ui.label_Speed_2->setText("1");
	}
	else
	{
		int value = ui.label_Speed_2->text().toInt();
		ui.label_Speed_2->setText(QString::number(value - 1));
	}
}
void AutoTreatShoulderUnblock::On_pushButton_SpeedIncr_Clicked()
{
	if (ui.label_Speed_2->text().toInt() >= 99)
	{
		ui.label_Speed_2->setText("99");
	}
	else
	{
		int value = ui.label_Speed_2->text().toInt();
		ui.label_Speed_2->setText(QString::number(value + 1));
	}
}

void AutoTreatShoulderUnblock::On_pushButton_BackToHome_Clicked()
{

}
void AutoTreatShoulderUnblock::On_pushButton_LastStep_Clicked()
{

}
void AutoTreatShoulderUnblock::On_pushButton_Confirm_Clicked()
{
	m_iSpeed = ui.label_Speed_2->text().toInt();
	QString time = ui.label_Time->text();
	m_iTime  = time.mid(4, 1).toInt();
}
void AutoTreatShoulderUnblock::On_pushButton_NextStep_Clicked()
{
	m_pAutoTreat->SetWidgetTreatShoulderUnblockBegin();
}
