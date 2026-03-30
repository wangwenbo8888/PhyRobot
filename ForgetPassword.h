#pragma once

#include <QWidget>
#include "ui_ForgetPassword.h"

class ForgetPassword : public QWidget
{
	Q_OBJECT

public:
	ForgetPassword(QWidget *parent = nullptr);
	~ForgetPassword();


public slots:
	void On_Pushbutton_Logon_Clicked();

private:
	Ui::ForgetPasswordClass ui;
};
