#pragma once

#include <QWidget>
#include "ui_StepModelAdjust.h"

class StepModelAdjust : public QWidget
{
	Q_OBJECT

public:
	StepModelAdjust(QWidget *parent = nullptr);
	~StepModelAdjust();

public slots:
	void On_pushButton_DistanceDecr_Clicked();
	void On_pushButton_DistanceIncr_Clicked();

	void On_pushButton_StandbyTimeDecr_Clicked();
	void On_pushButton_StandbyTimeIncr_Clicked();

	void On_pushButton_IntensityDecr_Clicked();
	void On_pushButton_IntensityIncr_Clicked();

	void On_pushButton_Cancel_Clicked();
	void On_pushButton_Confirm_Clicked();

private:
	void ResumePara();

private:
	Ui::StepModelAdjustClass ui;

	QWidget* m_pMyFatherWidget;
};
