#include "AutoTreatFinish.h"

#include "AutoTreat.h"

AutoTreatFinish::AutoTreatFinish(AutoTreat* treat, QWidget *parent)
	: m_pAutoTreat(treat)
	, QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_FinishReturn, SIGNAL(clicked()), this, SLOT(On_PushButton_FinishReturn()));
	connect(ui.pushButton_FinishReturn, SIGNAL(clicked()), this, SLOT(On_PushButton_FinishReturn()));
}

AutoTreatFinish::~AutoTreatFinish()
{
}

void AutoTreatFinish::On_PushButton_FinishReturn()
{
	m_pAutoTreat->FinishReturn();
}
