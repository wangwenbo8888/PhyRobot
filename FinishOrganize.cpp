#include "FinishOrganize.h"

FinishOrganize::FinishOrganize(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);
}

FinishOrganize::~FinishOrganize()
{
}
