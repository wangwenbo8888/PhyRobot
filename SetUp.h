#pragma once

#include <QWidget>
#include "ui_SetUp.h"

class MyWindow;
class QtWidgetsCommunicateTest;

class SetUp : public QWidget
{
	Q_OBJECT

public:
	SetUp(MyWindow* window,QWidget *parent = nullptr);
	~SetUp();

public slots:
	void On_pushButton_RobotStorage_Clicked();

	void On_pushButton_RobotResume_Clicked();

	void On_pushButton_CommunicateTest_Clicked();

	void On_pushButton_ReturnHome_Clicked();

private:
	Ui::SetUpClass ui;

	MyWindow* m_pWindow;

	QtWidgetsCommunicateTest* m_pCommTestWidget;
};
