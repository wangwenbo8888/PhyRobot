#pragma once

#include <QWidget>
#include "ui_HomePage.h"

#include "QtHomePagsAuto.h"
#include "QtHomePagesManual.h"
#include "QtHomePagesOrganize.h"
#include "SetUp.h"

class MyWindow;

class HomePage : public QWidget
{
	Q_OBJECT

public:
	HomePage(MyWindow* window,QWidget *parent = nullptr);
	~HomePage();

	void RefreshButtonState();

public slots:
	void On_pushButton_Auto_Clicked();

	void On_pushButton_Manual_Clicked();

	void On_pushButton_Plan_Clicked();

	void On_pushButton_Set_Clicked();

private:
	Ui::HomePageClass ui;
	MyWindow* m_pMyWindow;

	QSharedPointer<QtHomePagsAuto> m_pAuto;
	QSharedPointer<QtHomePagesManual> m_pManual;
	QSharedPointer<QtHomePagesOrganize> m_pOrganize;
	QSharedPointer<SetUp> m_pSetUp;
};
