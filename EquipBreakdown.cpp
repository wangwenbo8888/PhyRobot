#include "EquipBreakdown.h"

#include <QTime>
#include <QStringLiteral>

EquipBreakdown::EquipBreakdown(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

    InitUi();

	m_pTimer = new QTimer(this);

	m_pTimer->setInterval(1000);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(On_timeout()));
	m_pTimer->start();
}

EquipBreakdown::~EquipBreakdown()
{
}

void EquipBreakdown::On_timeout()
{
	QTime currentTime = QTime::currentTime();
	QString timeString = currentTime.toString("hh:mm");

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

	text.append(" ");
	text.append(timeString);
	ui.label_Date->setText(QObject::tr(text.toStdString().c_str()));
}

void EquipBreakdown::InitUi()
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
