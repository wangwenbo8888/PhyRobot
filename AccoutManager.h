#pragma once

#include <QWidget>
#include "ui_AccoutManager.h"

class AccoutManager : public QWidget
{
	Q_OBJECT

public:
	AccoutManager(QWidget *parent = nullptr);
	~AccoutManager();

private:
	Ui::AccoutManagerClass ui;
};
