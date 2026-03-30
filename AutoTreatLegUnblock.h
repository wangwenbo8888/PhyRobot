#pragma once

#include <QWidget>
#include "ui_AutoTreatLegUnblock.h"

#include "ZhengjixueweiAdj.h"

class AutoTreat;
// Õ»≤ø ËÕ®
class AutoTreatLegUnblock : public QWidget
{
	Q_OBJECT

public:
	AutoTreatLegUnblock(AutoTreat* treat,QWidget *parent = nullptr);
	~AutoTreatLegUnblock();

	void SetZhengjiPara(XUEWEI_TYPE, int minute, int intensity);

public slots:
	void On_pushButton_Shangliao1_Clicked();
	void On_pushButton_Youyaoyan1_Clicked();
	void On_pushButton_Shenyu1_Clicked();
	void On_pushButton_Yaoyangguan1_Clicked();
	void On_pushButton_Youweizhong1_Clicked();
	void On_pushButton_Youweiyang1_Clicked();
	void On_pushButton_Shangliao2_Clicked();
	void On_pushButton_Zuoyaoyan1_Clicked();
	void On_pushButton_Shenyu2_Clicked();
	void On_pushButton_Yaoyangguan2_Clicked();
	void On_pushButton_Zuoweizhong1_Clicked();
	void On_pushButton_Zuoweiyang1_Clicked();
	void On_pushButton_Shangliao3_Clicked();
	void On_pushButton_Youyaoyan2_Clicked();
	void On_pushButton_Shenyu3_Clicked();
	void On_pushButton_Yaoyangguan3_Clicked();
	void On_pushButton_Youkunlun1_Clicked();
	void On_pushButton_Youjiexi1_Clicked();
	void On_pushButton_Shangliao4_Clicked();
	void On_pushButton_Zuoyaoyan2_Clicked();
	void On_pushButton_Shenyu4_Clicked();
	void On_pushButton_Yaoyangguan4_Clicked();
	void On_pushButton_Zuokunlun1_Clicked();
	void On_pushButton_Zuojiexi1_Clicked();

	void On_pushButton_BackToHome_Clicked();
	void On_pushButton_LastStep_Clicked();
	void On_pushButton_Confirm_Clicked();
	void On_pushButton_NextStep_Clicked();

private:

public:
	int m_iShangliao1Time;
	int m_iShangliao1Intensity;

	int m_iYouyaoyan1Time;
	int m_iYouyaoyan1Intensity;

	int m_iShenyu1Time;
	int m_iShenyu1Intensity;

	int m_iYaoyangguan1Time;
	int m_iYaoyangguan1Intensity;

	int m_iShangliao2Time;
	int m_iShangliao2Intensity;

	int m_iZuoyaoyan1Time;
	int m_iZuoyaoyan1Intensity;

	int m_iShenyu2Time;
	int m_iShenyu2Intensity;

	int m_iYaoyangguan2Time;
	int m_iYaoyangguan2Intensity;

	int m_iShangliao3Time;
	int m_iShangliao3Intensity;

	int m_iYouyaoyan2Time;
	int m_iYouyaoyan2Intensity;

	int m_iShenyu3Time;
	int m_iShenyu3Intensity;

	int m_iYaoyangguan3Time;
	int m_iYaoyangguan3Intensity;

	int m_iShangliao4Time;
	int m_iShangliao4Intensity;

	int m_iZuoyaoyan2Time;
	int m_iZuoyaoyan2Intensity;

	int m_iShenyu4Time;
	int m_iShenyu4Intensity;

	int m_iYaoyangguan4Time;
	int m_iYaoyangguan4Intensity;

	int m_iTotalTreatTime;

private:
	Ui::AutoTreatLegUnblockClass ui;
	AutoTreat* m_pAutoTreat;

	QSharedPointer<ZhengjixueweiAdj> m_pZhengjixueweiAdj;
};
