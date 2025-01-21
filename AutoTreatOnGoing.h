#pragma once

#include <QWidget>
#include "ui_AutoTreatOnGoing.h"

#include <QTimer>

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

public slots:
	void On_pushButton_Back_Clicked();

	void On_pushButton_WorkContinue_Clicked();

	void On_pushButton_Pause_Clicked();

	void On_pushButton_Stop_Clicked();

	void On_TimeOut();

private:
	Ui::AutoTreatOnGoingClass ui;

	AutoTreat* m_pAutoTreat;

	QTimer* m_pTimer;

	quint64 m_iTotalTime;
};
