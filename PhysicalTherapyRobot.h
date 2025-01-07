#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PhysicalTherapyRobot.h"

#include "MyWindow.h"

class PhysicalTherapyRobot : public QMainWindow
{
    Q_OBJECT

public:
    PhysicalTherapyRobot(QWidget *parent = nullptr);
    ~PhysicalTherapyRobot();

public slots:

    void On_Pushbutton_Login_Clicked();
     
    
private:
    Ui::PhysicalTherapyRobotClass ui;

    void InitUi();

    MyWindow* m_pWindow;
};
