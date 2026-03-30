#include "ManualTreatDragBegin.h"

#include "MyWindow.h"

ManualTreatDragBegin::ManualTreatDragBegin(MyWindow* window,QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	m_pPauseWidget.reset(new PauseWidget(this));
	m_pPauseWidget->hide();

	m_pStopWidget.reset(new AutoTreatStop());
	m_pStopWidget->SetWindow(window);
	m_pStopWidget->hide();

	m_pMyWindow = window;

	disconnect(ui.pushButton_Back, SIGNAL(clicked()), this, SLOT(On_pushButton_Back_Clicked()));
	connect(ui.pushButton_Back, SIGNAL(clicked()), this, SLOT(On_pushButton_Back_Clicked()));

	disconnect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(On_pushButton_LastStep_Clicked()));
	connect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(On_pushButton_LastStep_Clicked()));

	disconnect(ui.pushButton_PauseOrContinue, SIGNAL(clicked()), this, SLOT(On_pushButton_PauseOrContinue_Clicked()));
	connect(ui.pushButton_PauseOrContinue, SIGNAL(clicked()), this, SLOT(On_pushButton_PauseOrContinue_Clicked()));

	disconnect(ui.pushButton_StartOrStop, SIGNAL(clicked()), this, SLOT(On_pushButton_StartOrStop_Clicked()));
	connect(ui.pushButton_StartOrStop, SIGNAL(clicked()), this, SLOT(On_pushButton_StartOrStop_Clicked()));

	disconnect(ui.pushButton_StartOrClose, SIGNAL(clicked()), this, SLOT(On_pushButton_StartOrClose_Clicked()));
	connect(ui.pushButton_StartOrClose, SIGNAL(clicked()), this, SLOT(On_pushButton_StartOrClose_Clicked()));

	disconnect(ui.pushButton_PosFixed, SIGNAL(clicked()), this, SLOT(On_pushButton_PosFixed_Clicked()));
	connect(ui.pushButton_PosFixed, SIGNAL(clicked()), this, SLOT(On_pushButton_PosFixed_Clicked()));

	disconnect(ui.pushButton_TimerDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_TimerDecr_Clicked()));
	connect(ui.pushButton_TimerDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_TimerDecr_Clicked()));

	disconnect(ui.pushButton_TimerIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_TimerIncr_Clicked()));
	connect(ui.pushButton_TimerIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_TimerIncr_Clicked()));

	disconnect(ui.pushButton_IntensityDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityDecr_Clicked()));
	connect(ui.pushButton_IntensityDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityDecr_Clicked()));

	disconnect(ui.pushButton_IntensityIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityIncr_Clicked()));
	connect(ui.pushButton_IntensityIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityIncr_Clicked()));

	m_bStartFlag = false;
	m_bPosFixedPushed = false;
	m_bTreatStarted = false;

	m_pTimer = new QTimer();
	m_pTimer->setInterval(1000);
	connect(m_pTimer,SIGNAL(timeout()),this,SLOT(On_Timer_Out()));
}

ManualTreatDragBegin::~ManualTreatDragBegin()
{
}

void ManualTreatDragBegin::On_Timer_Out()
{
	--m_iTotalTime;
	++m_iElapseTime;

	if (m_iTotalTime==0)
	{
		ui.label_Timer->setText("00:00:00");
		ui.progressBar->setValue(100);
		ui.label_Tik->setPixmap(QPixmap(":/WhiteTik.png"));
		m_pTimer->stop();
	}

	int minute = m_iTotalTime / 60;
	int second = m_iTotalTime % 60;

	QString text("00:0");
	text.append(QString::number(minute));
	text.append(":");
	if (second>=10)
	{
		text.append(QString::number(second));
	}
	else
	{
		text.append("0");
		text.append(QString::number(second));
	}

	ui.label_Timer->setText(text);
	ui.progressBar->setValue(m_iElapseTime*100 /(m_iTotalTime+m_iElapseTime));
}

void ManualTreatDragBegin::ResumePara()
{
	ui.label_TimerValue->setText("00:01:00");
	ui.label_IntensityValue->setText("30");
}

void ManualTreatDragBegin::On_pushButton_Back_Clicked()
{
}
void ManualTreatDragBegin::On_pushButton_LastStep_Clicked()
{
}
void ManualTreatDragBegin::On_pushButton_PauseOrContinue_Clicked()
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

void ManualTreatDragBegin::On_pushButton_StartOrStop_Clicked()
{
	if (!m_bTreatStarted)
	{
		ui.label_Timer->setText(ui.label_TimerValue->text());
		QString time = ui.label_TimerValue->text();
		int minute = time.mid(4, 1).toInt();
		m_iTotalTime = minute * 60;
		m_iElapseTime = 0;

		m_pTimer->start();
		m_bTreatStarted = true;
	}
	else if (m_bTreatStarted)
	{
		m_pStopWidget->move(920, 440);
		m_pStopWidget->setWindowFlags(m_pStopWidget->windowFlags() | Qt::Dialog);
		m_pStopWidget->setWindowModality(Qt::ApplicationModal);
		m_pStopWidget->show();

		m_pTimer->stop();
		m_bTreatStarted = false;
	}

}

void ManualTreatDragBegin::On_pushButton_PosFixed_Clicked()
{
	if (!m_bPosFixedPushed)
	{
		ui.pushButton_PosFixed->setIcon(QIcon(":/DragPosFixedPushed.png"));

		m_pMyWindow->StopDrag();

		m_bPosFixedPushed = true;
	}
	else if (m_bPosFixedPushed)
	{
		ui.pushButton_PosFixed->setIcon(QIcon(":/ManualTreatDragFixedImage.png"));

		m_pMyWindow->StartDrag();

		m_bPosFixedPushed = false;
	}
}

void ManualTreatDragBegin::On_pushButton_StartOrClose_Clicked()
{
	if (!m_bStartFlag)
	{
		ui.pushButton_StartOrClose->setIcon(QIcon(":/DragStartOrClosePushed.png"));
		m_pMyWindow->MoveToNormalPos();

		cv::Mat colorRawMat;
		std::vector<OBColorPoint> pointCloud_frame_data;
		obCapture(colorRawMat, pointCloud_frame_data);
		SaveImage(colorRawMat);

		m_pMyWindow->StartDrag();

		m_bStartFlag = true;
	}
	else if (m_bStartFlag)
	{
		ui.pushButton_StartOrClose->setIcon(QIcon(":/ManualTreatDragStartOrCloseImageUnpush.png"));
		m_pMyWindow->StopDrag();
		m_pMyWindow->MoveToNormalPos();

		m_bStartFlag = false;
	}
}

void ManualTreatDragBegin::On_pushButton_TimerDecr_Clicked()
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
void ManualTreatDragBegin::On_pushButton_TimerIncr_Clicked()
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
void ManualTreatDragBegin::On_pushButton_IntensityDecr_Clicked()
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

void ManualTreatDragBegin::On_pushButton_IntensityIncr_Clicked()
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

