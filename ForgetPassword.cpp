#include "ForgetPassword.h"

ForgetPassword::ForgetPassword(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_Logon, SIGNAL(clicked()), this, SLOT(On_Pushbutton_Logon_Clicked()));
	connect(ui.pushButton_Logon, SIGNAL(clicked()), this, SLOT(On_Pushbutton_Logon_Clicked()));

}

ForgetPassword::~ForgetPassword()
{
}

void ForgetPassword::On_Pushbutton_Logon_Clicked()
{
}





