#include "AutoTreatStop.h"

#include "MyWindow.h"

AutoTreatStop::AutoTreatStop(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	m_pWindow = NULL;

	disconnect(ui.pushButton_Cancel,SIGNAL(clicked()),this,SLOT(On_pushButton_Cancel_Clicked()));
	connect(ui.pushButton_Cancel, SIGNAL(clicked()), this, SLOT(On_pushButton_Cancel_Clicked()));

	disconnect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(On_pushButton_Confirm_Clicked()));
	connect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(On_pushButton_Confirm_Clicked()));
}

AutoTreatStop::~AutoTreatStop()
{

}

void AutoTreatStop::SetWindow(MyWindow* window)
{
	m_pWindow = window;
}

void AutoTreatStop::On_pushButton_Cancel_Clicked()
{
	this->hide();
}

void AutoTreatStop::On_pushButton_Confirm_Clicked()
{
	this->hide();

	if (m_pWindow!=NULL)
	{
		m_pWindow->SetStoped(true);

		m_pWindow->ResetRobot();
		m_pWindow->RobotGoHome();
	}

	emit EmitConfirmed();
}
