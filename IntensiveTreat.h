#pragma once

#include <QWidget>
#include "ui_IntensiveTreat.h"

#include "IntensiveTreatPalliativeCare.h"
#include "IntensiveTreatSetTime.h"
#include "IntensiveTreatAcupointRecog.h"
#include "IntensiveTreatToleranceTest.h"
#include "IntensiveTreatBegin.h"
#include "IntensiveTreatOnGoing.h"
#include "IntensiveTreatFinish.h"
#include "MyWindow.h"

class MyWindow;
class IntensiveTreat : public QWidget
{
	Q_OBJECT

public:
	IntensiveTreat(MyWindow* window,QWidget *parent = nullptr);
	~IntensiveTreat();

	void SetWidgetPalliativeCare();

	void SetWidgetSetTime();

	void SetWidgetAcupointRecog();

	void SetWidgetToleranceTest();

	void SetWidgetBegin();

	void SetWidgetOnGoing();

	void SetWidgetFinish();

	void FinishReturn();

    MyWindow* GetWindow();


private:
	Ui::IntensiveTreatClass ui;

	QSharedPointer<IntensiveTreatPalliativeCare> m_pIntensiveTreatPalliativeCare;
	QSharedPointer<IntensiveTreatSetTime> m_pIntensiveTreatSetTime;
	QSharedPointer<IntensiveTreatAcupointRecog> m_pIntensiveTreatAcupointRecog;
	QSharedPointer<IntensiveTreatToleranceTest> m_pIntensiveTreatToleranceTest;
	QSharedPointer<IntensiveTreatBegin> m_pIntensiveTreatBegin;
	QSharedPointer<IntensiveTreatOnGoing> m_pIntensiveTreatOnGoing;
	QSharedPointer<IntensiveTreatFinish> m_pIntensiveTreatFinish;

    MyWindow* m_pWindow;
};
