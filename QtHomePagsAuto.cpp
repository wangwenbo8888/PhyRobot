#include "QtHomePagsAuto.h"

#include "HomePage.h"
#include "MyWindow.h"

QtHomePagsAuto::QtHomePagsAuto(MyWindow* window,QWidget *parent)
	: m_pMyWindow(window)
	,QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_Auto, SIGNAL(clicked()), this, SLOT(on_Pushbutton_Auto_clicked()));
	connect(ui.pushButton_Auto,SIGNAL(clicked()),this,SLOT(on_Pushbutton_Auto_clicked()));

	RefreshButtonState();
}

void QtHomePagsAuto::RefreshButtonState()
{
	ui.pushButton_Auto->setEnabled(!m_pMyWindow->IsDragTeachMode());
}

QtHomePagsAuto::~QtHomePagsAuto()
{

}

void QtHomePagsAuto::on_Pushbutton_Auto_clicked()
{
	if (m_pMyWindow->IsDragTeachMode())
		return;
	//m_pHome->setAutoInstructPage();
	ui.label_Top->setStyleSheet("background-color:#41FFC5;");
	ui.label_Bottom->setStyleSheet("background-color:#41FFC5;");
	m_pMyWindow->SetWidgetInstruction(Treat_Auto);
}