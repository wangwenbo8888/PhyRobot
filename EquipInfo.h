#pragma once

#include <QWidget>
#include "ui_EquipInfo.h"

class EquipInfo : public QWidget
{
	Q_OBJECT

public:
	EquipInfo(QWidget *parent = nullptr);
	~EquipInfo();

private:
	Ui::EquipInfoClass ui;
};
