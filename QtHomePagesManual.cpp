#include "QtHomePagesManual.h"

#include "HomePage.h"
#include "MyWindow.h"

QtHomePagesManual::QtHomePagesManual(MyWindow* window,QWidget *parent)
	:m_pMyWindow(window)
	,QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_Manual, SIGNAL(clicked()), this, SLOT(on_Pushbutton_Manual_clicked()));
	connect(ui.pushButton_Manual, SIGNAL(clicked()), this, SLOT(on_Pushbutton_Manual_clicked()));
}

QtHomePagesManual::~QtHomePagesManual()
{
}

void QtHomePagesManual::on_Pushbutton_Manual_clicked()
{
	//m_pHome->setManualInstructPage();
	ui.label_Top->setStyleSheet("background-color:#41FFC5;");
	ui.label_Bottom->setStyleSheet("background-color:#41FFC5;");
	m_pMyWindow->SetWidgetInstruction(Treat_Manual);
}
