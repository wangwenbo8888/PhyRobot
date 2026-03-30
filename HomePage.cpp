#include "HomePage.h"
#include "MyWindow.h"

HomePage::HomePage(MyWindow* window,QWidget *parent)
	:m_pMyWindow(window)
	,QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);
	
	disconnect(ui.pushButton_Auto, SIGNAL(clicked()), this, SLOT(On_pushButton_Auto_Clicked()));
	connect(ui.pushButton_Auto, SIGNAL(clicked()), this, SLOT(On_pushButton_Auto_Clicked()));

	disconnect(ui.pushButton_Manual, SIGNAL(clicked()), this, SLOT(On_pushButton_Manual_Clicked()));
	connect(ui.pushButton_Manual, SIGNAL(clicked()), this, SLOT(On_pushButton_Manual_Clicked()));

	disconnect(ui.pushButton_Plan, SIGNAL(clicked()), this, SLOT(On_pushButton_Plan_Clicked()));
	connect(ui.pushButton_Plan, SIGNAL(clicked()), this, SLOT(On_pushButton_Plan_Clicked()));

	disconnect(ui.pushButton_Set, SIGNAL(clicked()), this, SLOT(On_pushButton_Set_Clicked()));
	connect(ui.pushButton_Set, SIGNAL(clicked()), this, SLOT(On_pushButton_Set_Clicked()));
}

HomePage::~HomePage()
{
}


void HomePage::On_pushButton_Auto_Clicked()
{
	ui.pushButton_Auto->setIcon(QIcon(":/HomeAutoPushed.png"));
	m_pMyWindow->SetWidgetInstruction(Treat_Auto);
}

void HomePage::On_pushButton_Manual_Clicked()
{
	ui.pushButton_Manual->setIcon(QIcon(":/HomeManualPushed.png"));
	m_pMyWindow->SetWidgetInstruction(Treat_Manual);
}

void HomePage::On_pushButton_Plan_Clicked()
{
	ui.pushButton_Plan->setIcon(QIcon(":/HomePlanPushed.png"));
}

void HomePage::On_pushButton_Set_Clicked()
{
	ui.pushButton_Set->setIcon(QIcon(":/HomeSetPushed.png"));
	m_pMyWindow->SetWidgetSetUp();
}