#pragma once

#include <QWidget>
#include "ui_IntensiveTreatToleranceTest.h"

class IntensiveTreat;
class IntensiveTreatToleranceTest : public QWidget
{
	Q_OBJECT

public:
	IntensiveTreatToleranceTest(IntensiveTreat* treat, QWidget *parent = nullptr);
	~IntensiveTreatToleranceTest();

public slots:
	void on_Pushbutton_NextStep_Clicked();

	void on_Pushbutton_LastStep_Clicked();

private:
	Ui::IntensiveTreatToleranceTestClass ui;

	IntensiveTreat* m_pIntensiveTreat;
};
