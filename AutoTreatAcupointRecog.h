#pragma once

#include <QWidget>
#include "ui_AutoTreatAcupointRecog.h"

class AutoTreat;
class AutoTreatAcupointRecog : public QWidget
{
	Q_OBJECT

public:
	AutoTreatAcupointRecog(AutoTreat* treat,QWidget *parent = nullptr);
	~AutoTreatAcupointRecog();

public slots:
	void on_Pushbutton_NextStep_Clicked();

	void on_Pushbutton_LastStep_Clicked();

	void on_Pushbutton_RecogBegin_Clicked();

    void on_Pushbutton_Manual_Clicked();

private:
	Ui::AutoTreatAcupointRecogClass ui;

    AutoTreat* m_pAutoTreat;
};
