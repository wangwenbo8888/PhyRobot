#pragma once

#include <QWidget>
#include "ui_AutoTreat.h"

#include "AutoTreatInstruction.h"
#include "AutoTreatPrePrepare.h"
#include "AutoTreatSetTime.h"
#include "AutoTreatAcupointRecog.h"
#include "AutoTreatToleranceTest.h"
#include "AutoTreatBegin.h"
#include "AutoTreatOnGoing.h"
#include "AutoTreatFinish.h"

class AutoTreat : public QWidget
{
	Q_OBJECT

public:
	AutoTreat(QWidget *parent = nullptr);
	~AutoTreat();

	void SetWidgetInstruction();

	void SetWidgetPrePrepare();

	void SetWidgetSetTime();

	void SetWidgetAcupointRecog();

	void SetWidgetToleranceTest();

	void SetWidgetTreatBegin();

	void SetWidgetTreatOnGoing();

	void SetWidgetTreatFinish();

private:
	Ui::AutoTreatClass ui;

	QSharedPointer<AutoTreatInstruction> m_pAutoTreatInstruction;
	QSharedPointer<AutoTreatPrePrepare> m_pAutoTreatPrePrepare;
	QSharedPointer<AutoTreatSetTime> m_pAutoTreatSetTime;
	QSharedPointer<AutoTreatAcupointRecog> m_pAutoTreatAcupointRecog;
	QSharedPointer<AutoTreatToleranceTest> m_pAutoTreatToleranceTest;
	QSharedPointer<AutoTreatBegin> m_pAutoTreatBegin;
	QSharedPointer<AutoTreatOnGoing> m_pAutoTreatOnGoing;
	QSharedPointer<AutoTreatFinish> m_pAutoTreatFinish;
};
