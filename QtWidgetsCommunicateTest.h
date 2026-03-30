#pragma once

#include <QWidget>
#include "ui_QtWidgetsCommunicateTest.h"

#include "Communicate.h"

class MyWindow;

class QtWidgetsCommunicateTest : public QWidget
{
	Q_OBJECT

public:
	QtWidgetsCommunicateTest(MyWindow* window,QWidget *parent = nullptr);
	~QtWidgetsCommunicateTest();

	void SetCommunicate(Communicate* comm);

public slots:

	void on_ShowMessage(QString);

	void On_pushButton_OpenBack_Start_Clicked();

	void On_pushButton_OpenBack_Stop_Clicked();

	void On_pushButton_LegOpen_Start_Clicked();

	void On_pushButton_LegOpen_Stop_Clicked();

	void On_pushButton_LegUnblock_Right1_Start_Clicked();

	void On_pushButton_LegUnblock_Right1_Stop_Clicked();

	void On_pushButton_LegUnblock_Left1_Start_Clicked();

	void On_pushButton_LegUnblock_Left1_Stop_Clicked();

	void On_pushButton_LegUnblock_Right2_Start_Clicked();

	void On_pushButton_LegUnblock_Right2_Stop_Clicked();

	void On_pushButton_LegUnblock_Left2_Start_Clicked();

	void On_pushButton_LegUnblock_Left2_Stop_Clicked();

	void On_pushButton_ShoulderUnblock_Start_Clicked();

	void On_pushButton_ShoulderUnblock_Stop_Clicked();

	void On_pushButton_Handle_Start_Clicked();

	void On_pushButton_Handle_Stop_Clicked();

	void On_pushButton_IncrIntensity_Clicked();

	void On_pushButton_DecrIntensity_Clicked();

private:
	Ui::QtWidgetsCommunicateTestClass ui;

	Communicate* m_pCommunicate;
	MyWindow* m_pWindow;
	int intensity;
};

