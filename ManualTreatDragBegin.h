#pragma once

#include <QWidget>
#include "ui_ManualTreatDragBegin.h"

#include "PauseWidget.h"
#include "AutoTreatStop.h"

#include "qtimer.h"

class MyWindow;
class Communicate;

class ManualTreatDragBegin : public QWidget
{
	Q_OBJECT

public:
	ManualTreatDragBegin(MyWindow* window,QWidget *parent = nullptr);
	~ManualTreatDragBegin();

	void SetFirstFlag()
	{
		m_bFirstDrag = true;
	}

public slots:
	void On_pushButton_Back_Clicked();
	void On_pushButton_LastStep_Clicked();
	void On_pushButton_PauseOrContinue_Clicked();

	void On_pushButton_StartOrStop_Clicked();

	void On_pushButton_PosFixed_Clicked();
	void On_pushButton_DragStartOrStop_Clicked();

	void On_pushButton_LocationStartOrStop_Clicked();

	void On_pushButton_TimerDecr_Clicked();
	void On_pushButton_TimerIncr_Clicked();
	void On_pushButton_IntensityDecr_Clicked();
	void On_pushButton_IntensityIncr_Clicked();

	void On_pushButton_TreatStart_Clicked();

	void On_pushButton_TreatStop_Clicked();

	void On_Timer_Out();

	void ResumePara();
private:
	Ui::ManualTreatDragBeginClass ui;
	uint64_t m_iTotalTime;
	uint64_t m_iElapseTime;

	bool m_bFirstDrag;

	bool m_bStartFlag;
	bool m_bPosFixedPushed;
	bool m_bTreatStarted;

	MyWindow* m_pMyWindow;

	Communicate* m_pCommunicate;

	QSharedPointer<PauseWidget> m_pPauseWidget;
	QSharedPointer<AutoTreatStop> m_pStopWidget;

	QTimer* m_pTimer;
};
