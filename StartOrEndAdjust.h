#pragma once

#include <QWidget>
#include "ui_StartOrEndAdjust.h"

#include "RobotComm.h"

class StartOrEndAdjust : public QWidget
{
	Q_OBJECT

public:
	StartOrEndAdjust(QWidget *parent = nullptr);
	~StartOrEndAdjust();

	void ResumePara();

	void SetTitleFinishAdjust();

	void SetXueweiType(XUEWEI_TYPE type);

public slots:
	void On_pushButton_Cancel_Clicked();
	void On_pushButton_Confirm_Clicked();

	void On_pushButton_IntensityDecr_Clicked();
	void On_pushButton_IntensityIncr_Clicked();

	void On_pushButton_TimerDecr_Clicked();
	void On_pushButton_TimerIncr_Clicked();

private:
	Ui::StartOrEndAdjustClass ui; 

	XUEWEI_TYPE m_eType;
	QWidget* m_pMyFatherWidget;
};
