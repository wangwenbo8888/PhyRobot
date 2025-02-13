#pragma once

#include <QWidget>
#include "ui_FinishOrganize.h"

enum ORGANIZE_STEP
{
	CLEAR = 0,
	SHUTDOWN,
	HOMING,
};

class MyWindow;
class FinishOrganize : public QWidget
{
	Q_OBJECT

public:
	FinishOrganize(MyWindow* window,QWidget *parent = nullptr);
	~FinishOrganize();

	void InitState();

public slots:
	void on_PushButton_NextStep_Clicked();

	void on_PushButton_Confirm();

	void on_PushButton_FinishBack();

private:
	Ui::FinishOrganizeClass ui;

	ORGANIZE_STEP m_eOperation;

	MyWindow* m_pWindow;
};
