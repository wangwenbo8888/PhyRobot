#pragma once

#include <QWidget>
#include "ui_AutoTreatShoulderUnblockBegin.h"

#include "QtRoundProgressBar.h"
#include "PauseWidget.h"
#include "AutoTreatStop.h"

class MyWindow;
class AutoTreatShoulderUnblock;

class AutoTreatShoulderUnblockBegin : public QWidget
{
	Q_OBJECT

public:
	AutoTreatShoulderUnblockBegin(MyWindow* window,QWidget *parent = nullptr);
	~AutoTreatShoulderUnblockBegin();

	void SetShoulderUnblock(AutoTreatShoulderUnblock* shoulder);

public slots:

	void On_pushButton_Back_Clicked();
	void On_pushButton_LastStep_Clicked();
	void On_pushButton_PauseOrContinue_Clicked();
	void On_pushButton_StartOrStop_Clicked();

	void On_pushButton_Recognize_Clicked();
	void On_pushButton_Drag_Clicked();

	void On_FinishOneXuewei(int minute);

	void On_FinishOneGroup();

private:
	void SetLabelFromValue(int value, QLabel* label);

	Ui::AutoTreatShoulderUnblockBeginClass ui;

	QSharedPointer<PauseWidget> m_pPauseWidget;
	QSharedPointer<AutoTreatStop> m_pStopWidget;

	QSharedPointer<RoundProgressBar> m_pRightShoulder;
	QSharedPointer<RoundProgressBar> m_pLeftShoulder;

	MyWindow* m_pWindow;
	AutoTreatShoulderUnblock* m_pShoulder;

	int m_iTotalRightShoulder;
	int m_iTotalLeftShoulder;
	int m_iTotalTime;

	bool m_bFirstStart;
};
