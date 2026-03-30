#pragma once

#include <QWidget>
#include "ui_AutoTreatShoulderUnblock.h"

#include "ZhengjixueweiAdj.h"
#include "RobotComm.h"
class AutoTreat;
// 自动治疗肩部疏通
class AutoTreatShoulderUnblock : public QWidget
{
	Q_OBJECT

public:
	AutoTreatShoulderUnblock(AutoTreat* treat,QWidget *parent = nullptr);
	~AutoTreatShoulderUnblock();

	void SetZhengjixueweiPara(XUEWEI_TYPE type,int time,int intensity);

public slots:
	void On_pushButton_Fengfu1_Clicked();
	void On_pushButton_Fengchi1_Clicked();
	void On_pushButton_Youneiguan1_Clicked();
	void On_pushButton_Youwaiguan1_Clicked();
	void On_pushButton_Fengfu2_Clicked();
	void On_pushButton_Fengchi2_Clicked();
	void On_pushButton_Zuoneiguan1_Clicked();
	void On_pushButton_Zuowaiguan1_Clicked();

	void On_pushButton_TimeDecr_Clicked();
	void On_pushButton_TimeIncr_Clicked();

	void On_pushButton_SpeedDecr_Clicked();
	void On_pushButton_SpeedIncr_Clicked();

	void On_pushButton_BackToHome_Clicked();
	void On_pushButton_LastStep_Clicked();
	void On_pushButton_Confirm_Clicked();
	void On_pushButton_NextStep_Clicked();

private:
	Ui::AutoTreatShoulderUnblockClass ui;
	AutoTreat* m_pAutoTreat;

	QSharedPointer<ZhengjixueweiAdj> m_pZhengjixueweiAdj;

public:
	int m_iFengfu1Time;
	int m_iFengfu1Intensity;

	int m_iFengchi1Time;
	int m_iFengchi1Intensity;

	int m_iFengfu2Time;
	int m_iFengfu2Intensity;

	int m_iFengchi2Time;
	int m_iFengchi2Intensity;

	int m_iTotalTime;

	int m_iSpeed;
	int m_iTime;
};
