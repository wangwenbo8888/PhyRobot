
#include "PhysicalTherapyRobot.h"

#include <QMessageBox>
#include <QTime>
#include <QStringLiteral>
#include "MyDatabase.h"

#include "MyWindow.h"

PhysicalTherapyRobot::PhysicalTherapyRobot(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);

    InitUi();

    m_pWindow = new MyWindow(this);

    m_pForgetPassword = new ForgetPassword();

    disconnect(ui.pushButton_Login, SIGNAL(clicked()),this,SLOT(On_Pushbutton_Login_Clicked()));
    connect(ui.pushButton_Login,SIGNAL(clicked()),this,SLOT(On_Pushbutton_Login_Clicked()));

    disconnect(ui.pushButton_ForgetPassword, SIGNAL(clicked()), this, SLOT(On_Pushbutton_ForgetPassword_Clicked()));
    connect(ui.pushButton_ForgetPassword, SIGNAL(clicked()), this, SLOT(On_Pushbutton_ForgetPassword_Clicked()));
    
}

PhysicalTherapyRobot::~PhysicalTherapyRobot()
{
    if (m_pWindow!=NULL)
    {
        delete m_pWindow;
        m_pWindow = NULL;
    }

    //JointMovJ(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    //Wait_Done();
}

void PhysicalTherapyRobot::On_Pushbutton_ForgetPassword_Clicked()
{
    m_pForgetPassword->show();
}

void PhysicalTherapyRobot::On_Pushbutton_Login_Clicked()
{
    QString status = MyDatabase::getInstance().userLogin(ui.lineEdit_UserName->text(), ui.lineEdit_Password->text());
    if (status == "loginOk")//验证账号密码正确性
    {
        m_pWindow->show();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setStyleSheet("QMessageBox{background-color: rgba(188, 223, 255,50);\
                             border:1px solid #CCFFF6;\
                             border-radius:3px;}");
        msgBox.setText(QStringLiteral("用户名或密码错误!"));
        msgBox.exec();
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
    QString dateString = currentDate.toString(QStringLiteral("MM月dd日"));

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

    TIMEINFO solar;
    solar.iYear = currentDate.year();
    solar.iMonth = currentDate.month();
    solar.iDay = currentDate.day();

    QStringList numerical_value = { QStringLiteral("一"),QStringLiteral("二"),QStringLiteral("三"),QStringLiteral("四"),QStringLiteral("五"),QStringLiteral("六"),QStringLiteral("七"),QStringLiteral("八"),QStringLiteral("九"),QStringLiteral("十"),QStringLiteral("十一"),QStringLiteral("十二"),QStringLiteral("十三"),QStringLiteral("十四"),QStringLiteral("十五"),QStringLiteral("十六"),QStringLiteral("十七"),QStringLiteral("十八"),QStringLiteral("十九"),QStringLiteral("二十"),QStringLiteral("二十一"),QStringLiteral("二十二"),QStringLiteral("二十三"),QStringLiteral("二十四"),QStringLiteral("二十五"),QStringLiteral("二十六"),QStringLiteral("二十七"),QStringLiteral("二十八"),QStringLiteral("二十九"),QStringLiteral("三十")};
    TIMEINFO lunar;
    char* error = new char[1024];
    if (toLunarDate(solar, lunar, error))
    {
        QString Lunar = numerical_value[lunar.iMonth-1];
        Lunar.append(QStringLiteral("月"));
        if (lunar.iDay<11)
        {
            Lunar.append(QStringLiteral("初"));
            Lunar.append(numerical_value[lunar.iDay-1]);
        }
        else
        {
            Lunar.append(numerical_value[lunar.iDay-1]);
        }

        //text.append(" ");
        //text.append(Lunar);
    }

    delete[] error;
    ui.label_Date->setText(QObject::tr(text.toStdString().c_str()));
}

bool toLunarDate(TIMEINFO solar, TIMEINFO& lunar, char* error)
{
    unsigned int lunar200y[199] = {
        0x04AE53,0x0A5748,0x5526BD,0x0D2650,0x0D9544,0x46AAB9,0x056A4D,0x09AD42,0x24AEB6,0x04AE4A,      /*1901-1910*/
        0x6A4DBE,0x0A4D52,0x0D2546,0x5D52BA,0x0B544E,0x0D6A43,0x296D37,0x095B4B,0x749BC1,0x049754,      /*1911-1920*/
        0x0A4B48,0x5B25BC,0x06A550,0x06D445,0x4ADAB8,0x02B64D,0x095742,0x2497B7,0x04974A,0x664B3E,       /*1921-1930*/
        0x0D4A51,0x0EA546,0x56D4BA,0x05AD4E,0x02B644,0x393738,0x092E4B,0x7C96BF,0x0C9553,0x0D4A48,       /*1931-1940*/
        0x6DA53B,0x0B554F,0x056A45,0x4AADB9,0x025D4D,0x092D42,0x2C95B6,0x0A954A,0x7B4ABD,0x06CA51,    /*1941-1950*/
        0x0B5546,0x555ABB,0x04DA4E,0x0A5B43,0x352BB8,0x052B4C,0x8A953F,0x0E9552,0x06AA48,0x6AD53C,       /*1951-1960*/
        0x0AB54F,0x04B645,0x4A5739,0x0A574D,0x052642,0x3E9335,0x0D9549,0x75AABE,0x056A51,0x096D46,        /*1961-1970*/
        0x54AEBB,0x04AD4F,0x0A4D43,0x4D26B7,0x0D254B,0x8D52BF,0x0B5452,0x0B6A47,0x696D3C,0x095B50,     /*1971-1980*/
        0x049B45,0x4A4BB9,0x0A4B4D,0xAB25C2,0x06A554,0x06D449,0x6ADA3D,0x0AB651,0x093746,0x5497BB,     /*1981-1990*/
        0x04974F,0x064B44,0x36A537,0x0EA54A,0x86B2BF,0x05AC53,0x0AB647,0x5936BC,0x092E50,0x0C9645,         /*1991-2000*/
        0x4D4AB8,0x0D4A4C,0x0DA541,0x25AAB6,0x056A49,0x7AADBD,0x025D52,0x092D47,0x5C95BA,0x0A954E,   /*2001-2010*/
        0x0B4A43,0x4B5537,0x0AD54A,0x955ABF,0x04BA53,0x0A5B48,0x652BBC,0x052B50,0x0A9345,0x474AB9,      /*2011-2020*/
        0x06AA4C,0x0AD541,0x24DAB6,0x04B64A,0x69573D,0x0A4E51,0x0D2646,0x5E933A,0x0D534D,0x05AA43,    /*2021-2030*/
        0x36B537,0x096D4B,0xB4AEBF,0x04AD53,0x0A4D48,0x6D25BC,0x0D254F,0x0D5244,0x5DAA38,0x0B5A4C,   /*2031-2040*/
        0x056D41,0x24ADB6,0x049B4A,0x7A4BBE,0x0A4B51,0x0AA546,0x5B52BA,0x06D24E,0x0ADA42,0x355B37,   /*2041-2050*/
        0x09374B,0x8497C1,0x049753,0x064B48,0x66A53C,0x0EA54F,0x06B244,0x4AB638,0x0AAE4C,0x092E42,        /*2051-2060*/
        0x3C9735,0x0C9649,0x7D4ABD,0x0D4A51,0x0DA545,0x55AABA,0x056A4E,0x0A6D43,0x452EB7,0x052D4B,   /*2061-2070*/
        0x8A95BF,0x0A9553,0x0B4A47,0x6B553B,0x0AD54F,0x055A45,0x4A5D38,0x0A5B4C,0x052B42,0x3A93B6,     /*2071-2080*/
        0x069349,0x7729BD,0x06AA51,0x0AD546,0x54DABA,0x04B64E,0x0A5743,0x452738,0x0D264A,0x8E933E,     /*2081-2090*/
        0x0D5252,0x0DAA47,0x66B53B,0x056D4F,0x04AE45,0x4A4EB9,0x0A4D4C,0x0D1541,0x2D92B5                          /*2091-2099*/
    };

    int monthTotal[13] = { 0,31,59,90,120,151,181,212,243,273,304,334,365 };

    int year = solar.iYear;
    int month = solar.iMonth;
    int day = solar.iDay;

    if (year > 2099)
    {
        char ch[] = "Years only support from 1901 to 2099";
        memcpy(error, ch, 1024);
        return false;
    }

    if (month >= 13)
    {
        char ch[] = "Months only support from 1 to 12";
        memcpy(error, ch, 1024);
        return false;
    }

    int bySpring, bySolar, daysPerMonth;
    int index, flag;


    if (((lunar200y[year - 1901] & 0x0060) >> 5) == 1)
        bySpring = (lunar200y[year - 1901] & 0x001F) - 1;
    else
        bySpring = (lunar200y[year - 1901] & 0x001F) - 1 + 31;
    bySolar = monthTotal[month - 1] + day - 1;
    if ((!(year % 4)) && (month > 2))
        bySolar++;


    if (bySolar >= bySpring) {
        bySolar -= bySpring;
        month = 1;
        index = 1;
        flag = 0;
        if ((lunar200y[year - 1901] & (0x80000 >> (index - 1))) == 0)
            daysPerMonth = 29;
        else
            daysPerMonth = 30;
        while (bySolar >= daysPerMonth) {
            bySolar -= daysPerMonth;
            index++;
            if (month == ((lunar200y[year - 1901] & 0xF00000) >> 20)) {
                flag = ~flag;
                if (flag == 0)
                    month++;
            }
            else
                month++;
            if ((lunar200y[year - 1901] & (0x80000 >> (index - 1))) == 0)
                daysPerMonth = 29;
            else
                daysPerMonth = 30;
        }
        day = bySolar + 1;
    }
    else {
        bySpring -= bySolar;
        year--;
        month = 12;
        if (((lunar200y[year - 1901] & 0xF00000) >> 20) == 0)
            index = 12;
        else
            index = 13;
        flag = 0;
        if ((lunar200y[year - 1901] & (0x80000 >> (index - 1))) == 0)
            daysPerMonth = 29;
        else
            daysPerMonth = 30;
        while (bySpring > daysPerMonth) {
            bySpring -= daysPerMonth;
            index--;
            if (flag == 0)
                month--;
            if (month == ((lunar200y[year - 1901] & 0xF00000) >> 20))
                flag = ~flag;
            if ((lunar200y[year - 1901] & (0x80000 >> (index - 1))) == 0)
                daysPerMonth = 29;
            else
                daysPerMonth = 30;
        }

        day = daysPerMonth - bySpring + 1;
    }

    lunar.iYear = year;
    lunar.iMonth = month;
    lunar.iDay = day;
    return true;
}
