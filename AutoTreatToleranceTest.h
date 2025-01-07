#pragma once

#include <QWidget>
#include "ui_AutoTreatToleranceTest.h"

class AutoTreat;
class AutoTreatToleranceTest : public QWidget
{
	Q_OBJECT

public:
	AutoTreatToleranceTest(AutoTreat* treat,QWidget *parent = nullptr);
	~AutoTreatToleranceTest();

public slots:
	void on_Pushbutton_NextStep_Clicked();

	void on_Pushbutton_LastStep_Clicked();

private:
	Ui::AutoTreatToleranceTestClass ui;

	AutoTreat* m_pAutoTreat;
};
