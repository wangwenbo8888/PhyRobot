#pragma once

#include <QWidget>
#include "ui_PathwayAdjust.h"

#include "RobotComm.h"
class PathwayAdjust : public QWidget
{
	Q_OBJECT

public:
	PathwayAdjust(QWidget *parent = nullptr);
	~PathwayAdjust();

	void ResumePara();

	void SetXueweiType(XUEWEI_TYPE type);

public slots:
	void On_pushButton_Cancel_Clicked();
	void On_pushButton_Confirm_Clicked();

	void On_pushButton_IntensityDecr_Clicked();
	void On_pushButton_IntensityIncr_Clicked();

private:
	Ui::PathwayAdjustClass ui;
	XUEWEI_TYPE m_eType;

	QWidget* m_pMyFatherWidget;
};
