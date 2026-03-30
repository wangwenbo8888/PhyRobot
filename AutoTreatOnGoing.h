#pragma once

#include <QWidget>
#include "ui_AutoTreatOnGoing.h"

#include <QTimer>
#include "RobotComm.h"

class AutoTreat;
class AutoTreatOnGoing : public QWidget
{
	Q_OBJECT

public:
	AutoTreatOnGoing(AutoTreat* treat,QWidget *parent = nullptr);
	~AutoTreatOnGoing();

	void TimerStart();

	void TimerStop();

	void TimerContinue();

	void SetPlanImage();

	void SetAcupointLabels(const std::vector<RobotPoint>& points);

	void SetLabelTreated(int);

	void SetLabelTreating(int);

public slots:
	void On_pushButton_Back_Clicked();

	void On_pushButton_WorkContinue_Clicked();

	void On_pushButton_Pause_Clicked();

	void On_pushButton_Stop_Clicked();

	void On_TimeOut();

private:
	Ui::AutoTreatOnGoingClass ui;

	std::vector<QSharedPointer<QLabel>> m_vAcupoints;

	AutoTreat* m_pAutoTreat;

	QTimer* m_pTimer;

	quint64 m_iTotalTime;

	int m_iWide;
	int m_iHight;
};
