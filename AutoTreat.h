#pragma once

#include <QWidget>
#include "ui_AutoTreat.h"

//#include "TreatInstruction.h"
#include "AutoTreatPrePrepare.h"
#include "AutoTreatSelectAndSetting.h"
#include "AutoTreatOpenBack.h"
#include "AutoTreatLegOpen.h"
#include "AutoTreatLegUnblock.h"
#include "AutoTreatShoulderUnblock.h"

#include "AutoTreatSetTime.h"
#include "AutoTreatAcupointRecog.h"
#include "AutoTreatToleranceTest.h"

#include "AutoTreatBegin.h"
#include "AutoTreatLegOpenBegin.h"
#include "AutoTreatLegUnblockBegin.h"
#include "AutoTreatShoulderUnblockBegin.h"

#include "AutoTreatOnGoing.h"
#include "AutoTreatFinish.h"

class MyWindow;
class AutoTreat : public QWidget
{
	Q_OBJECT

public:
    AutoTreat(MyWindow* window,QWidget *parent = nullptr);
	~AutoTreat();

	void SetWidgetInstruction();

	void SetWidgetPrePrepare();

	void SetWidgetSelectAndSetting();

	void SetWidgetOpenBack();

	void SetWidgetLegOpen();

	void SetWidgetLegUnblock();

	void SetWidgetShoulderUnblock();

	void SetWidgetSetTime();

	void SetWidgetAcupointRecog();

	void SetWidgetToleranceTest();

	void SetWidgetTreatBegin(OPENBACK_MODEL model,XUEWEI_TYPE type,int time);

	void SetWidgetTreatLegOpenBegin();
	void SetWidgetTreatLegUnblockBegin();
	void SetWidgetTreatShoulderUnblockBegin();

	void SetWidgetTreatOnGoing();

	QSharedPointer<AutoTreatOnGoing> GetWidgetTreatOnGoing();

    void SetWidgetTreatFinish();

	void SetCurrentProj(QString proj);

    MyWindow* GetWindow()
    {
        return m_pWindow;
    }

	void FinishReturn();

private:
	Ui::AutoTreatClass ui;

	QSharedPointer<AutoTreatPrePrepare> m_pAutoTreatPrePrepare;
	QSharedPointer<AutoTreatSelectAndSetting> m_pAutoTreatSelectAndSetting;
	QSharedPointer<AutoTreatOpenBack> m_pAutoTreatOpenBack;
	QSharedPointer<AutoTreatLegOpen> m_pAutoTreatLegOpen;
	QSharedPointer<AutoTreatLegUnblock> m_pAutoTreatLegUnblock;
	QSharedPointer<AutoTreatShoulderUnblock> m_pAutoTreatShoulderUnblock;


	QSharedPointer<AutoTreatSetTime> m_pAutoTreatSetTime;
	QSharedPointer<AutoTreatAcupointRecog> m_pAutoTreatAcupointRecog;
	QSharedPointer<AutoTreatToleranceTest> m_pAutoTreatToleranceTest;
	QSharedPointer<AutoTreatBegin> m_pAutoTreatBegin;
	QSharedPointer<AutoTreatLegOpenBegin> m_pAutoTreatLegOpenBegin;
	QSharedPointer<AutoTreatLegUnblockBegin> m_pAutoTreatLegUnblockBegin;
	QSharedPointer<AutoTreatShoulderUnblockBegin> m_pAutoTreatShoulderUnblockBegin;

	QSharedPointer<AutoTreatOnGoing> m_pAutoTreatOnGoing;
	QSharedPointer<AutoTreatFinish> m_pAutoTreatFinish;

    MyWindow* m_pWindow;
};
