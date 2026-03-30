#include "AutoTreatLegOpenBegin.h"

#include "MyWindow.h"

AutoTreatLegOpenBegin::AutoTreatLegOpenBegin(MyWindow* window,QWidget *parent)
	: m_pWindow(window)
	, QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	m_bFirstStart = true;

	m_pPauseWidget.reset(new PauseWidget(this));
	m_pPauseWidget->hide();

	m_pStopWidget.reset(new AutoTreatStop());
	m_pStopWidget->SetWindow(window);
	m_pStopWidget->hide();

	m_pRoundProgress.reset(new RoundProgressBar(this));
	m_pRoundProgress->move(1140, 360);
	m_pRoundProgress->setOutterBarWidth(10);
	m_pRoundProgress->setInnerBarWidth(16);
	m_pRoundProgress->setText(QStringLiteral("上髎"));
	m_pRoundProgress->setControlFlags(RoundProgressBar::all);

	disconnect(ui.pushButton_Back, SIGNAL(clicked()), this, SLOT(On_pushButton_Back_Clicked()));
	connect(ui.pushButton_Back, SIGNAL(clicked()), this, SLOT(On_pushButton_Back_Clicked()));

	disconnect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(On_pushButton_LastStep_Clicked()));
	connect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(On_pushButton_LastStep_Clicked()));

	disconnect(ui.pushButton_PauseOrContinue, SIGNAL(clicked()), this, SLOT(On_pushButton_PauseOrContinue_Clicked()));
	connect(ui.pushButton_PauseOrContinue, SIGNAL(clicked()), this, SLOT(On_pushButton_PauseOrContinue_Clicked()));

	disconnect(ui.pushButton_StartOrStop, SIGNAL(clicked()), this, SLOT(On_pushButton_StartOrStop_Clicked()));
	connect(ui.pushButton_StartOrStop, SIGNAL(clicked()), this, SLOT(On_pushButton_StartOrStop_Clicked()));

	disconnect(ui.pushButton_Recognize, SIGNAL(clicked()), this, SLOT(On_pushButton_Recognize_Clicked()));
	connect(ui.pushButton_Recognize, SIGNAL(clicked()), this, SLOT(On_pushButton_Recognize_Clicked()));

	disconnect(ui.pushButton_Drag, SIGNAL(clicked()), this, SLOT(On_pushButton_Drag_Clicked()));
	connect(ui.pushButton_Drag, SIGNAL(clicked()), this, SLOT(On_pushButton_Drag_Clicked()));

	disconnect(m_pWindow, SIGNAL(FinishOneGroup()), this, SLOT(On_FinishOneGroup()));
	connect(m_pWindow, SIGNAL(FinishOneGroup()), this, SLOT(On_FinishOneGroup()));

	ui.label_flag->hide();
}

AutoTreatLegOpenBegin::~AutoTreatLegOpenBegin()
{

}

void AutoTreatLegOpenBegin::SetLegOpen(AutoTreatLegOpen* legopen)
{
	m_pLegOpen = legopen;
	SetTreatTime(m_pLegOpen->m_iLegOpenTime);
}

void AutoTreatLegOpenBegin::On_pushButton_Back_Clicked()
{

}

void AutoTreatLegOpenBegin::On_pushButton_LastStep_Clicked()
{

}

void AutoTreatLegOpenBegin::On_pushButton_PauseOrContinue_Clicked()
{
	if (m_pPauseWidget->isVisible())
	{
		m_pWindow->WorkContinue();
		m_pPauseWidget->hide();
	}
	else
	{
		m_pWindow->pause();

		m_pPauseWidget->move(720, 320);
		m_pPauseWidget->setWindowFlags(m_pPauseWidget->windowFlags() | Qt::Dialog);
		//m_pPauseWidget->setWindowModality(Qt::ApplicationModal);
		m_pPauseWidget->show();
	}
}

void AutoTreatLegOpenBegin::On_pushButton_StartOrStop_Clicked()
{
	if (m_bFirstStart)
	{
		std::vector<std::vector<XUEWEI_INFO>> vXuewei;
		std::vector<XUEWEI_INFO> temp;
		XUEWEI_INFO info;

		info.intensity = m_pLegOpen->m_iLegOpenIntensity;
		info.time = m_pLegOpen->m_iLegOpenTime;
		info.xuewei = SHANGLIAO_DETECTED;
		temp.push_back(info);
		vXuewei.push_back(temp);

		temp.clear();

		m_pWindow->SetCurrentProj(QStringLiteral("腿部经络初开"));
		if (m_pWindow->SetXuewei(vXuewei))
		{
			ui.label_flag->show();
			m_pWindow->Run(OPENLEG);
		}

		m_bFirstStart = false;
	}
	else
	{
		m_pStopWidget->move(920, 440);
		m_pStopWidget->setWindowFlags(m_pStopWidget->windowFlags() | Qt::Dialog);
		m_pStopWidget->setWindowModality(Qt::ApplicationModal);
		m_pStopWidget->show();
	}

}

void AutoTreatLegOpenBegin::On_pushButton_Recognize_Clicked()
{
	cv::Mat img;
	m_pWindow->getImage(img/*points,colorRawMat*/);
	//cv::Mat imgR = img.t();
	//cv::rotate(img, imgR, cv::ROTATE_90_COUNTERCLOCKWISE);
	cv::Mat image_part = img(cv::Rect(430, 140, 506, 496));
	ui.label_Image->setPixmap(QPixmap(m_pWindow->cvMatToQPixmap(image_part)));

	std::vector<RobotPoint>& points = m_pWindow->GetPlanPoints();
}

void AutoTreatLegOpenBegin::SetTreatTime(int value)
{
	QString str("00:");
	if (value >= 10)
	{
		str = QString::number(value);
	}
	else
	{
		str = "0";
		str.append(QString::number(value));
	}
	str.append(":00");
	ui.label_TimeValue->setText(str);
}

void AutoTreatLegOpenBegin::On_FinishOneGroup()
{
	m_pRoundProgress->setValue(100.0);
	ui.label_flag->setPixmap(QPixmap(":/WhiteTik.png"));
	ui.label_TimeValue->setText("00:00:00");
}

void AutoTreatLegOpenBegin::On_pushButton_Drag_Clicked()
{

}
