#include "AccountInfo.h"

#include "MyWindow.h"

AccountInfo::AccountInfo(MyWindow* window, QWidget *parent)
	: m_pWindow(window)
	, QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint| Qt::WindowStaysOnTopHint);

	disconnect(ui.pushButton_Logout, SIGNAL(clicked()), this, SLOT(On_pushButton_Logout_Clicked()));
	connect(ui.pushButton_Logout, SIGNAL(clicked()), this, SLOT(On_pushButton_Logout_Clicked()));
}

AccountInfo::~AccountInfo()
{
}

void AccountInfo::On_pushButton_Logout_Clicked()
{
	this->hide();
	m_pWindow->On_PushButton_Exit_Clicked();
}
