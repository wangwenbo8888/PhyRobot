#pragma once

#include <QWidget>
#include "ui_AutoTreatFinish.h"

class AutoTreat;
class AutoTreatFinish : public QWidget
{
	Q_OBJECT

public:
	AutoTreatFinish(AutoTreat* treat,QWidget *parent = nullptr);
	~AutoTreatFinish();

	void SetCurrentProj(QString str);

public slots:
	void On_PushButton_BackToHome();

private:
	Ui::AutoTreatFinishClass ui;

	AutoTreat* m_pAutoTreat;
};
