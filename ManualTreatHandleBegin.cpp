#include "ManualTreatHandleBegin.h"

#include "MyWindow.h"

ManualTreatHandleBegin::ManualTreatHandleBegin(MyWindow* window, QWidget *parent)
	: m_pWindow(window),
	QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	m_pPauseWidget.reset(new PauseWidget(this));
	m_pPauseWidget->hide();

	m_pStopWidget.reset(new AutoTreatStop());
	m_pStopWidget->SetWindow(window);
	m_pStopWidget->hide();

	m_pCommunicate = m_pWindow->GetCommunicate();

	disconnect(ui.pushButton_Back, SIGNAL(clicked()), this, SLOT(On_pushButton_Back_Clicked()));
	connect(ui.pushButton_Back, SIGNAL(clicked()), this, SLOT(On_pushButton_Back_Clicked()));

	disconnect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(On_pushButton_LastStep_Clicked()));
	connect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(On_pushButton_LastStep_Clicked()));

	disconnect(ui.pushButton_PauseOrContinue, SIGNAL(clicked()), this, SLOT(On_pushButton_PauseOrContinue_Clicked()));
	connect(ui.pushButton_PauseOrContinue, SIGNAL(clicked()), this, SLOT(On_pushButton_PauseOrContinue_Clicked()));

	disconnect(ui.pushButton_StartOrStop, SIGNAL(clicked()), this, SLOT(On_pushButton_StartOrStop_Clicked()));
	connect(ui.pushButton_StartOrStop, SIGNAL(clicked()), this, SLOT(On_pushButton_StartOrStop_Clicked()));

	disconnect(ui.pushButton_TimerDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_TimerDecr_Clicked()));
	connect(ui.pushButton_TimerDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_TimerDecr_Clicked()));

	disconnect(ui.pushButton_TimerIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_TimerIncr_Clicked()));
	connect(ui.pushButton_TimerIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_TimerIncr_Clicked()));

	disconnect(ui.pushButton_IntensityDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityDecr_Clicked()));
	connect(ui.pushButton_IntensityDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityDecr_Clicked()));

	disconnect(ui.pushButton_IntensityIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityIncr_Clicked()));
	connect(ui.pushButton_IntensityIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityIncr_Clicked()));

	disconnect(m_pStopWidget.get(), SIGNAL(EmitConfirmed()), this, SLOT(On_TreatStop_Confirmed()));
	connect(m_pStopWidget.get(), SIGNAL(EmitConfirmed()), this, SLOT(On_TreatStop_Confirmed()));

	m_bStarted = false;
}

ManualTreatHandleBegin::~ManualTreatHandleBegin()
{
}

void ManualTreatHandleBegin::ResumePara()
{
	ui.label_TimerValue->setText("00:01:00");
	ui.label_IntensityValue->setText("30");
}

void ManualTreatHandleBegin::On_pushButton_Back_Clicked()
{
}
void ManualTreatHandleBegin::On_pushButton_LastStep_Clicked()
{
}
void ManualTreatHandleBegin::On_pushButton_PauseOrContinue_Clicked()
{
	if (m_pPauseWidget->isVisible())
	{

		m_pPauseWidget->hide();
	}
	else
	{
		m_pPauseWidget->move(720, 320);
		m_pPauseWidget->setWindowFlags(m_pPauseWidget->windowFlags() | Qt::Dialog);
		//m_pPauseWidget->setWindowModality(Qt::ApplicationModal);
		m_pPauseWidget->show();
	}
}

void ManualTreatHandleBegin::On_pushButton_StartOrStop_Clicked()
{
	if (m_bStarted)
	{
		m_pStopWidget->move(920, 440);
		m_pStopWidget->setWindowFlags(m_pStopWidget->windowFlags() | Qt::Dialog);
		m_pStopWidget->setWindowModality(Qt::ApplicationModal);
		m_pStopWidget->show();
	}
	else
	{
		int begin = 1;
		int run = 40;
		QByteArray data;
		m_pCommunicate->setData(HANDLE_MODEL, begin, run, true, data);

		qDebug() << "Handle model send start data " << data.toHex().toUpper();
		m_pCommunicate->sendData(data);

		m_bStarted = true;

	}

}


void ManualTreatHandleBegin::On_TreatStop_Confirmed()
{
	int begin = 1;
	int run = 1;
	QByteArray data;
	m_pCommunicate->setData(HANDLE_MODEL, begin, run, false, data);
	qDebug()<<"Handle model send stop data "<<data.toHex().toUpper();
	m_pCommunicate->sendData(data);
}

void ManualTreatHandleBegin::On_pushButton_TimerDecr_Clicked()
{
	QString time = ui.label_TimerValue->text();
	int minute = time.mid(4, 1).toInt();
	if (minute <= 1)
	{
		ui.label_TimerValue->setText("00:01:00");
	}
	else
	{
		time = "00:0";
		time.append(QString::number(minute - 1));
		time.append(":00");
		ui.label_TimerValue->setText(time);
	}
}
void ManualTreatHandleBegin::On_pushButton_TimerIncr_Clicked()
{
	QString time = ui.label_TimerValue->text();
	int minute = time.mid(4, 1).toInt();
	if (minute >= 5)
	{
		ui.label_TimerValue->setText("00:05:00");
	}
	else
	{
		time = "00:0";
		time.append(QString::number(minute + 1));
		time.append(":00");
		ui.label_TimerValue->setText(time);
	}
}
void ManualTreatHandleBegin::On_pushButton_IntensityDecr_Clicked()
{
	if (ui.label_IntensityValue->text().toInt() <= 1)
	{
		ui.label_IntensityValue->setText("1");
	}
	else
	{
		int value = ui.label_IntensityValue->text().toInt();
		ui.label_IntensityValue->setText(QString::number(value - 1));
	}
}

void ManualTreatHandleBegin::On_pushButton_IntensityIncr_Clicked()
{
	if (ui.label_IntensityValue->text().toInt() >= 99)
	{
		ui.label_IntensityValue->setText("99");
	}
	else
	{
		int value = ui.label_IntensityValue->text().toInt();
		ui.label_IntensityValue->setText(QString::number(value + 1));
	}
}


