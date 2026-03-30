#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PhysicalTherapyRobot.h"

#include "MyWindow.h"
#include "RobotComm.h"
#include "ForgetPassword.h"

bool toLunarDate(TIMEINFO solar, TIMEINFO& lunar, char* error);

class PhysicalTherapyRobot : public QMainWindow
{
    Q_OBJECT

public:
    PhysicalTherapyRobot(QWidget *parent = nullptr);
    ~PhysicalTherapyRobot();

public slots:

    void On_Pushbutton_Login_Clicked();

    void On_Pushbutton_ForgetPassword_Clicked();
     
private:
    Ui::PhysicalTherapyRobotClass ui;

    void InitUi();

    MyWindow* m_pWindow;
    ForgetPassword* m_pForgetPassword;
};
