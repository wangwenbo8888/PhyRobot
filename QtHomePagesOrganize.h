#pragma once

#include <QWidget>
#include "ui_QtHomePagesOrganize.h"

class HomePage;
class QtHomePagesOrganize : public QWidget
{
	Q_OBJECT

public:
	QtHomePagesOrganize(HomePage* home,QWidget *parent = nullptr);
	~QtHomePagesOrganize();

public slots:
	void on_Pushbutton_Plan_clicked();

private:
	Ui::QtHomePagesOrganizeClass ui;

	HomePage* m_pHome;
};
