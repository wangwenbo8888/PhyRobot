#pragma once

#include <QWidget>
#include "ui_HomePage.h"

class HomePage : public QWidget
{
	Q_OBJECT

public:
	HomePage(QWidget *parent = nullptr);
	~HomePage();

private:
	Ui::HomePageClass ui;
};
