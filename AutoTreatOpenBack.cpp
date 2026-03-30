#include "AutoTreatOpenBack.h"

#include "AutoTreat.h"

AutoTreatOpenBack::AutoTreatOpenBack(AutoTreat* treat,QWidget *parent)
	:m_pAuto(treat)
	,QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	m_pStartOrEndAdj.reset(new StartOrEndAdjust(this));
	m_pStartOrEndAdj->hide();
	m_pPathwayAdj.reset(new PathwayAdjust(this));
	m_pPathwayAdj->hide();

	m_pSpeedAdj.reset(new MoveSpeedAdjust(this));
	m_pSpeedAdj->hide();

	m_pStepModelAdj.reset(new StepModelAdjust(this));
	m_pStepModelAdj->hide();

	m_eModel = MODEL_CONTINUE;

	disconnect(ui.pushButton_BackToHome, SIGNAL(clicked()), this, SLOT(On_pushButton_BackToHome_Clicked()));
	connect(ui.pushButton_BackToHome, SIGNAL(clicked()), this, SLOT(On_pushButton_BackToHome_Clicked()));

	disconnect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(On_pushButton_LastStep_Clicked()));
	connect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(On_pushButton_LastStep_Clicked()));

	disconnect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(On_pushButton_Confirm_Clicked()));
	connect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(On_pushButton_Confirm_Clicked()));

	disconnect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(On_pushButton_NextStep_Clicked()));
	connect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(On_pushButton_NextStep_Clicked()));

	disconnect(ui.pushButton_Fengfu, SIGNAL(clicked()), this, SLOT(On_pushButton_Fengfu_Clicked()));
	connect(ui.pushButton_Fengfu, SIGNAL(clicked()), this, SLOT(On_pushButton_Fengfu_Clicked()));

	disconnect(ui.pushButton_Dazhui1, SIGNAL(clicked()), this, SLOT(On_pushButton_Dazhui1_Clicked()));
	connect(ui.pushButton_Dazhui1, SIGNAL(clicked()), this, SLOT(On_pushButton_Dazhui1_Clicked()));

	disconnect(ui.pushButton_Dazhui2, SIGNAL(clicked()), this, SLOT(On_pushButton_Dazhui2_Clicked()));
	connect(ui.pushButton_Dazhui2, SIGNAL(clicked()), this, SLOT(On_pushButton_Dazhui2_Clicked()));

	disconnect(ui.pushButton_Yaoyangguan, SIGNAL(clicked()), this, SLOT(On_pushButton_Yaoyangguan_Clicked()));
	connect(ui.pushButton_Yaoyangguan, SIGNAL(clicked()), this, SLOT(On_pushButton_Yaoyangguan_Clicked()));

	disconnect(ui.pushButton_Shendao, SIGNAL(clicked()), this, SLOT(On_pushButton_Shendao_Clicked()));
	connect(ui.pushButton_Shendao, SIGNAL(clicked()), this, SLOT(On_pushButton_Shendao_Clicked()));

	disconnect(ui.pushButton_Zhiyang, SIGNAL(clicked()), this, SLOT(On_pushButton_Zhiyang_Clicked()));
	connect(ui.pushButton_Zhiyang, SIGNAL(clicked()), this, SLOT(On_pushButton_Zhiyang_Clicked()));

	disconnect(ui.pushButton_Zuojianjing, SIGNAL(clicked()), this, SLOT(On_pushButton_Zuojianjing_Clicked()));
	connect(ui.pushButton_Zuojianjing, SIGNAL(clicked()), this, SLOT(On_pushButton_Zuojianjing_Clicked()));

	disconnect(ui.pushButton_Youjianjing, SIGNAL(clicked()), this, SLOT(On_pushButton_Youjianjing_Clicked()));
	connect(ui.pushButton_Youjianjing, SIGNAL(clicked()), this, SLOT(On_pushButton_Youjianjing_Clicked()));

	disconnect(ui.pushButton_Zuoqihaiyu, SIGNAL(clicked()), this, SLOT(On_pushButton_Zuoqihaiyu_Clicked()));
	connect(ui.pushButton_Zuoqihaiyu, SIGNAL(clicked()), this, SLOT(On_pushButton_Zuoqihaiyu_Clicked()));

	disconnect(ui.pushButton_Youqihaiyu, SIGNAL(clicked()), this, SLOT(On_pushButton_Youqihaiyu_Clicked()));
	connect(ui.pushButton_Youqihaiyu, SIGNAL(clicked()), this, SLOT(On_pushButton_Youqihaiyu_Clicked()));

	disconnect(ui.pushButton_ContinueModel, SIGNAL(clicked()), this, SLOT(On_pushButton_ContinueModel_Clicked()));
	connect(ui.pushButton_ContinueModel, SIGNAL(clicked()), this, SLOT(On_pushButton_ContinueModel_Clicked()));

	disconnect(ui.pushButton_StepModel, SIGNAL(clicked()), this, SLOT(On_pushButton_StepModel_Clicked()));
	connect(ui.pushButton_StepModel, SIGNAL(clicked()), this, SLOT(On_pushButton_StepModel_Clicked()));

	ui.pushButton_Decr->hide();
	ui.pushButton_Incr->hide();

	m_iFengfuTime = 0;
	m_iDazhui1Time = 0;
	m_iYaoyangguanTime = 0;
	m_iZuojianjingTime = 0;
	m_iZuoqihaiyuTime = 0;
	m_iYoujianjingTime = 0;
	m_iYouqihaiyuTime = 0;
}

AutoTreatOpenBack::~AutoTreatOpenBack()
{
}

void AutoTreatOpenBack::On_pushButton_BackToHome_Clicked()
{

}

void AutoTreatOpenBack::On_pushButton_LastStep_Clicked()
{

}
void AutoTreatOpenBack::On_pushButton_Confirm_Clicked()
{
	m_pAuto->SetWidgetTreatBegin(m_eModel, m_eType,m_iTotalTime);
}

void AutoTreatOpenBack::On_pushButton_NextStep_Clicked()
{

}

void AutoTreatOpenBack::On_pushButton_Fengfu_Clicked()
{
	ui.pushButton_Dazhui1->setIcon(QIcon(":/Dazhui.png"));
	m_iDazhui1Intensity = 0;
	m_iDazhui1Time = 0;

	ui.pushButton_Fengfu->setIcon(QIcon(":/FengfuPressed.png"));
	m_eType = FENGFU;
	m_pStartOrEndAdj->move(1220,360);
	m_pStartOrEndAdj->setWindowModality(Qt::ApplicationModal);
	m_pStartOrEndAdj->SetXueweiType(FENGFU);
	m_pStartOrEndAdj->show();
}

void AutoTreatOpenBack::On_pushButton_Dazhui1_Clicked()
{
	ui.pushButton_Fengfu->setIcon(QIcon(":/Fengfu.png"));
	m_iFengfuIntensity = 0;
	m_iFengfuTime = 0;
	m_eType = DAZHUI1;
	ui.pushButton_Dazhui1->setIcon(QIcon(":/DazhuiPressed.png"));
	m_pStartOrEndAdj->move(1220, 360);
	m_pStartOrEndAdj->setWindowModality(Qt::ApplicationModal);
	m_pStartOrEndAdj->SetXueweiType(DAZHUI1);
	m_pStartOrEndAdj->show();
}

void AutoTreatOpenBack::On_pushButton_Dazhui2_Clicked()
{
	ui.pushButton_Dazhui2->setIcon(QIcon(":/DazhuiPressed.png"));
	m_pPathwayAdj->move(1220, 360);
	m_pPathwayAdj->setWindowModality(Qt::ApplicationModal);
	m_pPathwayAdj->SetXueweiType(DAZHUI2);
	m_pPathwayAdj->show();
}

void AutoTreatOpenBack::On_pushButton_Yaoyangguan_Clicked()
{
	ui.pushButton_Yaoyangguan->setIcon(QIcon(":/YaoyangguanPushed.png"));
	m_pStartOrEndAdj->move(1220, 360);
	m_pStartOrEndAdj->SetTitleFinishAdjust();
	m_pStartOrEndAdj->setWindowModality(Qt::ApplicationModal);
	m_pStartOrEndAdj->SetXueweiType(YAOYANGGUAN);
	m_pStartOrEndAdj->show();
}

void AutoTreatOpenBack::On_pushButton_Shendao_Clicked()
{
	ui.pushButton_Shendao->setIcon(QIcon(":/ShendaoPressed.png"));
	m_pPathwayAdj->move(1220, 360);
	m_pPathwayAdj->setWindowModality(Qt::ApplicationModal);
	m_pPathwayAdj->SetXueweiType(SHENDAO);
	m_pPathwayAdj->show();
}

void AutoTreatOpenBack::On_pushButton_Zhiyang_Clicked()
{
	ui.pushButton_Zhiyang->setIcon(QIcon(":/ZhiyangPressed.png"));
	m_pPathwayAdj->move(1220, 360);
	m_pPathwayAdj->setWindowModality(Qt::ApplicationModal);
	m_pPathwayAdj->SetXueweiType(ZHIYANG);
	m_pPathwayAdj->show();
}

void AutoTreatOpenBack::On_pushButton_Zuojianjing_Clicked()
{
	ui.pushButton_Zuojianjing->setIcon(QIcon(":/ZuojianjingPushed.png"));
	m_pStartOrEndAdj->move(1220, 360);
	m_pStartOrEndAdj->setWindowModality(Qt::ApplicationModal);
	m_pStartOrEndAdj->SetXueweiType(ZUOJIANJING);
	m_pStartOrEndAdj->show();
}

void AutoTreatOpenBack::On_pushButton_Youjianjing_Clicked()
{
	ui.pushButton_Youjianjing->setIcon(QIcon(":/YoujianjingPressed.png"));
	m_pStartOrEndAdj->move(1220, 360);
	m_pStartOrEndAdj->setWindowModality(Qt::ApplicationModal);
	m_pStartOrEndAdj->SetXueweiType(YOUJIANJING);
	m_pStartOrEndAdj->show();
}

void AutoTreatOpenBack::On_pushButton_Zuoqihaiyu_Clicked()
{
	ui.pushButton_Zuoqihaiyu->setIcon(QIcon(":/ZuoqihaiyuPushed.png"));
	m_pStartOrEndAdj->move(1220, 360);
	m_pStartOrEndAdj->SetTitleFinishAdjust();
	m_pStartOrEndAdj->setWindowModality(Qt::ApplicationModal);
	m_pStartOrEndAdj->SetXueweiType(ZUOQIHAIYU);
	m_pStartOrEndAdj->show();
}

void AutoTreatOpenBack::On_pushButton_Youqihaiyu_Clicked()
{
	ui.pushButton_Youqihaiyu->setIcon(QIcon(":/YouqiyuhaiPressed.png"));
	m_pStartOrEndAdj->move(1220, 360);
	m_pStartOrEndAdj->SetTitleFinishAdjust();
	m_pStartOrEndAdj->setWindowModality(Qt::ApplicationModal);
	m_pStartOrEndAdj->SetXueweiType(YOUQIHAIYU);
	m_pStartOrEndAdj->show();
}

void AutoTreatOpenBack::On_pushButton_ContinueModel_Clicked()
{
	ui.pushButton_StepModel->setIcon(QIcon(":/Gear1.png"));
	ui.label_8->setPixmap(QPixmap(":/GrayCircle.png"));;

	ui.pushButton_ContinueModel->setIcon(QIcon(":/Gear1Pressed.png"));
	ui.pushButton_ContinueModel->setIconSize(QSize(126,70));
	m_pSpeedAdj->move(1220, 360);
	m_pSpeedAdj->setWindowModality(Qt::ApplicationModal);
	m_pSpeedAdj->show();

	m_eModel = MODEL_CONTINUE;
}
void AutoTreatOpenBack::On_pushButton_StepModel_Clicked()
{
	ui.pushButton_ContinueModel->setIcon(QIcon(":/Gear1.png"));
	ui.label_7->setPixmap(QPixmap(":/GrayCircle.png"));

	ui.pushButton_StepModel->setIcon(QIcon(":/Gear1Pressed.png"));
	ui.pushButton_StepModel->setIconSize(QSize(126, 70));
	m_pStepModelAdj->move(1220, 360);
	m_pStepModelAdj->setWindowModality(Qt::ApplicationModal);
	m_pStepModelAdj->show();

	m_eModel = MODEL_STEP;
}

void AutoTreatOpenBack::SetZhengjiPara(XUEWEI_TYPE type, int time, int intensity)
{
	if (type == FENGFU)
	{
		m_iFengfuTime = time;
		m_iFengfuIntensity = intensity;
		ui.pushButton_Fengfu->setIcon(QIcon(":/FengfuConfirmed.png"));
	}
	else if (type == DAZHUI1)
	{
		m_iDazhui1Time = time;
		m_iDazhui1Intensity = intensity;
		ui.pushButton_Dazhui1->setIcon(QIcon(":/DazhuiConfirmed.png"));
	}
	else if (type == YAOYANGGUAN)
	{
		m_iYaoyangguanTime = time;
		m_iYaoyangguanIntensity = intensity;
		ui.pushButton_Yaoyangguan->setIcon(QIcon(":/YaoyangguanConfirmed.png"));
	}
	else if (type == ZUOJIANJING)
	{
		m_iZuojianjingTime = time;
		m_iZuojianjingIntensity = intensity;
		ui.pushButton_Zuojianjing->setIcon(QIcon(":/ZuojianjingConfirmed.png"));
	}
	else if (type == ZUOQIHAIYU)
	{
		m_iZuoqihaiyuTime = time;
		m_iZuoqihaiyuIntensity = intensity;
		ui.pushButton_Zuoqihaiyu->setIcon(QIcon(":/ZuoqihaiyuConfirmed.png"));
	}
	else if (type == YOUJIANJING)
	{
		m_iYoujianjingTime = time;
		m_iYoujianjingIntensity = intensity;
		ui.pushButton_Youjianjing->setIcon(QIcon(":/YoujianjingConfirmed.png"));
	}
	else if (type == YOUQIHAIYU)
	{
		m_iYouqihaiyuTime = time;
		m_iYouqihaiyuIntensity = intensity;
		ui.pushButton_Youqihaiyu->setIcon(QIcon(":/YouqihaiyuConfirmed.png"));
	}

	m_iTotalTime = m_iFengfuTime+m_iDazhui1Time+m_iYaoyangguanTime+m_iZuojianjingTime
				   +m_iZuoqihaiyuTime+m_iYoujianjingTime+m_iYouqihaiyuTime;
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

	ui.label_Time->setText(strTime);
}
void AutoTreatOpenBack::SetContinueModelFlag()
{
	ui.label_7->setPixmap(QPixmap(":/BluePoint.png"));
}

void AutoTreatOpenBack::SetStepModelFlag()
{
	ui.label_8->setPixmap(QPixmap(":/BluePoint.png"));
}

void AutoTreatOpenBack::SetStepPara(int dist, int time, int intensity)
{
	m_iStepDistance = dist;
	m_iStepStandbyTime = time;
	m_iStepIntensity = intensity;
}

OPENBACK_MODEL AutoTreatOpenBack::GetModel()
{
	return m_eModel;
}

void AutoTreatOpenBack::SetSpeed(int speed)
{
	m_iContinueModelSpeed = speed;
}

void AutoTreatOpenBack::SetTujingPara(XUEWEI_TYPE type, int intensity)
{
	if (type == DAZHUI2)
	{
		m_iDazhui2Intensity = intensity;
		ui.pushButton_Dazhui2->setIcon(QIcon(":/DazhuiConfirmed.png"));
	}
	else if (type == SHENDAO)
	{
		m_iShendaoIntensity = intensity;
		ui.pushButton_Shendao->setIcon(QIcon(":/ShendaoConfirmed.png"));
	}
	else if (type == ZHIYANG)
	{
		m_iZhiyangIntensity = intensity;
		ui.pushButton_Zhiyang->setIcon(QIcon(":/ZhiyangConfirmed.png"));
	}
}