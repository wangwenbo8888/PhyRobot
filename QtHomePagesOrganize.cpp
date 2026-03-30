#include "QtHomePagesOrganize.h"

#include "HomePage.h"

QtHomePagesOrganize::QtHomePagesOrganize(HomePage* home,QWidget *parent)
	:m_pHome(home)
	,QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_Plan, SIGNAL(clicked()), this, SLOT(on_Pushbutton_Plan_clicked()));
	connect(ui.pushButton_Plan,SIGNAL(clicked()),this,SLOT(on_Pushbutton_Plan_clicked()));
}

QtHomePagesOrganize::~QtHomePagesOrganize()
{
}

void QtHomePagesOrganize::on_Pushbutton_Plan_clicked()
{
	//m_pHome->setOrganizeInstructPage();
	ui.label_Left->setStyleSheet("background-color:#41FFC5;");
	ui.label_Right->setStyleSheet("background-color:#41FFC5;");
}
