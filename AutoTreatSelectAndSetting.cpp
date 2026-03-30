#include "AutoTreatSelectAndSetting.h"

#include "AutoTreat.h"

AutoTreatSelectAndSetting::AutoTreatSelectAndSetting(AutoTreat* treat,QWidget *parent)
	: m_pAutoTreat(treat)
	, QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_OpenBack, SIGNAL(clicked()), this, SLOT(On_pushButton_OpenBack_Clicked()));
	connect(ui.pushButton_OpenBack, SIGNAL(clicked()), this, SLOT(On_pushButton_OpenBack_Clicked()));

	disconnect(ui.pushButton_OpenBackSetPara, SIGNAL(clicked()), this, SLOT(On_pushButton_OpenBackSetPara_Clicked()));
	connect(ui.pushButton_OpenBackSetPara, SIGNAL(clicked()), this, SLOT(On_pushButton_OpenBackSetPara_Clicked()));

	disconnect(ui.pushButton_LegOpen, SIGNAL(clicked()), this, SLOT(On_pushButton_LegOpen_Clicked()));
	connect(ui.pushButton_LegOpen, SIGNAL(clicked()), this, SLOT(On_pushButton_LegOpen_Clicked()));

	disconnect(ui.pushButton_LegOpenSetPara, SIGNAL(clicked()), this, SLOT(On_pushButton_LegOpenSetPara_Clicked()));
	connect(ui.pushButton_LegOpenSetPara, SIGNAL(clicked()), this, SLOT(On_pushButton_LegOpenSetPara_Clicked()));

	disconnect(ui.pushButton_LegUnlock, SIGNAL(clicked()), this, SLOT(On_pushButton_LegUnlock_Clicked()));
	connect(ui.pushButton_LegUnlock, SIGNAL(clicked()), this, SLOT(On_pushButton_LegUnlock_Clicked()));

	disconnect(ui.pushButton_LegUnlockSetPara, SIGNAL(clicked()), this, SLOT(On_pushButton_LegUnlockSetPara_Clicked()));
	connect(ui.pushButton_LegUnlockSetPara, SIGNAL(clicked()), this, SLOT(On_pushButton_LegUnlockSetPara_Clicked()));

	disconnect(ui.pushButton_ShoulderUnlock, SIGNAL(clicked()), this, SLOT(On_pushButton_ShoulderUnlock_Clicked()));
	connect(ui.pushButton_ShoulderUnlock, SIGNAL(clicked()), this, SLOT(On_pushButton_ShoulderUnlock_Clicked()));

	disconnect(ui.pushButton_ShoulderUnlockSetPara, SIGNAL(clicked()), this, SLOT(On_pushButton_ShoulderUnlockSetPara_Clicked()));
	connect(ui.pushButton_ShoulderUnlockSetPara, SIGNAL(clicked()), this, SLOT(On_pushButton_ShoulderUnlockSetPara_Clicked()));

	disconnect(ui.pushButton_BackToHome, SIGNAL(clicked()), this, SLOT(On_pushButton_BackToHome_Clicked()));
	connect(ui.pushButton_BackToHome, SIGNAL(clicked()), this, SLOT(On_pushButton_BackToHome_Clicked()));

	disconnect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(On_pushButton_LastStep_Clicked()));
	connect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(On_pushButton_LastStep_Clicked()));

	disconnect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(On_pushButton_Confirm_Clicked()));
	connect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(On_pushButton_Confirm_Clicked()));

	disconnect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(On_pushButton_NextStep_Clicked()));
	connect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(On_pushButton_NextStep_Clicked()));
}

AutoTreatSelectAndSetting::~AutoTreatSelectAndSetting()
{}


void AutoTreatSelectAndSetting::On_pushButton_OpenBack_Clicked()
{
	ui.pushButton_OpenBack->setDown(true);
	ui.pushButton_LegOpen->setDown(false);
	ui.pushButton_LegUnlock->setDown(false);
	ui.pushButton_ShoulderUnlock->setDown(false);

	ui.label_XueweiImage->setPixmap(QPixmap(":/OpenBack.png"));
}

void AutoTreatSelectAndSetting::On_pushButton_OpenBackSetPara_Clicked()
{
	m_pAutoTreat->SetWidgetOpenBack();
}

void AutoTreatSelectAndSetting::On_pushButton_LegOpen_Clicked()
{
	ui.pushButton_OpenBack->setDown(false);
	ui.pushButton_LegOpen->setDown(true);
	ui.pushButton_LegUnlock->setDown(false);
	ui.pushButton_ShoulderUnlock->setDown(false);

	ui.label_XueweiImage->setPixmap(QPixmap(":/LegOpen.png"));
}

void AutoTreatSelectAndSetting::On_pushButton_LegOpenSetPara_Clicked()
{
	m_pAutoTreat->SetWidgetLegOpen();
}

void AutoTreatSelectAndSetting::On_pushButton_LegUnlock_Clicked()
{
	ui.pushButton_OpenBack->setDown(false);
	ui.pushButton_LegOpen->setDown(false);
	ui.pushButton_LegUnlock->setDown(true);
	ui.pushButton_ShoulderUnlock->setDown(false);
	ui.label_XueweiImage->setPixmap(QPixmap(":/LegUnblock.png"));
}

void AutoTreatSelectAndSetting::On_pushButton_LegUnlockSetPara_Clicked()
{
	m_pAutoTreat->SetWidgetLegUnblock();
}

void AutoTreatSelectAndSetting::On_pushButton_ShoulderUnlock_Clicked()
{
	ui.pushButton_OpenBack->setDown(false);
	ui.pushButton_LegOpen->setDown(false);
	ui.pushButton_LegUnlock->setDown(false);
	ui.pushButton_ShoulderUnlock->setDown(true);

	ui.label_XueweiImage->setPixmap(QPixmap(":/ShoulderUnblock.png"));
}

void AutoTreatSelectAndSetting::On_pushButton_ShoulderUnlockSetPara_Clicked()
{
	m_pAutoTreat->SetWidgetShoulderUnblock();
}

void AutoTreatSelectAndSetting::On_pushButton_BackToHome_Clicked()
{}

void AutoTreatSelectAndSetting::On_pushButton_LastStep_Clicked()
{

}
void AutoTreatSelectAndSetting::On_pushButton_Confirm_Clicked()
{

}

void AutoTreatSelectAndSetting::On_pushButton_NextStep_Clicked()
{

}