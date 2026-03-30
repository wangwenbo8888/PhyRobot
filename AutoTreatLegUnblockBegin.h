#pragma once

#include <QWidget>
#include "ui_AutoTreatLegUnblockBegin.h"

#include "PauseWidget.h"
#include "AutoTreatStop.h"

#include "QtRoundProgressBar.h"

class MyWindow;
class AutoTreatLegUnblock;

class AutoTreatLegUnblockBegin : public QWidget
{
	Q_OBJECT

public:
	AutoTreatLegUnblockBegin(MyWindow* window,QWidget *parent = nullptr);
	~AutoTreatLegUnblockBegin();

	void SetLegUnblock(AutoTreatLegUnblock* leg);

	void SetTreatTime(int minute);

public slots:

	void On_pushButton_Back_Clicked();
	void On_pushButton_LastStep_Clicked();
	void On_pushButton_PauseOrContinue_Clicked();
	void On_pushButton_StartOrStop_Clicked();

	void On_pushButton_Recognize_Clicked();
	void On_pushButton_Drag_Clicked();

	void On_FinishOneXuewei(int);

	void On_FinishOneGroup();

private:
	void SetLabelFromValue(int value, QLabel* label);

	Ui::AutoTreatLegUnblockBeginClass ui;

	QSharedPointer<PauseWidget> m_pPauseWidget;
	QSharedPointer<AutoTreatStop> m_pStopWidget;

	QSharedPointer<RoundProgressBar> m_pProgressRightLeg1;
	QSharedPointer<RoundProgressBar> m_pProgressLeftLeg1;
	QSharedPointer<RoundProgressBar> m_pProgressRightLeg2;
	QSharedPointer<RoundProgressBar> m_pProgressLeftLeg2;

	MyWindow* m_pWindow;
	AutoTreatLegUnblock* m_pLegUnblock;

	int m_iRightLeg1Time;
	int m_iLeftLeg1Time;
	int m_iRightLeg2Time;
	int m_iLeftLeg2Time;

	int m_iTotalTime;

	bool m_bFirstStart;
};
