#include "HomePage.h"

HomePage::HomePage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);
}

HomePage::~HomePage()
{
}
