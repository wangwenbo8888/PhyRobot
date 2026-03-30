#include "AutoTreatOnGoing.h"

#include "AutoTreat.h"
#include "MyWindow.h"

AutoTreatOnGoing::AutoTreatOnGoing(AutoTreat* treat, QWidget *parent)
	: m_pAutoTreat(treat)
	, QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_Back, SIGNAL(clicked()), this, SLOT(On_pushButton_Back_Clicked()));
	connect(ui.pushButton_Back, SIGNAL(clicked()), this, SLOT(On_pushButton_Back_Clicked()));

	disconnect(ui.pushButton_WorkContinue, SIGNAL(clicked()), this, SLOT(On_pushButton_WorkContinue_Clicked()));
	connect(ui.pushButton_WorkContinue, SIGNAL(clicked()), this, SLOT(On_pushButton_WorkContinue_Clicked()));

	disconnect(ui.pushButton_WorkPause, SIGNAL(clicked()), this, SLOT(On_pushButton_Pause_Clicked()));
	connect(ui.pushButton_WorkPause, SIGNAL(clicked()), this, SLOT(On_pushButton_Pause_Clicked()));

	disconnect(ui.pushButton_WorkStop, SIGNAL(clicked()), this, SLOT(On_pushButton_Stop_Clicked()));
	connect(ui.pushButton_WorkStop, SIGNAL(clicked()), this, SLOT(On_pushButton_Stop_Clicked()));

	m_pTimer = new QTimer(this);

	m_pTimer->setInterval(1000);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(On_TimeOut()));
	//m_pTimer->start();

}

AutoTreatOnGoing::~AutoTreatOnGoing()
{
	if (m_pTimer!=NULL)
	{
		delete m_pTimer;
		m_pTimer = NULL;
	}
}

void AutoTreatOnGoing::SetPlanImage()
{
	cv::Mat img = m_pAutoTreat->GetWindow()->getPlanImage();
	m_iWide = img.rows;
	m_iHight = img.cols;

	if (img.empty())
	{
		return;
	}
	cv::Mat imgR = img.t();
	cv::rotate(img, imgR, cv::ROTATE_90_COUNTERCLOCKWISE);
	cv::Mat image_part = imgR(cv::Rect(200, 140, 920, 460));
	ui.label_Image->setPixmap(QPixmap(m_pAutoTreat->GetWindow()->cvMatToQPixmap(image_part)));
}

void AutoTreatOnGoing::SetLabelTreated(int i)
{
	if (i>= m_vAcupoints.size())
	{
		return;
	}

	QLabel* label = m_vAcupoints[i].get();
	label->setPixmap(QPixmap(":/treated.png"));
	label->show();
}

void AutoTreatOnGoing::SetLabelTreating(int i)
{
	if (i >= m_vAcupoints.size())
	{
		return;
	}

	QLabel* label = m_vAcupoints[i].get();
	label->setPixmap(QPixmap(":/treating.png"));
	label->show();
}

void AutoTreatOnGoing::SetAcupointLabels(const std::vector<RobotPoint>& points)
{
	m_vAcupoints.clear();
	for (int i = 0; i < points.size();++i)
	{
		const cv::Point& p = points[i].p2d;
		QSharedPointer<QLabel> label;
		label.reset(new QLabel(ui.label_Image));
		label->move(p.y-200-30, 460+140 - p.x-28);
		label->setPixmap(QPixmap(":/untreat.png"));
		label->show();
		m_vAcupoints.push_back(label);
	}
}

void AutoTreatOnGoing::On_pushButton_Back_Clicked()
{
	//m_pAutoTreat->SetWidgetTreatBegin();
}

void AutoTreatOnGoing::On_pushButton_WorkContinue_Clicked()
{
	TimerContinue();
	//m_pAutoTreat->SetWidgetTreatFinish();
    m_pAutoTreat->GetWindow()->WorkContinue();
}

void AutoTreatOnGoing::On_pushButton_Pause_Clicked()
{
	TimerStop();
    m_pAutoTreat->GetWindow()->pause();
}

void AutoTreatOnGoing::On_pushButton_Stop_Clicked()
{
	TimerStop();
    m_pAutoTreat->GetWindow()->stop();
}

void AutoTreatOnGoing::On_TimeOut()
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

void AutoTreatOnGoing::TimerStart()
{
	if (m_pTimer!=NULL)
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
		if (second==0)
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

void AutoTreatOnGoing::TimerContinue()
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

void AutoTreatOnGoing::TimerStop()
{
	if (m_pTimer != NULL)
	{
		m_pTimer->stop();
	}
}
