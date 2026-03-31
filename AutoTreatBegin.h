#pragma once

#include <QWidget>
#include "ui_AutoTreatBegin.h"

#include "PauseWidget.h"
#include "AutoTreatStop.h"

#include <vector>
#include "RobotComm.h"

#include "QtRoundProgressBar.h"

#include "Communicate.h"

class MyWindow;
class AutoTreatOpenBack;

class AutoTreatBegin : public QWidget
{
	Q_OBJECT

public:
	AutoTreatBegin(MyWindow* window,QWidget *parent = nullptr);
	~AutoTreatBegin();

	void SetStepDistanceLabel();

	void SetCommunicate(Communicate* comm);

	void SetXuewei(XUEWEI_TYPE type);

	void SetTime(int time);

	void SetSpeed(int speed);

	void SetOpenBack(AutoTreatOpenBack* back);

	void SetStarted(bool);

public slots:
	void on_Pushbutton_StartOrStop_Clicked();

	void on_Pushbutton_LastStep_Clicked();

	void on_pushButton_Back_Clicked();

	void on_pushButton_PauseOrContinue_clicked();

	void On_pushButton_Recognize_Clicked();

	void On_pushButton_Drag_Clicked();

	void On_FinishOneXuewei(int minute);

	void On_FinishOneGroup();

	void On_FinishOneSecond();

	void On_pushButton_IntensityIncr_Clicked();

	void On_pushButton_IntensityDecr_Clicked();

private:
	void SetLabelFromValue(int value,QLabel* label);

	Ui::AutoTreatBeginClass ui;

	QSharedPointer<AutoTreatStop> m_pStopWidget;
	QSharedPointer<PauseWidget> m_pPauseWidget;
	MyWindow* m_pWindow;
	AutoTreatOpenBack* m_pOpenBack;

	QSharedPointer<RoundProgressBar> barDumai;
	QSharedPointer< RoundProgressBar> barZuopangguangjing;
	QSharedPointer< RoundProgressBar> barYoupangguangjing;

	bool m_bStarted;
	int m_iTotalTime;
	int m_iTotalSecond;

	int m_iDumaiTime;
	int m_iZuopangguangjingTime;
	int m_iYoupangguangjingTime;

	int m_iSpeed;

	std::vector<QSharedPointer<QLabel>> m_vAcupoints;

	Communicate* m_pCommunicate;

	XUEWEI_TYPE m_eType;
};
