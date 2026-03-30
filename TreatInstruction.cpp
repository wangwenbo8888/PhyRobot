#include "TreatInstruction.h"

#include "MyWindow.h"

TreatInstruction::TreatInstruction(MyWindow* window,QWidget *parent)
	: m_pMyWindow(window)
	, QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(On_pushButton_Confirm_Clicked()));
	connect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(On_pushButton_Confirm_Clicked()));
}

TreatInstruction::~TreatInstruction()
{
}

void TreatInstruction::On_pushButton_Confirm_Clicked()
{
	m_pMyWindow->SetWidgetAfterInstruction();
}
