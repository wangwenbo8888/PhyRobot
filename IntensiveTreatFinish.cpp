#include "IntensiveTreatFinish.h"

#include "IntensiveTreat.h"

IntensiveTreatFinish::IntensiveTreatFinish(IntensiveTreat* treat, QWidget *parent)
	: m_pIntensiveTreat(treat)
	, QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_FinishReturn,SIGNAL(clicked()),this,SLOT(On_PushButton_FinishReturn()));
	connect(ui.pushButton_FinishReturn, SIGNAL(clicked()), this, SLOT(On_PushButton_FinishReturn()));
}

IntensiveTreatFinish::~IntensiveTreatFinish()
{}

void IntensiveTreatFinish::On_PushButton_FinishReturn()
{
	m_pIntensiveTreat->FinishReturn();
}
