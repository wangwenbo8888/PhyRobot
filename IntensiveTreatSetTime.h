#pragma once

#include <QWidget>
#include "ui_IntensiveTreatSetTime.h"

class IntensiveTreat;
class IntensiveTreatSetTime : public QWidget
{
	Q_OBJECT

public:
	IntensiveTreatSetTime(IntensiveTreat* treat, QWidget *parent = nullptr);
	~IntensiveTreatSetTime();

	void InitInterface();

public slots:
	void on_Pushbutton_NextStep_Clicked();

	void on_Pushbutton_LastStep_Clicked();

	void on_Pushbutton_Confirm_Clicked();

private:
	Ui::IntensiveTreatSetTimeClass ui;

	IntensiveTreat* m_pIntensiveTreat;
};
