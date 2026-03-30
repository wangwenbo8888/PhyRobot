#include "AutoTreatLegUnblockBegin.h"

#include "MyWindow.h"

AutoTreatLegUnblockBegin::AutoTreatLegUnblockBegin(MyWindow* window,QWidget *parent)
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

	m_pProgressRightLeg1.reset(new RoundProgressBar(this));
	m_pProgressRightLeg1->move(790, 360);
	m_pProgressRightLeg1->setOutterBarWidth(10);
	m_pProgressRightLeg1->setInnerBarWidth(16);
	m_pProgressRightLeg1->setText(QStringLiteral("右腿1"));
	m_pProgressRightLeg1->setControlFlags(RoundProgressBar::all);

	m_pProgressLeftLeg1.reset(new RoundProgressBar(this));
	m_pProgressLeftLeg1->move(1070, 360);
	m_pProgressLeftLeg1->setOutterBarWidth(10);
	m_pProgressLeftLeg1->setInnerBarWidth(16);
	m_pProgressLeftLeg1->setText(QStringLiteral("左腿1"));
	m_pProgressLeftLeg1->setControlFlags(RoundProgressBar::all);

	m_pProgressRightLeg2.reset(new RoundProgressBar(this));
	m_pProgressRightLeg2->move(1310, 360);
	m_pProgressRightLeg2->setOutterBarWidth(10);
	m_pProgressRightLeg2->setInnerBarWidth(16);
	m_pProgressRightLeg2->setText(QStringLiteral("右腿2"));
	m_pProgressRightLeg2->setControlFlags(RoundProgressBar::all);

	m_pProgressLeftLeg2.reset(new RoundProgressBar(this));
	m_pProgressLeftLeg2->move(1560, 360);
	m_pProgressLeftLeg2->setOutterBarWidth(10);
	m_pProgressLeftLeg2->setInnerBarWidth(16);
	m_pProgressLeftLeg2->setText(QStringLiteral("左腿2"));
	m_pProgressLeftLeg2->setControlFlags(RoundProgressBar::all);

	ui.label_RightLeg1_Flag->hide();
	ui.label_LeftLeg1_Flag->hide();
	ui.label_RightLeg2_Flag->hide();
	ui.label_LeftLeg2_Flag->hide();
}

void AutoTreatLegUnblockBegin::SetTreatTime(int value)
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

void AutoTreatLegUnblockBegin::SetLegUnblock(AutoTreatLegUnblock* leg)
{
	m_pLegUnblock = leg;
	m_iTotalTime = m_pLegUnblock->m_iTotalTreatTime;
	SetTreatTime(m_iTotalTime);
}

AutoTreatLegUnblockBegin::~AutoTreatLegUnblockBegin()
{
}

void AutoTreatLegUnblockBegin::On_pushButton_Back_Clicked()
{

}

void AutoTreatLegUnblockBegin::On_pushButton_LastStep_Clicked()
{

}

void AutoTreatLegUnblockBegin::On_pushButton_PauseOrContinue_Clicked()
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

void AutoTreatLegUnblockBegin::On_pushButton_StartOrStop_Clicked()
{
	if (m_bFirstStart)
	{
		std::vector<std::vector<XUEWEI_INFO>> vXuewei;
		std::vector<XUEWEI_INFO> temp;
		XUEWEI_INFO info;

		// 右腿1
		info.intensity = m_pLegUnblock->m_iShangliao1Intensity;
		QString str1(QStringLiteral("治疗强度："));
		str1.append(QString::number(info.intensity));
		ui.label_10->setText(str1);

		info.time = m_pLegUnblock->m_iShangliao1Time;
		QString str(QStringLiteral("治疗时间："));
		str.append(QString::number(info.time));
		str.append(QStringLiteral("分钟"));
		ui.label_9->setText(str);

		m_iRightLeg1Time = info.time;
		info.xuewei = SHANGLIAO_DETECTED;
		temp.push_back(info);
		vXuewei.push_back(temp);
		temp.clear();
		SetLabelFromValue(m_iRightLeg1Time,ui.label_RightLeg1_RemainTime);

		// 左腿1
		info.intensity = m_pLegUnblock->m_iZuoyaoyan1Intensity;

		QString str2(QStringLiteral("治疗强度："));
		str2.append(QString::number(info.intensity));
		ui.label_14->setText(str2);

		info.time = m_pLegUnblock->m_iZuoyaoyan1Time;
		QString str3(QStringLiteral("治疗时间："));
		str3.append(QString::number(info.time));
		str3.append(QStringLiteral("分钟"));
		ui.label_11->setText(str3);

		m_iLeftLeg1Time = info.time;
		info.xuewei = YOUYAOYAN_DETECTED;
		temp.push_back(info);
		vXuewei.push_back(temp);
		temp.clear();
		SetLabelFromValue(m_iLeftLeg1Time, ui.label_LeftLeg1_RemainTime);

		// 右腿2
		info.intensity = m_pLegUnblock->m_iShenyu3Intensity;
		QString str5(QStringLiteral("治疗强度："));
		str5.append(QString::number(info.intensity));
		ui.label_15->setText(str5);

		info.time = m_pLegUnblock->m_iShenyu3Time;

		QString str4(QStringLiteral("治疗时间："));
		str4.append(QString::number(info.time));
		str4.append(QStringLiteral("分钟"));
		ui.label_16->setText(str4);

		m_iRightLeg2Time = info.time;
		info.xuewei = SHENYU_DETECTED;
		temp.push_back(info);
		vXuewei.push_back(temp);
		temp.clear();
		SetLabelFromValue(m_iRightLeg2Time, ui.label_RightLeg2_RemainTime);

		// 左腿2
		info.intensity = m_pLegUnblock->m_iYaoyangguan4Intensity;
		QString str8(QStringLiteral("治疗强度："));
		str8.append(QString::number(info.intensity));
		ui.label_12->setText(str8);

		info.time = m_pLegUnblock->m_iYaoyangguan4Time;

		QString str7(QStringLiteral("治疗时间："));
		str7.append(QString::number(info.time));
		str7.append(QStringLiteral("分钟"));
		ui.label_13->setText(str7);

		m_iLeftLeg2Time = info.time;
		info.xuewei = YAOYANGGUAN_DETECTED;
		temp.push_back(info);
		vXuewei.push_back(temp);
		temp.clear();
		SetLabelFromValue(m_iLeftLeg2Time, ui.label_LeftLeg2_RemainTime);

		m_pWindow->SetCurrentProj(QStringLiteral("腿部经络疏通"));
		if (m_pWindow->SetXuewei(vXuewei))
		{
			ui.label_RightLeg1_Flag->show();
			m_pWindow->Run(LEGUNBLOCK_RIGHTLEG1);
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

void AutoTreatLegUnblockBegin::On_pushButton_Recognize_Clicked()
{
	cv::Mat img;
	m_pWindow->getImage(img/*points,colorRawMat*/);
	//cv::Mat imgR = img.t();
	//cv::rotate(img, imgR, cv::ROTATE_90_COUNTERCLOCKWISE);
	cv::Mat image_part = img(cv::Rect(430, 140, 506, 496));
	ui.label_Image->setPixmap(QPixmap(m_pWindow->cvMatToQPixmap(image_part)));

	std::vector<RobotPoint>& points = m_pWindow->GetPlanPoints();
}

void AutoTreatLegUnblockBegin::On_pushButton_Drag_Clicked()
{

}

void AutoTreatLegUnblockBegin::SetLabelFromValue(int value, QLabel* label)
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
void AutoTreatLegUnblockBegin::On_FinishOneXuewei(int minute)
{
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

void AutoTreatLegUnblockBegin::On_FinishOneGroup()
{
	if (m_pProgressRightLeg1->getValue() > 99.0)
	{
		if (m_pProgressLeftLeg1->getValue() > 99.0)
		{
			if (m_pProgressRightLeg2->getValue()>99.0)
			{
				m_pProgressLeftLeg2->setValue(100.0);
				ui.label_LeftLeg2_Flag->setPixmap(QPixmap(":/WhiteTik.png"));
				ui.label_LeftLeg2_RemainTime->setText("00:00");
			}
			else
			{
				m_pProgressRightLeg2->setValue(100.0);
				ui.label_RightLeg2_Flag->setPixmap(QPixmap(":/WhiteTik.png"));
				ui.label_RightLeg2_RemainTime->setText("00:00");
				ui.label_LeftLeg2_Flag->show();
			}
		}
		else
		{
			m_pProgressLeftLeg1->setValue(100.0);
			ui.label_LeftLeg1_Flag ->setPixmap(QPixmap(":/WhiteTik.png"));
			ui.label_LeftLeg1_RemainTime->setText("00:00");
			ui.label_RightLeg2_Flag->show();
		}
	}
	else
	{
		m_pProgressRightLeg1->setValue(100);
		ui.label_RightLeg1_Flag->setPixmap(QPixmap(":/WhiteTik.png"));
		ui.label_RightLeg1_RemainTime->setText("00:00");
		ui.label_LeftLeg1_Flag->show();
	}


}