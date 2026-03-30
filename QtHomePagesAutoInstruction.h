#pragma once

#include <QWidget>
#include "ui_QtHomePagesAutoInstruction.h"

class QtHomePagesAutoInstruction : public QWidget
{
	Q_OBJECT

public:
	QtHomePagesAutoInstruction(QWidget *parent = nullptr);
	~QtHomePagesAutoInstruction();

private:
	Ui::QtHomePagesAutoInstructionClass ui;
};
