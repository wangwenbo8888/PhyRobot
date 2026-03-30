#pragma once

#include <QWidget>
#include "ui_QtHomePagesOrganizeInstruct.h"

class QtHomePagesOrganizeInstruct : public QWidget
{
	Q_OBJECT

public:
	QtHomePagesOrganizeInstruct(QWidget *parent = nullptr);
	~QtHomePagesOrganizeInstruct();

private:
	Ui::QtHomePagesOrganizeInstructClass ui;
};
