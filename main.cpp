#include "PhysicalTherapyRobot.h"
#include <QtWidgets/QApplication>

#include <QCoreApplication>
#include <QtDebug>
#include <QFile>
#include <QTextStream>


void outputMessage(QtMsgType t, const QMessageLogContext& context, const QString& msg)
{
    static QMutex mutex;
    mutex.lock();
    QString text;
    switch (int(t))
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;
    case QtWarningMsg:
        text = QString("Warning:");
        break;
    case QtCriticalMsg:
        text = QString("Critical:");
        break;
    case QtFatalMsg:
        text = QString("Fatal:");
    }
    QString context_info = QString("File:%1 Func:%2 Line:%3").arg(QString(context.file)).arg(context.function).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1 %2 %3 %4").arg(current_date).arg(text).arg(context_info).arg(msg);
    QString nowDate = QDateTime::currentDateTime().toString("yyyyMMdd");
    QFile file(qAppName() + "." + nowDate + ".log");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();
    mutex.unlock();
}

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

    qInstallMessageHandler(outputMessage);

	PhysicalTherapyRobot w;
	w.show();

	return a.exec();
}
