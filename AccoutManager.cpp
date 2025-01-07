#include "AccoutManager.h"

AccoutManager::AccoutManager(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);
}

AccoutManager::~AccoutManager()
{

}
