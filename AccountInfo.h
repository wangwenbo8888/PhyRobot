#pragma once

#include <QWidget>
#include "ui_AccountInfo.h"

class MyWindow;

class AccountInfo : public QWidget
{
	Q_OBJECT

public:
	AccountInfo(MyWindow* window,QWidget *parent = nullptr);
	~AccountInfo();


public slots:
	void On_pushButton_Logout_Clicked();

private:
	Ui::AccountInfoClass ui;

	MyWindow* m_pWindow;
};
