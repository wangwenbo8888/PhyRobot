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

	void On_checkBox_DragTeachMode_stateChanged(int state);

	void On_spinBox_PayloadMass_valueChanged(double v);

	void On_spinBox_PayloadX_valueChanged(double v);

	void On_spinBox_PayloadY_valueChanged(double v);

	void On_spinBox_PayloadZ_valueChanged(double v);

private:
	Ui::SetUpClass ui;

	MyWindow* m_pWindow;

	QtWidgetsCommunicateTest* m_pCommTestWidget;

	bool m_bUpdatingPayload = false;
};
