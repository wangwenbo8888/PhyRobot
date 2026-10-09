#include "PhysicalTherapyRobot.h"
#include <QtWidgets/QApplication>

#include <QCoreApplication>
#include <QtDebug>
#include <QFile>
#include <QTextStream>


void outputMessage(QtMsgType t, const QMessageLogContext& context, const QString& msg)
{
    static QMutex mutex;
	static QFile g_logFile;
	static bool g_logFileOk = false;
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
    // 日志文件句柄保持常开,避免每条消息 open/close(高频 qDebug 时的 IO 开销)
    if (!g_logFileOk)
    {
        QString nowDate = QDateTime::currentDateTime().toString("yyyyMMdd");
        g_logFile.setFileName(qAppName() + "." + nowDate + ".log");
        g_logFileOk = g_logFile.open(QIODevice::WriteOnly | QIODevice::Append);
    }
    if (g_logFileOk)
    {
        QTextStream text_stream(&g_logFile);
        text_stream << message << "\r\n";
        text_stream.flush();
    }
    mutex.unlock();
}

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

    qInstallMessageHandler(outputMessage);

    // 全局样式 style.qss: 与各 .ui 里控件内联样式互补(内联样式优先级更高)
    QFile qss(":/style.qss");
    if (qss.open(QFile::ReadOnly))
    {
        a.setStyleSheet(QString::fromUtf8(qss.readAll()));
    }

	PhysicalTherapyRobot w;
	w.show();

	return a.exec();
}
