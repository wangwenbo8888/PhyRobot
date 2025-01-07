#pragma once

#include <QWidget>
#include "ui_FinishOrganize.h"

class FinishOrganize : public QWidget
{
	Q_OBJECT

public:
	FinishOrganize(QWidget *parent = nullptr);
	~FinishOrganize();

private:
	Ui::FinishOrganizeClass ui;
};
