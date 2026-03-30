#pragma once

#include <QWidget>
#include "ui_MoveSpeedAdjust.h"

#include "RobotComm.h"

class MoveSpeedAdjust : public QWidget
{
	Q_OBJECT

public:
	MoveSpeedAdjust(QWidget *parent = nullptr);
	~MoveSpeedAdjust();

	void ResumePara();

public slots:
	void On_pushButton_Cancel_Clicked();
	void On_pushButton_Confirm_Clicked();

	void On_pushButton_SpeedDecr_Clicked();
	void On_pushButton_SpeedIncr_Clicked();

private:
	Ui::MoveSpeedAdjustClass ui;

	QWidget* m_pFatherWidget;
};
