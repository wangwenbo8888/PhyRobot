#pragma once

#include <QWidget>
#include "ui_AutoTreatSetTime.h"

class AutoTreat;
class AutoTreatSetTime : public QWidget
{
	Q_OBJECT

public:
	AutoTreatSetTime(AutoTreat* treat,QWidget *parent = nullptr);
	~AutoTreatSetTime();

public slots:
	void on_Pushbutton_NextStep_Clicked();

	void on_Pushbutton_LastStep_Clicked();

private:
	Ui::AutoTreatSetTimeClass ui;

	AutoTreat* m_pAutoTreat;
};
