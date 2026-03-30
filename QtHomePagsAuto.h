#pragma once

#include <QWidget>
#include "ui_QtHomePagsAuto.h"

class HomePage;
class MyWindow;

class QtHomePagsAuto : public QWidget
{
	Q_OBJECT

public:
	QtHomePagsAuto(MyWindow* home,QWidget *parent = nullptr);
	~QtHomePagsAuto();

public slots:
	void on_Pushbutton_Auto_clicked();

private:
	Ui::QtHomePagsAutoClass ui;

	MyWindow* m_pMyWindow;
	 //QSharedPointer < HomePage* m_pHome;
};
