#pragma once

#include <QWidget>
#include "ui_QtHomePagesManual.h"

class HomePage;
class MyWindow;

class QtHomePagesManual : public QWidget
{
	Q_OBJECT

public:
	QtHomePagesManual(MyWindow* window,QWidget *parent = nullptr);
	~QtHomePagesManual();

public slots:
	void on_Pushbutton_Manual_clicked();

private:
	Ui::QtHomePagesManualClass ui;

	MyWindow* m_pMyWindow;
	HomePage* m_pHome;
};
