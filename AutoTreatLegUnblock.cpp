#include "AutoTreatLegUnblock.h"

#include "AutoTreat.h"

AutoTreatLegUnblock::AutoTreatLegUnblock(AutoTreat* treat,QWidget *parent)
	:m_pAutoTreat(treat)
	,QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	m_pZhengjixueweiAdj.reset(new ZhengjixueweiAdj(this));
	m_pZhengjixueweiAdj->SetWidgetType(LEG_UNBLOCK);
	m_pZhengjixueweiAdj->hide();
	m_iShangliao1Time = 0;
	m_iYouyaoyan1Time = 0;
	m_iShenyu1Time = 0;
	m_iYaoyangguan1Time = 0;
	m_iShangliao2Time = 0;
	m_iZuoyaoyan1Time = 0;
	m_iShenyu2Time = 0;
	m_iYaoyangguan2Time = 0;
	m_iShangliao3Time = 0;
	m_iYouyaoyan2Time = 0;
	m_iShenyu3Time = 0;
	m_iYaoyangguan3Time = 0;
	m_iShangliao4Time = 0;
	m_iZuoyaoyan2Time = 0;
	m_iShenyu4Time = 0;
	m_iYaoyangguan4Time = 0;

	disconnect(ui.pushButton_Shangliao1,SIGNAL(clicked()),this,SLOT(On_pushButton_Shangliao1_Clicked()));
	connect(ui.pushButton_Shangliao1, SIGNAL(clicked()), this, SLOT(On_pushButton_Shangliao1_Clicked()));

	disconnect(ui.pushButton_Youyaoyan1, SIGNAL(clicked()), this, SLOT(On_pushButton_Youyaoyan1_Clicked()));
	connect(ui.pushButton_Youyaoyan1, SIGNAL(clicked()), this, SLOT(On_pushButton_Youyaoyan1_Clicked()));

	disconnect(ui.pushButton_Shenyu1, SIGNAL(clicked()), this, SLOT(On_pushButton_Shenyu1_Clicked()));
	connect(ui.pushButton_Shenyu1, SIGNAL(clicked()), this, SLOT(On_pushButton_Shenyu1_Clicked()));

	disconnect(ui.pushButton_Yaoyangguan1, SIGNAL(clicked()), this, SLOT(On_pushButton_Yaoyangguan1_Clicked()));
	connect(ui.pushButton_Yaoyangguan1, SIGNAL(clicked()), this, SLOT(On_pushButton_Yaoyangguan1_Clicked()));

	disconnect(ui.pushButton_Youweizhong1, SIGNAL(clicked()), this, SLOT(On_pushButton_Youweizhong1_Clicked()));
	connect(ui.pushButton_Youweizhong1, SIGNAL(clicked()), this, SLOT(On_pushButton_Youweizhong1_Clicked()));

	disconnect(ui.pushButton_Youweiyang1, SIGNAL(clicked()), this, SLOT(On_pushButton_Youweiyang1_Clicked()));
	connect(ui.pushButton_Youweiyang1, SIGNAL(clicked()), this, SLOT(On_pushButton_Youweiyang1_Clicked()));

	disconnect(ui.pushButton_Shangliao2, SIGNAL(clicked()), this, SLOT(On_pushButton_Shangliao2_Clicked()));
	connect(ui.pushButton_Shangliao2, SIGNAL(clicked()), this, SLOT(On_pushButton_Shangliao2_Clicked()));

	disconnect(ui.pushButton_Zuoyaoyan1, SIGNAL(clicked()), this, SLOT(On_pushButton_Zuoyaoyan1_Clicked()));
	connect(ui.pushButton_Zuoyaoyan1, SIGNAL(clicked()), this, SLOT(On_pushButton_Zuoyaoyan1_Clicked()));

	disconnect(ui.pushButton_Shenyu2, SIGNAL(clicked()), this, SLOT(On_pushButton_Shenyu2_Clicked()));
	connect(ui.pushButton_Shenyu2, SIGNAL(clicked()), this, SLOT(On_pushButton_Shenyu2_Clicked()));

	disconnect(ui.pushButton_Yaoyangguan2, SIGNAL(clicked()), this, SLOT(On_pushButton_Yaoyangguan2_Clicked()));
	connect(ui.pushButton_Yaoyangguan2, SIGNAL(clicked()), this, SLOT(On_pushButton_Yaoyangguan2_Clicked()));

	disconnect(ui.pushButton_Zuoweizhong1, SIGNAL(clicked()), this, SLOT(On_pushButton_Zuoweizhong1_Clicked()));
	connect(ui.pushButton_Zuoweizhong1, SIGNAL(clicked()), this, SLOT(On_pushButton_Zuoweizhong1_Clicked()));

	disconnect(ui.pushButton_Zuoweiyang1, SIGNAL(clicked()), this, SLOT(On_pushButton_Zuoweiyang1_Clicked()));
	connect(ui.pushButton_Zuoweiyang1, SIGNAL(clicked()), this, SLOT(On_pushButton_Zuoweiyang1_Clicked()));

	disconnect(ui.pushButton_Shangliao3, SIGNAL(clicked()), this, SLOT(On_pushButton_Shangliao3_Clicked()));
	connect(ui.pushButton_Shangliao3, SIGNAL(clicked()), this, SLOT(On_pushButton_Shangliao3_Clicked()));

	disconnect(ui.pushButton_Youyaoyan2, SIGNAL(clicked()), this, SLOT(On_pushButton_Youyaoyan2_Clicked()));
	connect(ui.pushButton_Youyaoyan2, SIGNAL(clicked()), this, SLOT(On_pushButton_Youyaoyan2_Clicked()));

	disconnect(ui.pushButton_Shenyu3, SIGNAL(clicked()), this, SLOT(On_pushButton_Shenyu3_Clicked()));
	connect(ui.pushButton_Shenyu3, SIGNAL(clicked()), this, SLOT(On_pushButton_Shenyu3_Clicked()));

	disconnect(ui.pushButton_Yaoyangguan3, SIGNAL(clicked()), this, SLOT(On_pushButton_Yaoyangguan3_Clicked()));
	connect(ui.pushButton_Yaoyangguan3, SIGNAL(clicked()), this, SLOT(On_pushButton_Yaoyangguan3_Clicked()));

	disconnect(ui.pushButton_Youkunlun1, SIGNAL(clicked()), this, SLOT(On_pushButton_Youkunlun1_Clicked()));
	connect(ui.pushButton_Youkunlun1, SIGNAL(clicked()), this, SLOT(On_pushButton_Youkunlun1_Clicked()));

	disconnect(ui.pushButton_Youjiexi1, SIGNAL(clicked()), this, SLOT(On_pushButton_Youjiexi1_Clicked()));
	connect(ui.pushButton_Youjiexi1, SIGNAL(clicked()), this, SLOT(On_pushButton_Youjiexi1_Clicked()));

	disconnect(ui.pushButton_Shangliao4, SIGNAL(clicked()), this, SLOT(On_pushButton_Shangliao4_Clicked()));
	connect(ui.pushButton_Shangliao4, SIGNAL(clicked()), this, SLOT(On_pushButton_Shangliao4_Clicked()));

	disconnect(ui.pushButton_Zuoyaoyan2, SIGNAL(clicked()), this, SLOT(On_pushButton_Zuoyaoyan2_Clicked()));
	connect(ui.pushButton_Zuoyaoyan2, SIGNAL(clicked()), this, SLOT(On_pushButton_Zuoyaoyan2_Clicked()));

	disconnect(ui.pushButton_Shenyu4, SIGNAL(clicked()), this, SLOT(On_pushButton_Shenyu4_Clicked()));
	connect(ui.pushButton_Shenyu4, SIGNAL(clicked()), this, SLOT(On_pushButton_Shenyu4_Clicked()));

	disconnect(ui.pushButton_Yaoyangguan4, SIGNAL(clicked()), this, SLOT(On_pushButton_Yaoyangguan4_Clicked()));
	connect(ui.pushButton_Yaoyangguan4, SIGNAL(clicked()), this, SLOT(On_pushButton_Yaoyangguan4_Clicked()));

	disconnect(ui.pushButton_Zuokunlun1, SIGNAL(clicked()), this, SLOT(On_pushButton_Zuokunlun1_Clicked()));
	connect(ui.pushButton_Zuokunlun1, SIGNAL(clicked()), this, SLOT(On_pushButton_Zuokunlun1_Clicked()));

	disconnect(ui.pushButton_Zuojiexi1, SIGNAL(clicked()), this, SLOT(On_pushButton_Zuojiexi1_Clicked()));
	connect(ui.pushButton_Zuojiexi1, SIGNAL(clicked()), this, SLOT(On_pushButton_Zuojiexi1_Clicked()));

	disconnect(ui.pushButton_BackToHome, SIGNAL(clicked()), this, SLOT(On_pushButton_BackToHome_Clicked()));
	connect(ui.pushButton_BackToHome, SIGNAL(clicked()), this, SLOT(On_pushButton_BackToHome_Clicked()));

	disconnect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(On_pushButton_LastStep_Clicked()));
	connect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(On_pushButton_LastStep_Clicked()));

	disconnect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(On_pushButton_Confirm_Clicked()));
	connect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(On_pushButton_Confirm_Clicked()));

	disconnect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(On_pushButton_NextStep_Clicked()));
	connect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(On_pushButton_NextStep_Clicked()));

	ui.pushButton_Decr->hide();
	ui.pushButton_Incr->hide();
}

AutoTreatLegUnblock::~AutoTreatLegUnblock()
{
}

void AutoTreatLegUnblock::SetZhengjiPara(XUEWEI_TYPE type, int minute, int intensity)
{
	if (type==SHANGLIAO1)
	{
		m_iShangliao1Time = minute;
		m_iShangliao1Intensity = intensity;
	}
	else if (type == YOUYAOYAN1)
	{
		m_iYouyaoyan1Time = minute;
		m_iYouyaoyan1Intensity = intensity;
	}
	else if (type == SHENYU1)
	{
		m_iShenyu1Time = minute;
		m_iShenyu1Intensity = intensity;
	}
	else if (type == YAOYANGGUAN1)
	{
		m_iYaoyangguan1Time = minute;
		m_iYaoyangguan1Intensity = intensity;
	}
	else if (type == SHANGLIAO2)
	{
		m_iShangliao2Time = minute;
		m_iShangliao2Intensity = intensity;
	}
	else if (type == ZUOYAOYAN1)
	{
		m_iZuoyaoyan1Time = minute;
		m_iZuoyaoyan1Intensity = intensity;
	}
	else if (type == SHENYU2)
	{
		m_iShenyu2Time = minute;
		m_iShenyu2Intensity = intensity;
	}
	else if (type == YAOYANGGUAN2)
	{
		m_iYaoyangguan2Time = minute;
		m_iYaoyangguan2Intensity = intensity;
	}
	else if (type == SHANGLIAO3)
	{
		m_iShangliao3Time = minute;
		m_iShangliao3Intensity = intensity;
	}
	else if (type == YOUYAOYAN2)
	{
		m_iYouyaoyan2Time = minute;
		m_iYouyaoyan2Intensity = intensity;
	}
	else if (type == SHENYU3)
	{
		m_iShenyu3Time = minute;
		m_iShenyu3Intensity = intensity;
	}
	else if (type == YAOYANGGUAN3)
	{
		m_iYaoyangguan3Time = minute;
		m_iYaoyangguan3Intensity = intensity;
	}
	else if (type == SHANGLIAO4)
	{
		m_iShangliao4Time = minute;
		m_iShangliao4Intensity = intensity;
	}
	else if (type == ZUOYAOYAN2)
	{
		m_iZuoyaoyan2Time = minute;
		m_iZuoyaoyan2Intensity = intensity;
	}
	else if (type == SHENYU4)
	{
		m_iShenyu4Time = minute;
		m_iShenyu4Intensity = intensity;
	}
	else if (type == YAOYANGGUAN4)
	{
		m_iYaoyangguan4Time = minute;
		m_iYaoyangguan4Intensity = intensity;
	}
}

void AutoTreatLegUnblock::On_pushButton_Shangliao1_Clicked()
{
	ui.pushButton_Shangliao1->setIcon(QIcon(":/ShangliaoPushed.png"));
	m_pZhengjixueweiAdj->move(1284, 322);
	m_pZhengjixueweiAdj->setWindowModality(Qt::ApplicationModal);
	m_pZhengjixueweiAdj->SetXueweiType(SHANGLIAO1);
	m_pZhengjixueweiAdj->show();
}
void AutoTreatLegUnblock::On_pushButton_Youyaoyan1_Clicked()
{
	ui.pushButton_Youyaoyan1->setIcon(QIcon(":/YouyaoyanPushed.png"));
	m_pZhengjixueweiAdj->move(1284, 322);
	m_pZhengjixueweiAdj->setWindowModality(Qt::ApplicationModal);
	m_pZhengjixueweiAdj->SetXueweiType(YOUYAOYAN1);
	m_pZhengjixueweiAdj->show();
}
void AutoTreatLegUnblock::On_pushButton_Shenyu1_Clicked()
{
	ui.pushButton_Shenyu1->setIcon(QIcon(":/ShenyuPushed.png"));
	m_pZhengjixueweiAdj->move(1284, 322);
	m_pZhengjixueweiAdj->setWindowModality(Qt::ApplicationModal);
	m_pZhengjixueweiAdj->SetXueweiType(SHENYU1);
	m_pZhengjixueweiAdj->show();
}
void AutoTreatLegUnblock::On_pushButton_Yaoyangguan1_Clicked()
{
	ui.pushButton_Yaoyangguan1->setIcon(QIcon(":/LegUnblockYaoyangguanPushed.png"));
	m_pZhengjixueweiAdj->move(1284, 322);
	m_pZhengjixueweiAdj->setWindowModality(Qt::ApplicationModal);
	m_pZhengjixueweiAdj->SetXueweiType(YAOYANGGUAN1);
	m_pZhengjixueweiAdj->show();
}
void AutoTreatLegUnblock::On_pushButton_Youweizhong1_Clicked()
{
}
void AutoTreatLegUnblock::On_pushButton_Youweiyang1_Clicked()
{

}
void AutoTreatLegUnblock::On_pushButton_Shangliao2_Clicked()
{
	ui.pushButton_Shangliao2->setIcon(QIcon(":/ShangliaoPushed.png"));
	m_pZhengjixueweiAdj->move(1284, 322);
	m_pZhengjixueweiAdj->setWindowModality(Qt::ApplicationModal);
	m_pZhengjixueweiAdj->SetXueweiType(SHANGLIAO2);
	m_pZhengjixueweiAdj->show();
}
void AutoTreatLegUnblock::On_pushButton_Zuoyaoyan1_Clicked()
{
	ui.pushButton_Zuoyaoyan1->setIcon(QIcon(":/ZuoyaoyanPushed.png"));
	m_pZhengjixueweiAdj->move(1284, 322);
	m_pZhengjixueweiAdj->setWindowModality(Qt::ApplicationModal);
	m_pZhengjixueweiAdj->SetXueweiType(ZUOYAOYAN1);
	m_pZhengjixueweiAdj->show();
}
void AutoTreatLegUnblock::On_pushButton_Shenyu2_Clicked()
{
	ui.pushButton_Shenyu2->setIcon(QIcon(":/ShenyuPushed.png"));
	m_pZhengjixueweiAdj->move(1284, 322);
	m_pZhengjixueweiAdj->setWindowModality(Qt::ApplicationModal);
	m_pZhengjixueweiAdj->SetXueweiType(SHENYU2);
	m_pZhengjixueweiAdj->show();
}
void AutoTreatLegUnblock::On_pushButton_Yaoyangguan2_Clicked()
{
	ui.pushButton_Yaoyangguan2->setIcon(QIcon(":/LegUnblockYaoyangguanPushed.png"));
	m_pZhengjixueweiAdj->move(1284, 322);
	m_pZhengjixueweiAdj->setWindowModality(Qt::ApplicationModal);
	m_pZhengjixueweiAdj->SetXueweiType(YAOYANGGUAN2);
	m_pZhengjixueweiAdj->show();
}
void AutoTreatLegUnblock::On_pushButton_Zuoweizhong1_Clicked()
{
}
void AutoTreatLegUnblock::On_pushButton_Zuoweiyang1_Clicked()
{

}
void AutoTreatLegUnblock::On_pushButton_Shangliao3_Clicked()
{
	ui.pushButton_Shangliao3->setIcon(QIcon(":/ShangliaoPushed.png"));
	m_pZhengjixueweiAdj->move(1284, 322);
	m_pZhengjixueweiAdj->setWindowModality(Qt::ApplicationModal);
	m_pZhengjixueweiAdj->SetXueweiType(SHANGLIAO3);
	m_pZhengjixueweiAdj->show();
}
void AutoTreatLegUnblock::On_pushButton_Youyaoyan2_Clicked()
{
	ui.pushButton_Youyaoyan2->setIcon(QIcon(":/YouyaoyanPushed.png"));
	m_pZhengjixueweiAdj->move(1284, 322);
	m_pZhengjixueweiAdj->setWindowModality(Qt::ApplicationModal);
	m_pZhengjixueweiAdj->SetXueweiType(YOUYAOYAN2);
	m_pZhengjixueweiAdj->show();
}
void AutoTreatLegUnblock::On_pushButton_Shenyu3_Clicked()
{
	ui.pushButton_Shenyu3->setIcon(QIcon(":/ShenyuPushed.png"));
	m_pZhengjixueweiAdj->move(1284, 322);
	m_pZhengjixueweiAdj->setWindowModality(Qt::ApplicationModal);
	m_pZhengjixueweiAdj->SetXueweiType(SHENYU3);
	m_pZhengjixueweiAdj->show();
}
void AutoTreatLegUnblock::On_pushButton_Yaoyangguan3_Clicked()
{
	ui.pushButton_Yaoyangguan3->setIcon(QIcon(":/LegUnblockYaoyangguanPushed.png"));
	m_pZhengjixueweiAdj->move(1284, 322);
	m_pZhengjixueweiAdj->setWindowModality(Qt::ApplicationModal);
	m_pZhengjixueweiAdj->SetXueweiType(YAOYANGGUAN3);
	m_pZhengjixueweiAdj->show();
}
void AutoTreatLegUnblock::On_pushButton_Youkunlun1_Clicked()
{

}
void AutoTreatLegUnblock::On_pushButton_Youjiexi1_Clicked()
{

}
void AutoTreatLegUnblock::On_pushButton_Shangliao4_Clicked()
{
	ui.pushButton_Shangliao4->setIcon(QIcon(":/ShangliaoPushed.png"));
	m_pZhengjixueweiAdj->move(1284, 322);
	m_pZhengjixueweiAdj->setWindowModality(Qt::ApplicationModal);
	m_pZhengjixueweiAdj->SetXueweiType(SHANGLIAO4);
	m_pZhengjixueweiAdj->show();
}
void AutoTreatLegUnblock::On_pushButton_Zuoyaoyan2_Clicked()
{
	ui.pushButton_Zuoyaoyan2->setIcon(QIcon(":/ZuoyaoyanPushed.png"));
	m_pZhengjixueweiAdj->move(1284, 322);
	m_pZhengjixueweiAdj->setWindowModality(Qt::ApplicationModal);
	m_pZhengjixueweiAdj->SetXueweiType(ZUOYAOYAN2);
	m_pZhengjixueweiAdj->show();
}
void AutoTreatLegUnblock::On_pushButton_Shenyu4_Clicked()
{
	ui.pushButton_Shenyu4->setIcon(QIcon(":/ShenyuPushed.png"));
	m_pZhengjixueweiAdj->move(1284, 322);
	m_pZhengjixueweiAdj->setWindowModality(Qt::ApplicationModal);
	m_pZhengjixueweiAdj->SetXueweiType(SHENYU4);
	m_pZhengjixueweiAdj->show();
}
void AutoTreatLegUnblock::On_pushButton_Yaoyangguan4_Clicked()
{
	ui.pushButton_Yaoyangguan4->setIcon(QIcon(":/LegUnblockYaoyangguanPushed.png"));
	m_pZhengjixueweiAdj->move(1284, 322);
	m_pZhengjixueweiAdj->setWindowModality(Qt::ApplicationModal);
	m_pZhengjixueweiAdj->SetXueweiType(YAOYANGGUAN4);
	m_pZhengjixueweiAdj->show();
}
void AutoTreatLegUnblock::On_pushButton_Zuokunlun1_Clicked()
{

}

void AutoTreatLegUnblock::On_pushButton_Zuojiexi1_Clicked()
{

}

void AutoTreatLegUnblock::On_pushButton_BackToHome_Clicked()
{

}

void AutoTreatLegUnblock::On_pushButton_LastStep_Clicked()
{

}

void AutoTreatLegUnblock::On_pushButton_Confirm_Clicked()
{

}

void AutoTreatLegUnblock::On_pushButton_NextStep_Clicked()
{
	m_iTotalTreatTime = m_iShangliao1Time + m_iYouyaoyan1Time + m_iShenyu1Time + m_iYaoyangguan1Time + m_iShangliao2Time + m_iZuoyaoyan1Time + m_iShenyu2Time + m_iYaoyangguan2Time + m_iShangliao3Time + m_iYouyaoyan2Time + m_iShenyu3Time + m_iYaoyangguan3Time + m_iShangliao4Time + m_iZuoyaoyan2Time + m_iShenyu4Time + m_iYaoyangguan4Time;
	m_pAutoTreat->SetWidgetTreatLegUnblockBegin();
}
