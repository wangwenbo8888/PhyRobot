#include "PhysicalTherapyRobot.h"

#include <QTime>
#include <QStringLiteral>

PhysicalTherapyRobot::PhysicalTherapyRobot(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);

    InitUi();

    m_pWindow = new MyWindow(this);

    disconnect(ui.pushButton_Login, SIGNAL(clicked()),this,SLOT(On_Pushbutton_Login_Clicked()));
    connect(ui.pushButton_Login,SIGNAL(clicked()),this,SLOT(On_Pushbutton_Login_Clicked()));
}

PhysicalTherapyRobot::~PhysicalTherapyRobot()
{
    if (m_pWindow!=NULL)
    {
        delete m_pWindow;
        m_pWindow = NULL;
    }
}

void PhysicalTherapyRobot::On_Pushbutton_Login_Clicked()
{
    if (m_pWindow)
    {
        m_pWindow->show();
    }
}

void PhysicalTherapyRobot::InitUi()
{
    QTime currentTime = QTime::currentTime();
    QString timeString = currentTime.toString("hh:mm");
    ui.label_Time->setText(timeString);

    // 获取当前日期
    QDate currentDate = QDate::currentDate();

    // 格式化输出日期
    QString dateString = currentDate.toString(QStringLiteral("yyyy年MM月dd日"));

    // 格式化输出星期
    //QString weekDayString = currentDate.toString("dddd");  // 返回英文星期
    //qDebug() << "今天是: " << weekDayString;

    // 使用自定义的星期表示
    const QString daysOfWeek[] = { "", QStringLiteral("星期一"), 
        QStringLiteral("星期二"), QStringLiteral("星期三"), QStringLiteral("星期四"),
        QStringLiteral("星期五"), QStringLiteral("星期六"), QStringLiteral("星期日") };
    int dayOfWeek = currentDate.dayOfWeek();
    QString text = dateString;
    text.append(" ");
    text.append(daysOfWeek[dayOfWeek]);
    ui.label_Date->setText(QObject::tr(text.toStdString().c_str()));
}
