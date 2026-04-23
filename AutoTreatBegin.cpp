#include "AutoTreatBegin.h"

#include "AutoTreat.h"
#include "MyWindow.h"

#include "qmessagebox.h"

#include "AutoTreatOpenBack.h"

AutoTreatBegin::AutoTreatBegin(MyWindow* window, QWidget *parent)
	: m_pWindow(window)
	, QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	m_bStarted = false;

	m_pPauseWidget.reset(new PauseWidget(this));
	m_pPauseWidget->hide();

	m_pStopWidget.reset(new AutoTreatStop(this));
	m_pStopWidget->SetWindow(m_pWindow);
	m_pStopWidget->hide();

	disconnect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_LastStep_Clicked()));
	connect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_LastStep_Clicked()));

	disconnect(ui.pushButton_StartOrStop, SIGNAL(clicked()), this, SLOT(on_Pushbutton_StartOrStop_Clicked()));
	connect(ui.pushButton_StartOrStop, SIGNAL(clicked()), this, SLOT(on_Pushbutton_StartOrStop_Clicked()));

	disconnect(ui.pushButton_Back, SIGNAL(clicked()), this, SLOT(on_pushButton_Back_Clicked()));
	connect(ui.pushButton_Back, SIGNAL(clicked()), this, SLOT(on_pushButton_Back_Clicked()));

	disconnect(ui.pushButton_PauseOrContinue, SIGNAL(clicked()), this, SLOT(on_pushButton_PauseOrContinue_clicked()));
	connect(ui.pushButton_PauseOrContinue, SIGNAL(clicked()), this, SLOT(on_pushButton_PauseOrContinue_clicked()));

	disconnect(ui.pushButton_Recognize, SIGNAL(clicked()), this, SLOT(On_pushButton_Recognize_Clicked()));
	connect(ui.pushButton_Recognize, SIGNAL(clicked()), this, SLOT(On_pushButton_Recognize_Clicked()));

	disconnect(ui.pushButton_Drag, SIGNAL(clicked()), this, SLOT(On_pushButton_Drag_Clicked()));
	connect(ui.pushButton_Drag, SIGNAL(clicked()), this, SLOT(On_pushButton_Drag_Clicked()));

	disconnect(ui.pushButton_IntensityIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityIncr_Clicked()));
	connect(ui.pushButton_IntensityIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityIncr_Clicked()));

	disconnect(ui.pushButton_IntensityDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityDecr_Clicked()));
	connect(ui.pushButton_IntensityDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityDecr_Clicked()));

	disconnect(m_pWindow, SIGNAL(FinishOneXuewei(int)), this, SLOT(On_FinishOneXuewei(int)));
	connect(m_pWindow, SIGNAL(FinishOneXuewei(int)), this, SLOT(On_FinishOneXuewei(int)));

	disconnect(m_pWindow, SIGNAL(FinishOneSecond()), this, SLOT(On_FinishOneSecond()));
	connect(m_pWindow, SIGNAL(FinishOneSecond()), this, SLOT(On_FinishOneSecond()));

	disconnect(m_pWindow, SIGNAL(FinishOneGroup()), this, SLOT(On_FinishOneGroup()));
	connect(m_pWindow, SIGNAL(FinishOneGroup()), this, SLOT(On_FinishOneGroup()));

	barDumai.reset(new RoundProgressBar(this));
	barDumai->move(720,360);
	barDumai->setOutterBarWidth(10);
	barDumai->setInnerBarWidth(16);
	barDumai->setText(QStringLiteral("督脉"));
	barDumai->setControlFlags(RoundProgressBar::all);

	barZuopangguangjing.reset(new RoundProgressBar(this));
	barZuopangguangjing->move(990, 360);
	barZuopangguangjing->setOutterBarWidth(10);
	barZuopangguangjing->setInnerBarWidth(16);
	barZuopangguangjing->setText(QStringLiteral("左膀胱经"));
	barZuopangguangjing->setControlFlags(RoundProgressBar::all);

	barYoupangguangjing.reset(new RoundProgressBar(this));
	barYoupangguangjing->move(1250, 360);
	barYoupangguangjing->setOutterBarWidth(10);
	barYoupangguangjing->setInnerBarWidth(16);
	barYoupangguangjing->setText(QStringLiteral("右膀胱经"));
	barYoupangguangjing->setControlFlags(RoundProgressBar::all);

	ui.label_flag1->hide();
	ui.label_flag2->hide();
	ui.label_flag3->hide();
	ui.stackedWidget_Widgets->setCurrentIndex(0);
}

AutoTreatBegin::~AutoTreatBegin()
{

}

void AutoTreatBegin::on_pushButton_Back_Clicked()
{
	m_pWindow->GetAutoTreat()->FinishReturn();
}

void AutoTreatBegin::on_pushButton_PauseOrContinue_clicked()
{
	if (m_pPauseWidget->isVisible())
	{
		m_pWindow->WorkContinue();
		m_pPauseWidget->hide();
	}
	else
	{
		m_pWindow->pause();

		m_pPauseWidget->move(660,20);
		//m_pPauseWidget->setWindowFlags(m_pPauseWidget->windowFlags() | Qt::Dialog);
		//m_pPauseWidget->setWindowFlags(Qt::WindowStaysOnTopHint);
		//m_pPauseWidget->setWindowModality(Qt::NonModal);
		m_pPauseWidget->show();

	}
}

void AutoTreatBegin::on_Pushbutton_LastStep_Clicked()
{
	//m_pAutoTreat->SetWidgetToleranceTest();
}

void AutoTreatBegin::SetXuewei(XUEWEI_TYPE type)
{
	m_eType = type;
	if (m_eType ==FENGFU)
	{
		ui.label_2->setPixmap(QPixmap(":/FengfuLabel.png"));
	}
	else if (m_eType ==DAZHUI1)
	{
		ui.label_2->setPixmap(QPixmap(":/DazhuiLabel.png"));
	}
}

void AutoTreatBegin::SetCommunicate(Communicate* comm)
{
	m_pCommunicate = comm;
}

void AutoTreatBegin::SetStepDistanceLabel()
{
	ui.label_Speed->setPixmap(QPixmap(":/StepDistance.png"));
}

void AutoTreatBegin::SetStarted(bool b)
{
	m_bStarted = b;
}

void AutoTreatBegin::SetOpenBack(AutoTreatOpenBack* openback)
{
	m_pOpenBack = openback;
}

void AutoTreatBegin::SetSpeed(int speed)
{
	ui.label_SpeedValue->setText(QString::number(speed));
	m_iSpeed = speed;
}

void AutoTreatBegin::SetTime(int time)
{
	m_iTotalTime = time;
	m_iTotalSecond = m_iTotalTime * 60;

	QString strTime("00:");
	if (m_iTotalTime<10)
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

void AutoTreatBegin::on_Pushbutton_StartOrStop_Clicked()
{
#if 0
    m_pAutoTreat->GetWindow()->poweron();

    m_pAutoTreat->SetWidgetTreatOnGoing();
	m_pAutoTreat->GetWidgetTreatOnGoing()->SetAcupointLabels(m_pAutoTreat->GetWindow()->GetPlanPoints());
    m_pAutoTreat->GetWindow()->go();
#endif
	if (!m_bStarted)
	{
		m_bStarted = true;

		std::vector<std::vector<XUEWEI_INFO>> vXuewei;
		std::vector<XUEWEI_INFO> temp;
		XUEWEI_INFO info;

		m_iDumaiTime = 0;
		if (m_eType==FENGFU)
		{
			info.xuewei = FENGFU_DETECTED;
			info.time = m_pOpenBack->m_iFengfuTime;
			m_iDumaiTime += info.time;

			info.intensity = m_pOpenBack->m_iFengfuIntensity;
			temp.push_back(info);
		}
		else if (m_eType==DAZHUI1)
		{
			info.time = m_pOpenBack->m_iDazhui1Time;
			info.intensity = m_pOpenBack->m_iDazhui1Intensity;
			m_iDumaiTime += info.time;
			info.xuewei = DAZHUI_DETECTED;
			temp.push_back(info);
		}

		// 途径穴位，不停留
		info.intensity = m_pOpenBack->m_iDazhui2Intensity;
		info.time = 0;
		m_iDumaiTime += info.time;
		info.xuewei = DAZHUI_DETECTED;
		temp.push_back(info);

		// 途径穴位，不停留
		info.intensity = m_pOpenBack->m_iShendaoIntensity;
		info.time = 0;
		m_iDumaiTime += info.time;
		info.xuewei = SHENDAO_DETECTED;
		temp.push_back(info);

		// 途径穴位，不停留
		info.intensity = m_pOpenBack->m_iZhiyangIntensity;
		info.time = 0;
		m_iDumaiTime += info.time;
		info.xuewei = ZHIYANG_DETECTED;
		temp.push_back(info);

		info.intensity = m_pOpenBack->m_iYaoyangguanIntensity;
		info.time = m_pOpenBack->m_iYaoyangguanTime;
		m_iDumaiTime += info.time;

		info.xuewei = YAOYANGGUAN_DETECTED;
		temp.push_back(info);
		vXuewei.push_back(temp);
		temp.clear();

		SetLabelFromValue(m_iDumaiTime, ui.label_Dumai_RemainTime);

		m_iZuopangguangjingTime = 0;
		info.intensity = m_pOpenBack->m_iZuojianjingIntensity;
		info.time = m_pOpenBack->m_iZuojianjingTime;
		m_iZuopangguangjingTime += info.time;

		info.xuewei = ZUOJIANJING_DETECTED;
		temp.push_back(info);

		info.intensity = m_pOpenBack->m_iZuoqihaiyuIntensity;
		info.time = m_pOpenBack->m_iZuoqihaiyuTime;
		m_iZuopangguangjingTime += info.time;

		info.xuewei = ZUOQIHAIYU_DETECTED;
		temp.push_back(info);
		vXuewei.push_back(temp);
		temp.clear();

		SetLabelFromValue(m_iZuopangguangjingTime, ui.label_Zuopangguangjing_RemainTime);

		m_iYoupangguangjingTime = 0;
		info.intensity = m_pOpenBack->m_iYoujianjingIntensity;
		info.time = m_pOpenBack->m_iYoujianjingTime;
		m_iYoupangguangjingTime += info.time;

		info.xuewei = YOUJIANJING_DETECTED;
		temp.push_back(info);

		info.intensity = m_pOpenBack->m_iYouqihaiyuIntensity;
		info.time = m_pOpenBack->m_iYouqihaiyuTime;
		m_iYoupangguangjingTime += info.time;

		info.xuewei = YOUQIHAIYU_DETECTED;
		temp.push_back(info);
		vXuewei.push_back(temp);
		temp.clear();

		SetLabelFromValue(m_iYoupangguangjingTime,ui.label_Youpangguangjing_RemainTime);
		m_pWindow->SetCurrentProj(QStringLiteral("开背"));

		if (m_pWindow->SetXuewei(vXuewei))
		{
			ui.label_flag1->show();
			m_pWindow->SetModel(m_pOpenBack->GetModel());
			m_pWindow->Run(OPENBACK,m_iSpeed);
		}

		// 开始治疗
		return;
	}
	else
	{
		m_pStopWidget->move(920, 440);
		m_pStopWidget->setWindowFlags(m_pStopWidget->windowFlags() | Qt::Dialog);
		m_pStopWidget->setWindowModality(Qt::ApplicationModal);
		m_pStopWidget->show();
	}
}

void AutoTreatBegin::On_pushButton_Recognize_Clicked()
{
	//ui.pushButton_Recognize->setStyleSheet("background-color:blue;color:#059FC9;border-radius:29px;border:1px solid #B6B6B7;");
	cv::Mat img;
	if (!m_pWindow->getImage(img/*points,colorRawMat*/))
	{
		QMessageBox::information(NULL, "Info", "Find acupoint error !", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}
	//cv::Mat imgR = img.t();
	//cv::rotate(img, imgR, cv::ROTATE_90_COUNTERCLOCKWISE);
	//cv::Mat image_part = imgR(cv::Rect(430, 140, 506, 496));
	cv::Mat image_part = img(cv::Rect(430, 140, 506, 496));
	ui.label_Image->setPixmap(QPixmap(m_pWindow->cvMatToQPixmap(image_part)));

	std::vector<RobotPoint>& points = m_pWindow->GetPlanPoints();
	m_vAcupoints.clear();
	for (int i = 0; i < points.size(); ++i)
	{
		const cv::Point& p = points[i].p2d;
		QSharedPointer<QLabel> label;
		//label.reset(new QLabel(ui.label_Image));
		//label->move(p.y - 200 - 30, 460 + 140 - p.x - 56);
		//label->setPixmap(QPixmap(":/untreat.png"));
		//label->show();
		m_vAcupoints.push_back(label);
	}

	//ui.pushButton_Recognize->setStyleSheet("background-color:white;color:#059FC9;border-radius:29px;border:1px solid #B6B6B7;");
}

void AutoTreatBegin::On_pushButton_Drag_Clicked()
{
}

void AutoTreatBegin::On_pushButton_IntensityIncr_Clicked()
{
	m_pWindow->IncrIntensity();
	ui.label_IntensityValue->setText(QString::number(m_pWindow->GetIntensity()));
}

void AutoTreatBegin::On_pushButton_IntensityDecr_Clicked()
{
	m_pWindow->DecrIntensity();
	ui.label_IntensityValue->setText(QString::number(m_pWindow->GetIntensity()));
}

void AutoTreatBegin::On_FinishOneSecond()
{
	m_iTotalSecond = m_iTotalSecond-1;
	int hour = m_iTotalSecond / 3600;
	int minute = (m_iTotalSecond - 3600 * hour) / 60;
	int second = m_iTotalSecond - 3600 * hour - 60 * minute;
	QString strTime;
	if (hour<10)
	{
		strTime = "0";
		strTime.append(QString::number(hour));
	}
	else
	{
		strTime.append(QString::number(hour));
	}

	strTime.append(":");
	if (minute < 10)
	{
		strTime.append("0");
		strTime.append(QString::number(minute));
	}
	else
	{
		strTime.append(QString::number(minute));
	}

	strTime.append(":");
	if (second < 10)
	{
		strTime.append("0");
		strTime.append(QString::number(second));
	}
	else
	{
		strTime.append(QString::number(second));
	}

	ui.label_TimeValue->setText(strTime);
}

void AutoTreatBegin::On_FinishOneXuewei(int minute)
{
	if (barDumai->getValue() > 99.0)
	{
		if (barZuopangguangjing->getValue() > 99.0)
		{
			barYoupangguangjing->setValue(50.0);
			m_iYoupangguangjingTime -= minute;
			SetLabelFromValue(m_iYoupangguangjingTime, ui.label_Youpangguangjing_RemainTime);
		}
		else
		{
			float value = barZuopangguangjing->getValue();
			barZuopangguangjing->setValue(value+50.0);
			m_iZuopangguangjingTime -= minute;
			SetLabelFromValue(m_iZuopangguangjingTime, ui.label_Zuopangguangjing_RemainTime);
		}
	}
	else
	{
		float value = barDumai->getValue();
		barDumai->setValue(value+20.0);
		m_iDumaiTime -= minute;
		SetLabelFromValue(m_iDumaiTime,ui.label_Dumai_RemainTime);
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

void AutoTreatBegin::SetLabelFromValue(int value, QLabel* label)
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

void AutoTreatBegin::On_FinishOneGroup()
{
	if (barDumai->getValue()>99.0)
	{
		if (barZuopangguangjing->getValue()>99.0)
		{
			barYoupangguangjing->setValue(100.0);
			ui.label_flag3->setPixmap(QPixmap(":/WhiteTik.png"));
			ui.label_Youpangguangjing_RemainTime->setText("00:00");
			ui.stackedWidget_Widgets->setCurrentIndex(0);
		}
		else
		{
			barZuopangguangjing->setValue(100.0);
			ui.label_flag2->setPixmap(QPixmap(":/WhiteTik.png"));
			ui.label_Zuopangguangjing_RemainTime->setText("00:00");
			ui.label_flag3->show();
		}
	}
	else
	{
		barDumai->setValue(100);
		ui.label_flag1->setPixmap(QPixmap(":/WhiteTik.png"));
		ui.label_Dumai_RemainTime->setText("00:00");
		ui.label_flag2->show();
		ui.stackedWidget_Widgets->setCurrentIndex(1);
	}
}

