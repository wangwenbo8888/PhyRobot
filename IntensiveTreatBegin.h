#pragma once

#include <QWidget>
#include "ui_IntensiveTreatBegin.h"

class IntensiveTreat;
class IntensiveTreatBegin : public QWidget
{
	Q_OBJECT

public:
	IntensiveTreatBegin(IntensiveTreat* treat, QWidget *parent = nullptr);
	~IntensiveTreatBegin();

public slots:
	void on_Pushbutton_StartOrStop_Clicked();

	void on_Pushbutton_LastStep_Clicked();

private:
	Ui::IntensiveTreatBeginClass ui;

	IntensiveTreat* m_pIntensiveTreat;
};
