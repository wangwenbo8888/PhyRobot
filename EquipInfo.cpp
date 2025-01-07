#include "EquipInfo.h"

EquipInfo::EquipInfo(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);
}

EquipInfo::~EquipInfo()
{}
