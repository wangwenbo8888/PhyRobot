#include "QtWidgetsCommunicateTest.h"

#include "MyWindow.h"

QtWidgetsCommunicateTest::QtWidgetsCommunicateTest(MyWindow* window, QWidget *parent)
	: m_pWindow(window),
	QWidget(parent)
{
	ui.setupUi(this);

	disconnect(ui.pushButton_OpenBack_Start, SIGNAL(clicked()), this, SLOT(On_pushButton_OpenBack_Start_Clicked()));
	connect(ui.pushButton_OpenBack_Start, SIGNAL(clicked()), this, SLOT(On_pushButton_OpenBack_Start_Clicked()));

	disconnect(ui.pushButton_OpenBack_Stop, SIGNAL(clicked()), this, SLOT(On_pushButton_OpenBack_Stop_Clicked()));
	connect(ui.pushButton_OpenBack_Stop, SIGNAL(clicked()), this, SLOT(On_pushButton_OpenBack_Stop_Clicked()));

	disconnect(ui.pushButton_LegOpen_Start, SIGNAL(clicked()), this, SLOT(On_pushButton_LegOpen_Start_Clicked()));
	connect(ui.pushButton_LegOpen_Start, SIGNAL(clicked()), this, SLOT(On_pushButton_LegOpen_Start_Clicked()));

	disconnect(ui.pushButton_LegOpen_Stop, SIGNAL(clicked()), this, SLOT(On_pushButton_LegOpen_Stop_Clicked()));
	connect(ui.pushButton_LegOpen_Stop, SIGNAL(clicked()), this, SLOT(On_pushButton_LegOpen_Stop_Clicked()));

	disconnect(ui.pushButton_LegUnblock_Right1_Start, SIGNAL(clicked()), this, SLOT(On_pushButton_LegUnblock_Right1_Start_Clicked()));
	connect(ui.pushButton_LegUnblock_Right1_Start, SIGNAL(clicked()), this, SLOT(On_pushButton_LegUnblock_Right1_Start_Clicked()));

	disconnect(ui.pushButton_LegUnblock_Right1_Stop, SIGNAL(clicked()), this, SLOT(On_pushButton_LegUnblock_Right1_Stop_Clicked()));
	connect(ui.pushButton_LegUnblock_Right1_Stop, SIGNAL(clicked()), this, SLOT(On_pushButton_LegUnblock_Right1_Stop_Clicked()));

	disconnect(ui.pushButton_LegUnblock_Left1_Start, SIGNAL(clicked()), this, SLOT(On_pushButton_LegUnblock_Left1_Start_Clicked()));
	connect(ui.pushButton_LegUnblock_Left1_Start, SIGNAL(clicked()), this, SLOT(On_pushButton_LegUnblock_Left1_Start_Clicked()));

	disconnect(ui.pushButton_LegUnblock_Left1_Stop, SIGNAL(clicked()), this, SLOT(On_pushButton_LegUnblock_Left1_Stop_Clicked()));
	connect(ui.pushButton_LegUnblock_Left1_Stop, SIGNAL(clicked()), this, SLOT(On_pushButton_LegUnblock_Left1_Stop_Clicked()));

	disconnect(ui.pushButton_LegUnblock_Right2_Start, SIGNAL(clicked()), this, SLOT(On_pushButton_LegUnblock_Right2_Start_Clicked()));
	connect(ui.pushButton_LegUnblock_Right2_Start, SIGNAL(clicked()), this, SLOT(On_pushButton_LegUnblock_Right2_Start_Clicked()));

	disconnect(ui.pushButton_LegUnblock_Right2_Stop, SIGNAL(clicked()), this, SLOT(On_pushButton_LegUnblock_Right2_Stop_Clicked()));
	connect(ui.pushButton_LegUnblock_Right2_Stop, SIGNAL(clicked()), this, SLOT(On_pushButton_LegUnblock_Right2_Stop_Clicked()));

	disconnect(ui.pushButton_LegUnblock_Left2_Start, SIGNAL(clicked()), this, SLOT(On_pushButton_LegUnblock_Left2_Start_Clicked()));
	connect(ui.pushButton_LegUnblock_Left2_Start, SIGNAL(clicked()), this, SLOT(On_pushButton_LegUnblock_Left2_Start_Clicked()));

	disconnect(ui.pushButton_LegUnblock_Left2_Stop, SIGNAL(clicked()), this, SLOT(On_pushButton_LegUnblock_Left2_Stop_Clicked()));
	connect(ui.pushButton_LegUnblock_Left2_Stop, SIGNAL(clicked()), this, SLOT(On_pushButton_LegUnblock_Left2_Stop_Clicked()));

	disconnect(ui.pushButton_ShoulderUnblock_Start, SIGNAL(clicked()), this, SLOT(On_pushButton_ShoulderUnblock_Start_Clicked()));
	connect(ui.pushButton_ShoulderUnblock_Start, SIGNAL(clicked()), this, SLOT(On_pushButton_ShoulderUnblock_Start_Clicked()));

	disconnect(ui.pushButton_ShoulderUnblock_Stop, SIGNAL(clicked()), this, SLOT(On_pushButton_ShoulderUnblock_Stop_Clicked()));
	connect(ui.pushButton_ShoulderUnblock_Stop, SIGNAL(clicked()), this, SLOT(On_pushButton_ShoulderUnblock_Stop_Clicked()));

	disconnect(ui.pushButton_Handle_Start, SIGNAL(clicked()), this, SLOT(On_pushButton_Handle_Start_Clicked()));
	connect(ui.pushButton_Handle_Start, SIGNAL(clicked()), this, SLOT(On_pushButton_Handle_Start_Clicked()));

	disconnect(ui.pushButton_Handle_Stop, SIGNAL(clicked()), this, SLOT(On_pushButton_Handle_Stop_Clicked()));
	connect(ui.pushButton_Handle_Stop, SIGNAL(clicked()), this, SLOT(On_pushButton_Handle_Stop_Clicked()));
	
	disconnect(ui.pushButton_IncrIntensity, SIGNAL(clicked()), this, SLOT(On_pushButton_IncrIntensity_Clicked()));
	connect(ui.pushButton_IncrIntensity, SIGNAL(clicked()), this, SLOT(On_pushButton_IncrIntensity_Clicked()));

	disconnect(ui.pushButton_DecrIntensity, SIGNAL(clicked()), this, SLOT(On_pushButton_DecrIntensity_Clicked()));
	connect(ui.pushButton_DecrIntensity, SIGNAL(clicked()), this, SLOT(On_pushButton_DecrIntensity_Clicked()));

	intensity = 40;
}

QtWidgetsCommunicateTest::~QtWidgetsCommunicateTest()
{
}

void QtWidgetsCommunicateTest::SetCommunicate(Communicate* comm)
{
	m_pCommunicate = comm;

	disconnect(m_pCommunicate, SIGNAL(setMessage(QString)), this, SLOT(on_ShowMessage(QString)));
	connect(m_pCommunicate, SIGNAL(setMessage(QString)), this, SLOT(on_ShowMessage(QString)));
}

void QtWidgetsCommunicateTest::on_ShowMessage(QString message)
{
	ui.textEdit_Info->append("Receive message :");
	ui.textEdit_Info->append(message);
}

void QtWidgetsCommunicateTest::On_pushButton_OpenBack_Start_Clicked()
{
	ui.textEdit_Info->append("Enter open back start !");
	int begin = ui.spinBox_StartIntensity->value();
	ui.textEdit_Info->append(QString("begin intensity is %1").arg(begin));
	int run = ui.spinBox_RunIntensity->value();
	//ui.textEdit_Info->append(QString("run intensity is %1").arg(run));

	QByteArray data;
	m_pCommunicate->setData(OPENBACK,begin,run,true,data);
	ui.textEdit_Info->append("Send data :");
	ui.textEdit_Info->append(data.toHex().toUpper());
	m_pCommunicate->sendData(data);
}

void QtWidgetsCommunicateTest::On_pushButton_OpenBack_Stop_Clicked()
{
	ui.textEdit_Info->append("Enter open back stop !");
	int begin = ui.spinBox_StartIntensity->value();
	ui.textEdit_Info->append(QString("begin intensity is %1").arg(begin));
	int run = ui.spinBox_RunIntensity->value();
	ui.textEdit_Info->append(QString("run intensity is %1").arg(run));

	QByteArray data;
	m_pCommunicate->setData(OPENBACK, begin, run, false, data);
	ui.textEdit_Info->append("Send data :");
	ui.textEdit_Info->append(data.toHex().toUpper());
	m_pCommunicate->sendData(data);
}

void QtWidgetsCommunicateTest::On_pushButton_LegOpen_Start_Clicked()
{
	ui.textEdit_Info->append("Enter leg open start !");
	int begin = ui.spinBox_StartIntensity->value();
	ui.textEdit_Info->append(QString("begin intensity is %1").arg(begin));
	int run = ui.spinBox_RunIntensity->value();
	ui.textEdit_Info->append(QString("run intensity is %1").arg(run));

	QByteArray data;
	m_pCommunicate->setData(OPENLEG, begin, run, true, data);
	ui.textEdit_Info->append("Send data :");
	ui.textEdit_Info->append(data.toHex().toUpper());
	m_pCommunicate->sendData(data);
}

void QtWidgetsCommunicateTest::On_pushButton_LegOpen_Stop_Clicked()
{
	ui.textEdit_Info->append("Enter leg open stop !");
	int begin = ui.spinBox_StartIntensity->value();
	ui.textEdit_Info->append(QString("begin intensity is %1").arg(begin));
	int run = ui.spinBox_RunIntensity->value();
	ui.textEdit_Info->append(QString("run intensity is %1").arg(run));

	QByteArray data;
	m_pCommunicate->setData(OPENLEG, begin, run, false, data);
	ui.textEdit_Info->append("Send data :");
	ui.textEdit_Info->append(data.toHex().toUpper());
	m_pCommunicate->sendData(data);
}

void QtWidgetsCommunicateTest::On_pushButton_LegUnblock_Right1_Start_Clicked()
{
	ui.textEdit_Info->append("Enter leg unblock right1 start !");
	int begin = ui.spinBox_StartIntensity->value();
	ui.textEdit_Info->append(QString("begin intensity is %1").arg(begin));
	int run = ui.spinBox_RunIntensity->value();
	ui.textEdit_Info->append(QString("run intensity is %1").arg(run));

	QByteArray data;
	m_pCommunicate->setData(LEGUNBLOCK_RIGHTLEG1, begin, run, true, data);
	ui.textEdit_Info->append("Send data :");
	ui.textEdit_Info->append(data.toHex().toUpper());
	m_pCommunicate->sendData(data);
}

void QtWidgetsCommunicateTest::On_pushButton_LegUnblock_Right1_Stop_Clicked()
{
	ui.textEdit_Info->append("Enter leg unblock right1 stop !");
	int begin = ui.spinBox_StartIntensity->value();
	ui.textEdit_Info->append(QString("begin intensity is %1").arg(begin));
	int run = ui.spinBox_RunIntensity->value();
	ui.textEdit_Info->append(QString("run intensity is %1").arg(run));

	QByteArray data;
	m_pCommunicate->setData(LEGUNBLOCK_RIGHTLEG1, begin, run, false, data);
	ui.textEdit_Info->append("Send data :");
	ui.textEdit_Info->append(data.toHex().toUpper());
	m_pCommunicate->sendData(data);
}

void QtWidgetsCommunicateTest::On_pushButton_LegUnblock_Left1_Start_Clicked()
{
	ui.textEdit_Info->append("Enter leg unblock left1 start !");
	int begin = ui.spinBox_StartIntensity->value();
	ui.textEdit_Info->append(QString("begin intensity is %1").arg(begin));
	int run = ui.spinBox_RunIntensity->value();
	ui.textEdit_Info->append(QString("run intensity is %1").arg(run));

	QByteArray data;
	m_pCommunicate->setData(LEGUNBLOCK_LEFTLEG1, begin, run, true, data);
	ui.textEdit_Info->append("Send data :");
	ui.textEdit_Info->append(data.toHex().toUpper());
	m_pCommunicate->sendData(data);
}

void QtWidgetsCommunicateTest::On_pushButton_LegUnblock_Left1_Stop_Clicked()
{
	ui.textEdit_Info->append("Enter leg unblock left1 stop !");
	int begin = ui.spinBox_StartIntensity->value();
	ui.textEdit_Info->append(QString("begin intensity is %1").arg(begin));
	int run = ui.spinBox_RunIntensity->value();
	ui.textEdit_Info->append(QString("run intensity is %1").arg(run));

	QByteArray data;
	m_pCommunicate->setData(LEGUNBLOCK_LEFTLEG1, begin, run, false, data);
	ui.textEdit_Info->append("Send data :");
	ui.textEdit_Info->append(data.toHex().toUpper());
	m_pCommunicate->sendData(data);
}

void QtWidgetsCommunicateTest::On_pushButton_LegUnblock_Right2_Start_Clicked()
{
	ui.textEdit_Info->append("Enter leg unblock right2 start !");
	int begin = ui.spinBox_StartIntensity->value();
	ui.textEdit_Info->append(QString("begin intensity is %1").arg(begin));
	int run = ui.spinBox_RunIntensity->value();
	ui.textEdit_Info->append(QString("run intensity is %1").arg(run));

	QByteArray data;
	m_pCommunicate->setData(LEGUNBLOCK_RIGHTLEG2, begin, run, true, data);
	ui.textEdit_Info->append("Send data :");
	ui.textEdit_Info->append(data.toHex().toUpper());
	m_pCommunicate->sendData(data);
}

void QtWidgetsCommunicateTest::On_pushButton_LegUnblock_Right2_Stop_Clicked()
{
	ui.textEdit_Info->append("Enter leg unblock right2 stop !");
	int begin = ui.spinBox_StartIntensity->value();
	ui.textEdit_Info->append(QString("begin intensity is %1").arg(begin));
	int run = ui.spinBox_RunIntensity->value();
	ui.textEdit_Info->append(QString("run intensity is %1").arg(run));

	QByteArray data;
	m_pCommunicate->setData(LEGUNBLOCK_RIGHTLEG2, begin, run, false, data);
	ui.textEdit_Info->append("Send data :");
	ui.textEdit_Info->append(data.toHex().toUpper());
	m_pCommunicate->sendData(data);
}

void QtWidgetsCommunicateTest::On_pushButton_LegUnblock_Left2_Start_Clicked()
{
	ui.textEdit_Info->append("Enter leg unblock left2 start !");
	int begin = ui.spinBox_StartIntensity->value();
	ui.textEdit_Info->append(QString("begin intensity is %1").arg(begin));
	int run = ui.spinBox_RunIntensity->value();
	ui.textEdit_Info->append(QString("run intensity is %1").arg(run));

	QByteArray data;
	m_pCommunicate->setData(LEGUNBLOCK_LEFTLEG2, begin, run, true, data);
	ui.textEdit_Info->append("Send data :");
	ui.textEdit_Info->append(data.toHex().toUpper());
	m_pCommunicate->sendData(data);
}

void QtWidgetsCommunicateTest::On_pushButton_LegUnblock_Left2_Stop_Clicked()
{
	ui.textEdit_Info->append("Enter leg unblock left2 stop !");
	int begin = ui.spinBox_StartIntensity->value();
	ui.textEdit_Info->append(QString("begin intensity is %1").arg(begin));
	int run = ui.spinBox_RunIntensity->value();
	ui.textEdit_Info->append(QString("run intensity is %1").arg(run));

	QByteArray data;
	m_pCommunicate->setData(LEGUNBLOCK_LEFTLEG2, begin, run, false, data);
	ui.textEdit_Info->append("Send data :");
	ui.textEdit_Info->append(data.toHex().toUpper());
	m_pCommunicate->sendData(data);
}

void QtWidgetsCommunicateTest::On_pushButton_ShoulderUnblock_Start_Clicked()
{
	ui.textEdit_Info->append("Enter shoulder unblock start !");
	int begin = ui.spinBox_StartIntensity->value();
	ui.textEdit_Info->append(QString("begin intensity is %1").arg(begin));
	int run = ui.spinBox_RunIntensity->value();
	ui.textEdit_Info->append(QString("run intensity is %1").arg(run));

	QByteArray data;
	m_pCommunicate->setData(SHOULDERUNBLOCK, begin, run, true, data);
	ui.textEdit_Info->append("Send data :");
	ui.textEdit_Info->append(data.toHex().toUpper());
	m_pCommunicate->sendData(data);
}

void QtWidgetsCommunicateTest::On_pushButton_ShoulderUnblock_Stop_Clicked()
{
	ui.textEdit_Info->append("Enter shoulder unblock stop !");
	int begin = ui.spinBox_StartIntensity->value();
	ui.textEdit_Info->append(QString("begin intensity is %1").arg(begin));
	int run = ui.spinBox_RunIntensity->value();
	ui.textEdit_Info->append(QString("run intensity is %1").arg(run));

	QByteArray data;
	m_pCommunicate->setData(SHOULDERUNBLOCK, begin, run, false, data);
	ui.textEdit_Info->append("Send data :");
	ui.textEdit_Info->append(data.toHex().toUpper());
	m_pCommunicate->sendData(data);
}

void QtWidgetsCommunicateTest::On_pushButton_Handle_Start_Clicked()
{
	ui.textEdit_Info->append("Enter handle start !");
	int begin = ui.spinBox_StartIntensity->value();
	ui.textEdit_Info->append(QString("begin intensity is %1").arg(begin));
	int run = ui.spinBox_RunIntensity->value();
	ui.textEdit_Info->append(QString("run intensity is %1").arg(run));

	QByteArray data;
	m_pCommunicate->setData(HANDLE_MODEL, begin, run, true, data);
	ui.textEdit_Info->append("Send data :");
	ui.textEdit_Info->append(data.toHex().toUpper());
	m_pCommunicate->sendData(data);
}

void QtWidgetsCommunicateTest::On_pushButton_Handle_Stop_Clicked()
{
	ui.textEdit_Info->append("Enter handle stop !");
	int begin = ui.spinBox_StartIntensity->value();
	ui.textEdit_Info->append(QString("begin intensity is %1").arg(begin));
	int run = ui.spinBox_RunIntensity->value();
	ui.textEdit_Info->append(QString("run intensity is %1").arg(run));

	QByteArray data;
	m_pCommunicate->setData(HANDLE_MODEL, begin, run, false, data);
	ui.textEdit_Info->append("Send data :");
	ui.textEdit_Info->append(data.toHex().toUpper());
	m_pCommunicate->sendData(data);
}

void QtWidgetsCommunicateTest::On_pushButton_IncrIntensity_Clicked()
{
	ui.textEdit_Info->append("Intensity incr !");
	//m_pWindow->IncrIntensity();
	QByteArray data;
	m_pCommunicate->setData(OPENBACK, 1, ++intensity, true, data);
	ui.textEdit_Info->append("Send data :");
	ui.textEdit_Info->append(data.toHex().toUpper());
	m_pCommunicate->sendData(data);
}

void QtWidgetsCommunicateTest::On_pushButton_DecrIntensity_Clicked()
{
	ui.textEdit_Info->append("Intensity decr !");
	m_pWindow->DecrIntensity();
}