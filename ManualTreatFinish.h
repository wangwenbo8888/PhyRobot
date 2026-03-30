#pragma once

#include <QWidget>
#include "ui_ManualTreatFinish.h"

class ManualTreatFinish : public QWidget
{
	Q_OBJECT

public:
	ManualTreatFinish(QWidget *parent = nullptr);
	~ManualTreatFinish();

public slots:
	void On_pushButton_Back_Clicked();

	void On_pushButton_TreatContinue_Clicked();

	void On_pushButton_ModelSelect_Clicked();

private:
	Ui::ManualTreatFinishClass ui;
};
