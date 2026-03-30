#include "AutoTreatShoulderUnblockBegin.h"

#include "MyWindow.h"

AutoTreatShoulderUnblockBegin::AutoTreatShoulderUnblockBegin(MyWindow* window,QWidget *parent)
	: m_pWindow(window)
	, QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	m_bFirstStart = true;

	m_pRightShoulder.reset(new RoundProgressBar(this));
	m_pRightShoulder->move(890, 360);
	m_pRightShoulder->setOutterBarWidth(10);
	m_pRightShoulder->setInnerBarWidth(16);
	m_pRightShoulder->setText(QStringLiteral("右肩膀"));
	m_pRightShoulder->setControlFlags(RoundProgressBar::all);

	m_pLeftShoulder.reset(new RoundProgressBar(this));
	m_pLeftShoulder->move(1350, 360);
	m_pLeftShoulder->setOutterBarWidth(10);
	m_pLeftShoulder->setInnerBarWidth(16);
	m_pLeftShoulder->setText(QStringLiteral("左肩膀"));
	m_pLeftShoulder->setControlFlags(RoundProgressBar::all);
	
	m_pPauseWidget.reset(new PauseWidget(this));
	m_pPauseWidget->hide();

	m_pStopWidget.reset(new AutoTreatStop());
	m_pStopWidget->SetWindow(window);
	m_pStopWidget->hide();

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

	disconnect(m_pWindow, SIGNAL(FinishOneXuewei(int)), this, SLOT(On_FinishOneXuewei(int)));
	connect(m_pWindow, SIGNAL(FinishOneXuewei(int)), this, SLOT(On_FinishOneXuewei(int)));

	disconnect(m_pWindow, SIGNAL(FinishOneGroup()), this, SLOT(On_FinishOneGroup()));
	connect(m_pWindow, SIGNAL(FinishOneGroup()), this, SLOT(On_FinishOneGroup()));

	ui.label_RightShoulderFlag->hide();
	ui.label_LeftShoulderFlag->hide();
}

AutoTreatShoulderUnblockBegin::~AutoTreatShoulderUnblockBegin()
{
}

void AutoTreatShoulderUnblockBegin::SetShoulderUnblock(AutoTreatShoulderUnblock* shoulder)
{
	m_pShoulder = shoulder;
	m_iTotalTime = m_pShoulder->m_iTotalTime;

	QString str("00:");
	if (m_iTotalTime>=10)
	{
		str.append(QString::number(m_iTotalTime));
	}
	else
	{
		str.append("0");
		str.append(QString::number(m_iTotalTime));
	}
	str.append(":00");
	ui.label_TimeValue->setText(str);
}

void AutoTreatShoulderUnblockBegin::On_pushButton_Back_Clicked()
{
}

void AutoTreatShoulderUnblockBegin::On_pushButton_LastStep_Clicked()
{
}

void AutoTreatShoulderUnblockBegin::On_pushButton_PauseOrContinue_Clicked()
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

void AutoTreatShoulderUnblockBegin::SetLabelFromValue(int value, QLabel* label)
{
	QString str;
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
	label->setText(str);
}

void AutoTreatShoulderUnblockBegin::On_pushButton_StartOrStop_Clicked()
{
	if (m_bFirstStart)
	{
		std::vector<std::vector<XUEWEI_INFO>> vXuewei;
		std::vector<XUEWEI_INFO> temp;
		XUEWEI_INFO info;

		info.intensity = m_pShoulder->m_iFengfu1Intensity;

		QString str0(QStringLiteral("治疗强度："));
		str0.append(QString::number(info.intensity));
		ui.label_Fengfu1_Intensity->setText(str0);

		info.time = m_pShoulder->m_iFengfu1Time;

		QString str1(QStringLiteral("治疗时间："));
		str1.append(QString::number(info.time));
		str1.append(QStringLiteral("分钟"));
		ui.label_Fengfu1_RemainTime->setText(str1);

		m_iTotalRightShoulder = info.time;
		info.xuewei = FENGFU_DETECTED;
		temp.push_back(info);

		info.intensity = m_pShoulder->m_iFengchi1Intensity;

		QString str2(QStringLiteral("治疗强度："));
		str2.append(QString::number(info.intensity));
		ui.label_Fengchi1_Intensity->setText(str2);
		info.time = m_pShoulder->m_iFengchi1Time;

		QString str3(QStringLiteral("治疗时间："));
		str3.append(QString::number(info.time));
		str3.append(QStringLiteral("分钟"));
		ui.label_Fengchi1_RemainTime->setText(str3);

		m_iTotalRightShoulder += info.time;
		info.xuewei = FENGCHI_DETECTED;
		temp.push_back(info);
		vXuewei.push_back(temp);
		temp.clear();
		SetLabelFromValue(m_iTotalRightShoulder,ui.label_RightShoulder_RemainTime);

		info.intensity = m_pShoulder->m_iFengfu2Intensity;

		QString str4(QStringLiteral("治疗强度："));
		str4.append(QString::number(info.intensity));
		ui.label_Fengfu2_Intensity->setText(str4);

		info.time = m_pShoulder->m_iFengfu2Time;
		QString str5(QStringLiteral("治疗时间："));
		str5.append(QString::number(info.time));
		str5.append(QStringLiteral("分钟"));
		ui.label_Fengfu2_RemainTime->setText(str5);

		m_iTotalLeftShoulder = info.time;
		info.xuewei = FENGFU_DETECTED;
		temp.push_back(info);

		info.intensity = m_pShoulder->m_iFengchi2Intensity;
		QString str6(QStringLiteral("治疗强度："));
		str6.append(QString::number(info.intensity));
		ui.label_Fengchi2_Intensity->setText(str6);

		info.time = m_pShoulder->m_iFengchi2Time;
		QString str7(QStringLiteral("治疗时间："));
		str7.append(QString::number(info.time));
		str7.append(QStringLiteral("分钟"));
		ui.label_Fengchi2_RemainTime->setText(str7);

		m_iTotalLeftShoulder += info.time;
		info.xuewei = FENGCHI_DETECTED;
		temp.push_back(info);
		vXuewei.push_back(temp);
		temp.clear();
		SetLabelFromValue(m_iTotalLeftShoulder, ui.label_LeftShoulder_RemainTime);

		m_pWindow->SetCurrentProj(QStringLiteral("肩部经络疏通"));
		if (m_pWindow->SetXuewei(vXuewei))
		{
			ui.label_RightShoulderFlag->show();
			m_pWindow->Run(SHOULDERUNBLOCK);
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

void AutoTreatShoulderUnblockBegin::On_pushButton_Recognize_Clicked()
{
	cv::Mat img;
	m_pWindow->getImage(img/*points,colorRawMat*/);
	//cv::Mat imgR = img.t();
	//cv::rotate(img, imgR, cv::ROTATE_90_COUNTERCLOCKWISE);
	cv::Mat image_part = img(cv::Rect(430, 140, 506, 496));
	ui.label_Image->setPixmap(QPixmap(m_pWindow->cvMatToQPixmap(image_part)));

	std::vector<RobotPoint>& points = m_pWindow->GetPlanPoints();
}

void AutoTreatShoulderUnblockBegin::On_pushButton_Drag_Clicked()
{

}

void AutoTreatShoulderUnblockBegin::On_FinishOneXuewei(int minute)
{
	if (m_pRightShoulder->getValue() > 99.0)
	{
		if (m_pLeftShoulder->getValue() > 99.0)
		{
			
		}
		else
		{
			float value = m_pLeftShoulder->getValue();
			m_pLeftShoulder->setValue(value + 50.0);
			m_iTotalLeftShoulder -= minute;
			SetLabelFromValue(m_iTotalLeftShoulder, ui.label_LeftShoulder_RemainTime);
		}
	}
	else
	{
		float value = m_pRightShoulder->getValue();
		m_pRightShoulder->setValue(value + 50.0);
		m_iTotalRightShoulder -= minute;
		SetLabelFromValue(m_iTotalRightShoulder, ui.label_RightShoulder_RemainTime);
	}

	m_iTotalTime -= minute;
	QString strTime("00:");
	if (m_iTotalTime < 10)
	{
		strTime.append("0");
		strTime.append(QString::number(m_iTotalTime));
	}
	else
	{
		strTime.append(QString::number(m_iTotalTime));
	}
	strTime.append(":00");
	ui.label_TimeValue->setText(strTime);
}

void AutoTreatShoulderUnblockBegin::On_FinishOneGroup()
{
	if (m_pRightShoulder->getValue() > 99.0)
	{
		if (m_pLeftShoulder->getValue() > 99.0)
		{

		}
		else
		{
			m_pLeftShoulder->setValue(100.0);
			ui.label_LeftShoulderFlag->setPixmap(QPixmap(":/WhiteTik.png"));
			ui.label_LeftShoulder_RemainTime->setText("00:00");
		}
	}
	else
	{
		m_pRightShoulder->setValue(100);
		ui.label_RightShoulderFlag->setPixmap(QPixmap(":/WhiteTik.png"));
		ui.label_RightShoulder_RemainTime->setText("00:00");
		ui.label_LeftShoulderFlag->show();
	}
}