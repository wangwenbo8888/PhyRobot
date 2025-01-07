#pragma once

#include <QWidget>
#include "ui_AutoTreatOnGoing.h"

class AutoTreat;
class AutoTreatOnGoing : public QWidget
{
	Q_OBJECT

public:
	AutoTreatOnGoing(AutoTreat* treat,QWidget *parent = nullptr);
	~AutoTreatOnGoing();

public slots:
	void On_pushButton_Back_Clicked();

	void On_pushButton_WorkContinue_Clicked();

	void On_pushButton_Pause_Clicked();

	void On_pushButton_Stop_Clicked();

private:
	Ui::AutoTreatOnGoingClass ui;

	AutoTreat* m_pAutoTreat;
};
