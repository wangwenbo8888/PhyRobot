#pragma once

#include <QWidget>
#include "ui_AutoTreatPrePrepare.h"

enum STEP
{
	BLAM_TO_APPLY = 0,
	METAL_DETECT,
	OPER_DEBUG,
	WORK_TEST
};

class AutoTreat;
class AutoTreatPrePrepare : public QWidget
{
	Q_OBJECT

public:
	AutoTreatPrePrepare(AutoTreat* treat,QWidget *parent = nullptr);
	~AutoTreatPrePrepare();

	void SetFirstStep()
	{
		m_eOperation = BLAM_TO_APPLY;
		//ui.label_Hook->setPixmap(QPixmap(":/»Ò¹´.png"));
		ui.pushButton_NextStep->setEnabled(false);
	}

public slots:
	void on_Pushbutton_NextStep_Clicked();

	void on_Pushbutton_LastStep_Clicked();

	void on_PushButton_Confirm();

private:
	Ui::AutoTreatPrePrepareClass ui;

	STEP m_eOperation;

	AutoTreat* m_pAutoTreat;
};
