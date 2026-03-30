#include "MyWifi.h"

MyWifi::MyWifi(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);
}

MyWifi::~MyWifi()
{

}

