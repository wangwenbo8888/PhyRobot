#pragma once

#include <QWidget>
#include "ui_SetUp.h"

class SetUp : public QWidget
{
	Q_OBJECT

public:
	SetUp(QWidget *parent = nullptr);
	~SetUp();

private:
	Ui::SetUpClass ui;
};
