#pragma once

#include <QWidget>
#include "ui_IntensiveTreatOnGoing.h"

#include <QTimer>

class IntensiveTreat;
class IntensiveTreatOnGoing : public QWidget
{
	Q_OBJECT

public:
	IntensiveTreatOnGoing(IntensiveTreat* treat, QWidget *parent = nullptr);
	~IntensiveTreatOnGoing();

	void TimerStart();

	void TimerStop();

	void TimerContinue();

public slots:
	void On_pushButton_Back_Clicked();

	void On_pushButton_WorkContinue_Clicked();

	void On_TimeOut();

private:
	Ui::IntensiveTreatOnGoingClass ui;

	IntensiveTreat* m_pIntensiveTreat;

	QTimer* m_pTimer;

	quint64 m_iTotalTime;
};
