#pragma once

#include <QWidget>
#include "ui_AutoTreatPrePrepare.h"

enum STEP
{
	PREPARE_ONE = 0,
	PREPARE_TWO = 1,
	PREPARE_THREE= 2,
	PREPARE_FOUR = 3,
	PREPARE_FIVE = 4,
	PREPARE_SIX = 5,
	PREPARE_SEVEN = 6,
	PREPARE_EIGHT = 7,
	PREPARE_NINE = 8,
	PREPARE_TEN = 9,
	PREPARE_ELEVEN = 10,
	PREPARE_TWELVE = 11,
	PREPARE_THIRTEEN = 12,
	PREPARE_FOURTEEN = 13,
	PREPARE_FIFTEEN = 14,
	PREPARE_UNKNOWN
};

class AutoTreat;
class AutoTreatPrePrepare : public QWidget
{
	Q_OBJECT

public:
	AutoTreatPrePrepare(AutoTreat* treat,QWidget *parent = nullptr);
	~AutoTreatPrePrepare();

	void ResumeOption()
	{
		m_eOperation = PREPARE_ONE;
	}

	void SetFirstStep()
	{
		m_eOperation = PREPARE_ONE;
		//ui.label_Hook->setPixmap(QPixmap(":/»Ò¹´.png"));
		// ui.pushButton_NextStep->setEnabled(false);
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
