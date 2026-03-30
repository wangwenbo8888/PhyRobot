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

	void On_pushButton_BackToHome_Clicked();

	void On_pushButton_DragModel_Clicked();

	void On_pushButton_HandleModel_Clicked();

private:
	Ui::IntensiveTreatAcupointRecogClass ui;

	IntensiveTreat* m_pIntensiveTreat;
};
