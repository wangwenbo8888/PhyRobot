#pragma once

#include <QWidget>
#include "ui_EquipBreakdown.h"

#include <QTimer>

class EquipBreakdown : public QWidget
{
	Q_OBJECT

public:
	EquipBreakdown(QWidget *parent = nullptr);
	~EquipBreakdown();

private:
	void InitUi();

public slots:
	void On_timeout();

private:
	Ui::EquipBreakdownClass ui;

	QTimer* m_pTimer;
};
