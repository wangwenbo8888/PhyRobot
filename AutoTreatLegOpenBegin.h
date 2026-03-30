#pragma once

#include <QWidget>
#include "ui_AutoTreatLegOpenBegin.h"

#include "QtRoundProgressBar.h"

#include "PauseWidget.h"
#include "AutoTreatStop.h"

class MyWindow;
class AutoTreatLegOpen;

class AutoTreatLegOpenBegin : public QWidget
{
	Q_OBJECT

public:
	AutoTreatLegOpenBegin(MyWindow* window,QWidget *parent = nullptr);
	~AutoTreatLegOpenBegin();

	void SetLegOpen(AutoTreatLegOpen* legopen);

	void SetTreatTime(int minute);

public slots:

	void On_pushButton_Back_Clicked();
	void On_pushButton_LastStep_Clicked();
	void On_pushButton_PauseOrContinue_Clicked();
	void On_pushButton_StartOrStop_Clicked();

	void On_pushButton_Recognize_Clicked();
	void On_pushButton_Drag_Clicked();

	void On_FinishOneGroup();

private:
	Ui::AutoTreatLegOpenBeginClass ui;

	QSharedPointer<PauseWidget> m_pPauseWidget;
	QSharedPointer<AutoTreatStop> m_pStopWidget;

	QSharedPointer<RoundProgressBar> m_pRoundProgress;

	MyWindow* m_pWindow;
	AutoTreatLegOpen* m_pLegOpen;

	bool m_bFirstStart;
};
