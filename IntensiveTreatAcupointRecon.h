#pragma once

#include <QWidget>
#include "ui_IntensiveTreatAcupointRecon.h"

class IntensiveTreatAcupointRecon : public QWidget
{
	Q_OBJECT

public:
	IntensiveTreatAcupointRecon(QWidget *parent = nullptr);
	~IntensiveTreatAcupointRecon();

private:
	Ui::IntensiveTreatAcupointReconClass ui;
};
