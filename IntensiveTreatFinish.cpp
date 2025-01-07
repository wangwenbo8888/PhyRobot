#include "IntensiveTreatFinish.h"

IntensiveTreatFinish::IntensiveTreatFinish(IntensiveTreat* treat, QWidget *parent)
	: m_pIntensiveTreat(treat)
	, QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);
}

IntensiveTreatFinish::~IntensiveTreatFinish()
{}
