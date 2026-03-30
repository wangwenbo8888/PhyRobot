#pragma once

#include <QWidget>
#include "ui_AutoTreatSelectAndSetting.h"

class AutoTreat;

// 自动治疗选择设置
class AutoTreatSelectAndSetting : public QWidget
{
	Q_OBJECT

public:
	AutoTreatSelectAndSetting(AutoTreat* treat,QWidget *parent = nullptr);
	~AutoTreatSelectAndSetting();

public slots:
	void On_pushButton_OpenBack_Clicked();
	void On_pushButton_OpenBackSetPara_Clicked();

	void On_pushButton_LegOpen_Clicked();
	void On_pushButton_LegOpenSetPara_Clicked();

	void On_pushButton_LegUnlock_Clicked();
	void On_pushButton_LegUnlockSetPara_Clicked();

	void On_pushButton_ShoulderUnlock_Clicked();
	void On_pushButton_ShoulderUnlockSetPara_Clicked();

	void On_pushButton_BackToHome_Clicked();
	void On_pushButton_LastStep_Clicked();
	void On_pushButton_Confirm_Clicked();
	void On_pushButton_NextStep_Clicked();

private:
	Ui::AutoTreatSelectAndSettingClass ui;

	AutoTreat* m_pAutoTreat;
};
