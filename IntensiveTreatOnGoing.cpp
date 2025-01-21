#include "IntensiveTreatOnGoing.h"

#include "IntensiveTreat.h"

IntensiveTreatOnGoing::IntensiveTreatOnGoing(IntensiveTreat* treat, QWidget *parent)
	: m_pIntensiveTreat(treat)
	, QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_Back, SIGNAL(clicked()), this, SLOT(On_pushButton_Back_Clicked()));
	connect(ui.pushButton_Back, SIGNAL(clicked()), this, SLOT(On_pushButton_Back_Clicked()));

	disconnect(ui.pushButton_WorkContinue, SIGNAL(clicked()), this, SLOT(On_pushButton_WorkContinue_Clicked()));
	connect(ui.pushButton_WorkContinue, SIGNAL(clicked()), this, SLOT(On_pushButton_WorkContinue_Clicked()));

	m_pTimer = new QTimer(this);

	m_pTimer->setInterval(1000);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(On_TimeOut()));
}

IntensiveTreatOnGoing::~IntensiveTreatOnGoing()
{
}

void IntensiveTreatOnGoing::On_pushButton_Back_Clicked()
{
	TimerStop();
	m_pIntensiveTreat->SetWidgetBegin();
}

void IntensiveTreatOnGoing::On_pushButton_WorkContinue_Clicked()
{
	TimerContinue();
	m_pIntensiveTreat->SetWidgetFinish();
}

void IntensiveTreatOnGoing::TimerContinue()
{
	if (m_pTimer != NULL)
	{
		m_pTimer->start();
		//m_iTotalTime = 3600;
		QString time("00");
		time.append(":");
		int min = m_iTotalTime / 60;
		int second = m_iTotalTime % 60;
		if (min == 0)
		{
			time.append("00");
		}
		else if (min < 10)
		{
			time.append("0");
			time.append(QString::number(min));
		}
		else
		{
			time.append(QString::number(min));
		}
		time.append(":");
		if (second == 0)
		{
			time.append("00");
		}
		else if (second < 10)
		{
			time.append("0");
			time.append(QString::number(second));
		}
		else
		{
			time.append(QString::number(second));
		}

		ui.label_10->setText(time);
	}
}

void IntensiveTreatOnGoing::On_TimeOut()
{
	--m_iTotalTime;
	QString time("00");
	time.append(":");
	int min = m_iTotalTime / 60;
	int second = m_iTotalTime % 60;
	if (min == 0)
	{
		time.append("00");
	}
	else if (min < 10)
	{
		time.append("0");
		time.append(QString::number(min));
	}
	else
	{
		time.append(QString::number(min));
	}
	time.append(":");
	if (second == 0)
	{
		time.append("00");
	}
	else if (second < 10)
	{
		time.append("0");
		time.append(QString::number(second));
	}
	else
	{
		time.append(QString::number(second));
	}

	ui.label_10->setText(time);
}

void IntensiveTreatOnGoing::TimerStart()
{
	if (m_pTimer != NULL)
	{
		m_pTimer->start();
		m_iTotalTime = 3600;
		QString time("00");
		time.append(":");
		int min = m_iTotalTime / 60;
		int second = m_iTotalTime % 60;
		if (min == 0)
		{
			time.append("00");
		}
		else if (min < 10)
		{
			time.append("0");
			time.append(QString::number(min));
		}
		else
		{
			time.append(QString::number(min));
		}
		time.append(":");
		if (second == 0)
		{
			time.append("00");
		}
		else if (second < 10)
		{
			time.append("0");
			time.append(QString::number(second));
		}
		else
		{
			time.append(QString::number(second));
		}

		ui.label_10->setText(time);
	}
}

void IntensiveTreatOnGoing::TimerStop()
{
	if (m_pTimer != NULL)
	{
		m_pTimer->stop();
	}
}
