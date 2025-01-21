#pragma once

#include <QWidget>
#include "ui_IntensiveTreatFinish.h"

class IntensiveTreat;
class IntensiveTreatFinish : public QWidget
{
	Q_OBJECT

public:
	IntensiveTreatFinish(IntensiveTreat* treat, QWidget *parent = nullptr);
	~IntensiveTreatFinish();

public slots:
	void On_PushButton_FinishReturn();

private:
	Ui::IntensiveTreatFinishClass ui;

	IntensiveTreat* m_pIntensiveTreat;
};
