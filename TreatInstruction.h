#pragma once

#include <QWidget>
#include "ui_AutoTreatInstruction.h"

class MyWindow;

class TreatInstruction : public QWidget
{
	Q_OBJECT

public:
	TreatInstruction(MyWindow* window,QWidget *parent = nullptr);
	~TreatInstruction();

public slots:
	void On_pushButton_Confirm_Clicked();

private:
	Ui::AutoTreatInstructionClass ui;

	MyWindow* m_pMyWindow;
};
