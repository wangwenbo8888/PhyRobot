#pragma once

#include <QWidget>
#include "ui_AutoTreatPrePrepare.h"

class AutoTreat;
class AutoTreatPrePrepare : public QWidget
{
	Q_OBJECT

public:
	AutoTreatPrePrepare(AutoTreat* treat,QWidget *parent = nullptr);
	~AutoTreatPrePrepare();

public slots:
	void on_Pushbutton_NextStep_Clicked();

	void on_Pushbutton_LastStep_Clicked();

private:
	Ui::AutoTreatPrePrepareClass ui;

	AutoTreat* m_pAutoTreat;
};
