#include "AutoTreatFinish.h"

#include "AutoTreat.h"

AutoTreatFinish::AutoTreatFinish(AutoTreat* treat, QWidget *parent)
	: m_pAutoTreat(treat)
	, QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_BackToHome, SIGNAL(clicked()), this, SLOT(On_PushButton_BackToHome()));
	connect(ui.pushButton_BackToHome, SIGNAL(clicked()), this, SLOT(On_PushButton_BackToHome()));
}

void AutoTreatFinish::SetCurrentProj(QString str)
{
	ui.label_Proj->setText(str);
}

AutoTreatFinish::~AutoTreatFinish()
{
}

void AutoTreatFinish::On_PushButton_BackToHome()
{
	m_pAutoTreat->FinishReturn();
}
