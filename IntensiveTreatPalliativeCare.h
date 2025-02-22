#pragma once

#include <QWidget>
#include "ui_IntensiveTreatPalliativeCare.h"

class IntensiveTreat;
class IntensiveTreatPalliativeCare : public QWidget
{
	Q_OBJECT

public:
	IntensiveTreatPalliativeCare(IntensiveTreat* treat, QWidget *parent = nullptr);
	~IntensiveTreatPalliativeCare();

	void InitInterface();

public slots:
	void on_Pushbutton_NextStep_Clicked();

	void on_Pushbutton_Back_Clicked();

	void on_Pushbutton_Confirm_Clicked();

private:
	Ui::IntensiveTreatPalliativeCareClass ui;

	IntensiveTreat* m_pIntensiveTreat;
};
