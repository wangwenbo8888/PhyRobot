#pragma once

#include <QWidget>
#include "ui_MyWindow.h"

#include <QTimer>
#include <QSharedPointer>

#include "HomePage.h"
#include "AutoTreat.h"
#include "IntensiveTreat.h"
#include "FinishOrganize.h"
#include "AccoutManager.h"
#include "EquipInfo.h"
#include "SetUp.h"

class PhysicalTherapyRobot;
class MyWindow : public QWidget
{
	Q_OBJECT

public:
	MyWindow(PhysicalTherapyRobot* robot,QWidget *parent = nullptr);
	~MyWindow();

public slots:
	void On_PushButton_Exit_Clicked();

	void On_pushButton_MainFrame_Clicked();

	void On_pushButton_AutoTreat_Clicked();

	void On_pushButton_IntensiveTreat_Clicked();

	void On_pushButton_FinishOrganize_Clicked();

	void On_pushButton_AccountManage_Clicked();

	void On_pushButton_EquipInfo_Clicked();

	void On_pushButton_SetUp_Clicked();

	void On_timeout();

private:

private:
	Ui::MyWindowClass ui;

	PhysicalTherapyRobot* m_pRobot;

	QTimer* m_pTimer;

	QSharedPointer<HomePage> m_pHomePage;
	QSharedPointer<AutoTreat> m_pAutoTreat;
	QSharedPointer<IntensiveTreat> m_pIntensiveTreat;
	QSharedPointer<FinishOrganize> m_pFinishOrganize;
	QSharedPointer<AccoutManager> m_pAccoutManager;
	QSharedPointer<EquipInfo> m_pEquipInfo;
	QSharedPointer<SetUp> m_pSetUp;

	
};
