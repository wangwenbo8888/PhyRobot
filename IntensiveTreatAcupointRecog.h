#pragma once

#include <QWidget>
#include "ui_IntensiveTreatAcupointRecog.h"

class IntensiveTreat;
class IntensiveTreatAcupointRecog : public QWidget
{
	Q_OBJECT

public:
	IntensiveTreatAcupointRecog(IntensiveTreat* treat,QWidget *parent = nullptr);
	~IntensiveTreatAcupointRecog();

public slots:
	void on_Pushbutton_NextStep_Clicked();

	void on_Pushbutton_LastStep_Clicked();

private:
	Ui::IntensiveTreatAcupointRecogClass ui;

	IntensiveTreat* m_pIntensiveTreat;
};
