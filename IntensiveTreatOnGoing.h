#pragma once

#include <QWidget>
#include "ui_IntensiveTreatOnGoing.h"

class IntensiveTreat;
class IntensiveTreatOnGoing : public QWidget
{
	Q_OBJECT

public:
	IntensiveTreatOnGoing(IntensiveTreat* treat, QWidget *parent = nullptr);
	~IntensiveTreatOnGoing();

public slots:
	void On_pushButton_Back_Clicked();

	void On_pushButton_WorkContinue_Clicked();

private:
	Ui::IntensiveTreatOnGoingClass ui;

	IntensiveTreat* m_pIntensiveTreat;
};
