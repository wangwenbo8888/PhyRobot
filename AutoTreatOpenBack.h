#pragma once

#include <QWidget>
#include "ui_AutoTreatOpenBack.h"

#include "StartOrEndAdjust.h"
#include "PathwayAdjust.h"
#include "MoveSpeedAdjust.h"
#include "StepModelAdjust.h"

#include "RobotComm.h"

class MyWindow;
class AutoTreat;
// 自动治疗开背
class AutoTreatOpenBack : public QWidget
{
	Q_OBJECT

public:
	AutoTreatOpenBack(AutoTreat* treat,QWidget *parent = nullptr);
	~AutoTreatOpenBack();

	void SetZhengjiPara(XUEWEI_TYPE type,int time,int intensity);

	void SetTujingPara(XUEWEI_TYPE type, int intensity);

	void SetSpeed(int speed);

	int GetSpeed()
	{
		return m_iContinueModelSpeed;
	}

	OPENBACK_MODEL GetModel();

	void SetContinueModelFlag();

	void SetStepModelFlag();

	void SetStepPara(int dist,int time,int intensity);

public slots:

	void On_pushButton_BackToHome_Clicked();
	void On_pushButton_LastStep_Clicked();
	void On_pushButton_Confirm_Clicked();
	void On_pushButton_NextStep_Clicked();

	void On_pushButton_Fengfu_Clicked();
	void On_pushButton_Dazhui1_Clicked();
	void On_pushButton_Dazhui2_Clicked();
	void On_pushButton_Yaoyangguan_Clicked();
	void On_pushButton_Shendao_Clicked();
	void On_pushButton_Zhiyang_Clicked();

	void On_pushButton_Zuojianjing_Clicked();
	void On_pushButton_Youjianjing_Clicked();
	void On_pushButton_Zuoqihaiyu_Clicked();
	void On_pushButton_Youqihaiyu_Clicked();

	void On_pushButton_ContinueModel_Clicked();
	void On_pushButton_StepModel_Clicked();

private:
	Ui::AutoTreatOpenBackClass ui;

public:
	int m_iFengfuTime;
	int m_iFengfuIntensity;

	int m_iDazhui1Time;
	int m_iDazhui1Intensity;

	int m_iDazhui2Intensity;
	int m_iShendaoIntensity;
	int m_iZhiyangIntensity;

	int m_iYaoyangguanTime;
	int m_iYaoyangguanIntensity;

	int m_iZuojianjingTime;
	int m_iZuojianjingIntensity;

	int m_iZuoqihaiyuTime;
	int m_iZuoqihaiyuIntensity;

	int m_iYoujianjingTime;
	int m_iYoujianjingIntensity;

	int m_iYouqihaiyuTime;
	int m_iYouqihaiyuIntensity;
private:

	int m_iTotalTime;
	int m_iContinueModelSpeed;

	// 步经模式参数
	int m_iStepDistance;
	int m_iStepStandbyTime;
	int m_iStepIntensity;

	OPENBACK_MODEL m_eModel;
	XUEWEI_TYPE m_eType;

	AutoTreat* m_pAuto;
	QSharedPointer<StartOrEndAdjust> m_pStartOrEndAdj;
	QSharedPointer<PathwayAdjust> m_pPathwayAdj;
	QSharedPointer<MoveSpeedAdjust> m_pSpeedAdj;
	QSharedPointer<StepModelAdjust> m_pStepModelAdj;

};
