#include "AutoTreatFinish.h"

#include "AutoTreat.h"

AutoTreatFinish::AutoTreatFinish(AutoTreat* treat, QWidget *parent)
	: m_pAutoTreat(treat)
	, QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);
}

AutoTreatFinish::~AutoTreatFinish()
{
}
