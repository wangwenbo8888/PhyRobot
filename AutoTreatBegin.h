#pragma once

#include <QWidget>
#include "ui_AutoTreatBegin.h"

class AutoTreat;
class AutoTreatBegin : public QWidget
{
	Q_OBJECT

public:
	AutoTreatBegin(AutoTreat* treat,QWidget *parent = nullptr);
	~AutoTreatBegin();

public slots:
	void on_Pushbutton_WorkBegin_Clicked();

	void on_Pushbutton_LastStep_Clicked();

private:
	Ui::AutoTreatBeginClass ui;

	AutoTreat* m_pAutoTreat;
};
