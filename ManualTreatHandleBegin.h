#pragma once

#include <QWidget>
#include "ui_ManualTreatHandleBegin.h"

#include "PauseWidget.h"
#include "AutoTreatStop.h"

class MyWindow;
class Communicate;

class ManualTreatHandleBegin : public QWidget
{
	Q_OBJECT

public:
	ManualTreatHandleBegin(MyWindow* window,QWidget *parent = nullptr);
	~ManualTreatHandleBegin();

public slots:
	void On_pushButton_Back_Clicked();
	void On_pushButton_LastStep_Clicked();
	void On_pushButton_PauseOrContinue_Clicked();

	void On_pushButton_StartOrStop_Clicked();

	void On_pushButton_TimerDecr_Clicked();
	void On_pushButton_TimerIncr_Clicked();
	void On_pushButton_IntensityDecr_Clicked();
	void On_pushButton_IntensityIncr_Clicked();

	void ResumePara();

	void On_TreatStop_Confirmed();

private:
	Ui::ManualTreatHandleBeginClass ui;

	QSharedPointer<PauseWidget> m_pPauseWidget;
	QSharedPointer<AutoTreatStop> m_pStopWidget;

	MyWindow* m_pWindow;
	Communicate* m_pCommunicate;
	bool m_bStarted;
};
