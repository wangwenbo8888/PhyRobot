#pragma once

#include <QWidget>
#include "ui_AutoTreatLegOpen.h"

class AutoTreat;
// ÍÈ²¿³õ¿ª
class AutoTreatLegOpen : public QWidget
{
	Q_OBJECT

public:
	AutoTreatLegOpen(AutoTreat* treat,QWidget *parent = nullptr);
	~AutoTreatLegOpen();


public slots:
	void On_pushButton_IntensityDecr_Clicked();
	void On_pushButton_IntensityIncr_Clicked();

	void On_pushButton_TreatTimeDecr_Clicked();
	void On_pushButton_TreatTimeIncr_Clicked();

	void On_pushButton_BackToHome_Clicked();
	void On_pushButton_LastStep_Clicked();
	void On_pushButton_Confirm_Clicked();
	void On_pushButton_NextStep_Clicked();

private:
	Ui::AutoTreatLegOpenClass ui;
	AutoTreat* m_pAutoTreat;

public:
	int m_iLegOpenTime;
	int m_iLegOpenIntensity;
};
