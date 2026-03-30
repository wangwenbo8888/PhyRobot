#pragma once

#include <QWidget>
#include "ui_ManualTreatOutputAdj.h"

class ManualTreatOutputAdj : public QWidget
{
	Q_OBJECT

public:
	ManualTreatOutputAdj(QWidget *parent = nullptr);
	~ManualTreatOutputAdj();

private:
	Ui::ManualTreatOutputAdjClass ui;
};
