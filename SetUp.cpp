#include "SetUp.h"

SetUp::SetUp(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);
}

SetUp::~SetUp()
{

}
