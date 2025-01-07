#pragma once

#include <QWidget>
#include "ui_AutoTreatInstruction.h"

class AutoTreat;

class AutoTreatInstruction : public QWidget
{
	Q_OBJECT

public:
	AutoTreatInstruction(AutoTreat* treat,QWidget *parent = nullptr);
	~AutoTreatInstruction();

public slots:
	void On_pushButton_NextStep_Clicked();

private:
	Ui::AutoTreatInstructionClass ui;

	AutoTreat* m_pAutoTreat;
};
