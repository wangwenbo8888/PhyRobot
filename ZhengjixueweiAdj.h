#pragma once

#include <QWidget>
#include "ui_ZhengjixueweiAdj.h"

#include "RobotComm.h"

class ZhengjixueweiAdj : public QWidget
{
	Q_OBJECT

public:
	ZhengjixueweiAdj(QWidget *parent = nullptr);
	~ZhengjixueweiAdj();

	void ResumePara();

	void SetXueweiType(XUEWEI_TYPE type);

	void SetWidgetType(WIDGET_TYPE type);

public slots:
	void On_pushButton_Cancel_Clicked();
	void On_pushButton_Confirm_Clicked();

	void On_pushButton_IntensityDecr_Clicked();
	void On_pushButton_IntensityIncr_Clicked();

	void On_pushButton_TimerDecr_Clicked();
	void On_pushButton_TimerIncr_Clicked();

private:
	Ui::ZhengjixueweiAdjClass ui;

	XUEWEI_TYPE m_eType;
	WIDGET_TYPE m_eWidget;
	QWidget* m_pMyFatherWidget;
};
