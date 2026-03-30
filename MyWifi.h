#pragma once

#include <QWidget>
#include "ui_MyWifi.h"

class MyWifi : public QWidget
{
	Q_OBJECT

public:
	MyWifi(QWidget *parent = nullptr);
	~MyWifi();

private:
	Ui::MyWifiClass ui;
};

