#pragma once

#include <QWidget>
#include "ui_QtHomePagesManualInstruction.h"

class QtHomePagesManualInstruction : public QWidget
{
	Q_OBJECT

public:
	QtHomePagesManualInstruction(QWidget *parent = nullptr);
	~QtHomePagesManualInstruction();

private:
	Ui::QtHomePagesManualInstructionClass ui;
};
