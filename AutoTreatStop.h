#pragma once

#include <QWidget>
#include "ui_AutoTreatStop.h"

class MyWindow;

class AutoTreatStop : public QWidget
{
	Q_OBJECT

public:
	AutoTreatStop(QWidget *parent = nullptr);
	~AutoTreatStop();

	void SetWindow(MyWindow* window);

public slots:
	void On_pushButton_Cancel_Clicked();

	void On_pushButton_Confirm_Clicked();

 signals:

	 void EmitConfirmed();

private:
	Ui::AutoTreatStopClass ui;

	MyWindow* m_pWindow;
};
