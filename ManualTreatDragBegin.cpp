#include "ManualTreatDragBegin.h"

#include "MyWindow.h"

#include <QMessageBox>

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

	disconnect(ui.pushButton_DragStartOrStop, SIGNAL(clicked()), this, SLOT(On_pushButton_DragStartOrStop_Clicked()));
	connect(ui.pushButton_DragStartOrStop, SIGNAL(clicked()), this, SLOT(On_pushButton_DragStartOrStop_Clicked()));
	
	//disconnect(ui.pushButton_LocationStartOrStop, SIGNAL(clicked()), this, SLOT(On_pushButton_LocationStartOrStop_Clicked()));
	//connect(ui.pushButton_LocationStartOrStop, SIGNAL(clicked()), this, SLOT(On_pushButton_LocationStartOrStop_Clicked()));
	//disconnect(ui.pushButton_PosFixed, SIGNAL(clicked()), this, SLOT(On_pushButton_PosFixed_Clicked()));
	//connect(ui.pushButton_PosFixed, SIGNAL(clicked()), this, SLOT(On_pushButton_PosFixed_Clicked()));

	//disconnect(ui.pushButton_TimerDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_TimerDecr_Clicked()));
	//connect(ui.pushButton_TimerDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_TimerDecr_Clicked()));

	//disconnect(ui.pushButton_TimerIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_TimerIncr_Clicked()));
	//connect(ui.pushButton_TimerIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_TimerIncr_Clicked()));

	disconnect(ui.pushButton_IntensityDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityDecr_Clicked()));
	connect(ui.pushButton_IntensityDecr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityDecr_Clicked()));

	disconnect(ui.pushButton_IntensityIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityIncr_Clicked()));
	connect(ui.pushButton_IntensityIncr, SIGNAL(clicked()), this, SLOT(On_pushButton_IntensityIncr_Clicked()));

	disconnect(ui.pushButton_TreatStart, SIGNAL(clicked()), this, SLOT(On_pushButton_TreatStart_Clicked()));
	connect(ui.pushButton_TreatStart, SIGNAL(clicked()), this, SLOT(On_pushButton_TreatStart_Clicked()));

	disconnect(ui.pushButton_TreatStop, SIGNAL(clicked()), this, SLOT(On_pushButton_TreatStop_Clicked()));
	connect(ui.pushButton_TreatStop, SIGNAL(clicked()), this, SLOT(On_pushButton_TreatStop_Clicked()));

	ui.pushButton_PauseOrContinue->hide();
	ui.pushButton_LastStep->hide();
	ui.pushButton_StartOrStop->hide();

	m_bFirstDrag = true;
	m_bStartFlag = false;
	m_bPosFixedPushed = false;
	m_bTreatStarted = false;

	//m_pCommunicate = m_pMyWindow->GetCommunicate();

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
		//ui.label_Timer->setText("00:00:00");
		//ui.progressBar->setValue(100);
		//ui.label_Tik->setPixmap(QPixmap(":/WhiteTik.png"));
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

	//ui.label_Timer->setText(text);
	//ui.progressBar->setValue(m_iElapseTime*100 /(m_iTotalTime+m_iElapseTime));
}

void ManualTreatDragBegin::ResumePara()
{
	//ui.label_TimerValue->setText("00:01:00");
	//ui.label_IntensityValue->setText("30");
}

void ManualTreatDragBegin::On_pushButton_Back_Clicked()
{
	m_pMyWindow->SetWidgetHomePage();
	m_pMyWindow->show();

	// 如果拖拽模式打开，则关闭
	if (m_bStartFlag)
	{
		On_pushButton_DragStartOrStop_Clicked();
	}
	//this->hide();
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
		//ui.label_Timer->setText(ui.label_TimerValue->text());
		//QString time = ui.label_TimerValue->text();
		//int minute = time.mid(4, 1).toInt();
		//m_iTotalTime = minute * 60;
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
		//ui.pushButton_PosFixed->setIcon(QIcon(":/DragPosFixedPushed.png"));

		m_pMyWindow->StopDrag();

		m_bPosFixedPushed = true;
	}
	else if (m_bPosFixedPushed)
	{
		//ui.pushButton_PosFixed->setIcon(QIcon(":/ManualTreatDragFixedImage.png"));

		m_pMyWindow->StartDrag();

		m_bPosFixedPushed = false;
	}
}

void ManualTreatDragBegin::On_pushButton_LocationStartOrStop_Clicked()
{

}

void ManualTreatDragBegin::On_pushButton_DragStartOrStop_Clicked()
{
	if (!m_bStartFlag)
	{
		m_bStartFlag = true;
		ui.pushButton_DragStartOrStop->setIcon(QIcon(":/DragStart.png"));

		if (m_bFirstDrag)
		{
			m_pMyWindow->MoveToNormalPos();

			cv::Mat colorRawMat;
			std::vector<OBColorPoint> pointCloud_frame_data;
			obCapture(colorRawMat, pointCloud_frame_data);

			if (colorRawMat.rows == 0 || colorRawMat.cols == 0)
			{
				QMessageBox::information(NULL, "Info", "Capture image failed !", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

				m_bStartFlag = true;
				return;
			}

			SaveImage(colorRawMat);

			m_bFirstDrag = false;
		}

		m_pMyWindow->StartDrag();

		//m_bStartFlag = true;
	}
	else if (m_bStartFlag)
	{
		ui.pushButton_DragStartOrStop->setIcon(QIcon(":/DragStop.png"));
		m_pMyWindow->StopDrag();
		//m_pMyWindow->MoveToNormalPos();

		m_bStartFlag = false;
	}
}

void ManualTreatDragBegin::On_pushButton_TimerDecr_Clicked()
{
	//QString time = ui.label_TimerValue->text();
	//int minute = time.mid(4, 1).toInt();
	//if (minute <= 1)
	{
		//ui.label_TimerValue->setText("00:01:00");
	}
	//else
	{
		//time = "00:0";
		//time.append(QString::number(minute - 1));
		//time.append(":00");
		//ui.label_TimerValue->setText(time);
	}
}
void ManualTreatDragBegin::On_pushButton_TimerIncr_Clicked()
{
	//QString time = ui.label_TimerValue->text();
	//int minute = time.mid(4, 1).toInt();
	//if (minute >= 5)
	//{
	//	ui.label_TimerValue->setText("00:05:00");
	//}
	//else
	//{
	//	time = "00:0";
	//	time.append(QString::number(minute + 1));
	//	time.append(":00");
	//	ui.label_TimerValue->setText(time);
	//}
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
		//m_pMyWindow->DecrIntensity();
		int begin = 1;
		//int run = ui.label_IntensityValue->text().toInt();
		int run = value - 1;
		QByteArray data;
		m_pMyWindow->GetCommunicate()->setData(HANDLE_MODEL, begin, run, true, data);

		qDebug() << "Drag model send start data " << data.toHex().toUpper();
		m_pMyWindow->GetCommunicate()->sendData(data);
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
		//m_pMyWindow->IncrIntensity();
		int begin = 1;
		//int run = ui.label_IntensityValue->text().toInt();
		int run = value + 1;
		QByteArray data;
		m_pMyWindow->GetCommunicate()->setData(HANDLE_MODEL, begin, run, true, data);

		qDebug() << "Drag model send start data " << data.toHex().toUpper();
		m_pMyWindow->GetCommunicate()->sendData(data);
	}
}

void ManualTreatDragBegin::On_pushButton_TreatStart_Clicked()
{
	if (!m_bTreatStarted)
	{
		ui.pushButton_TreatStart->setIcon(QIcon(":/LabelDragStartPushed.png"));

		m_bTreatStarted = true;

		int begin = 1;
		int run = ui.label_IntensityValue->text().toInt();
		QByteArray data;
		m_pMyWindow->GetCommunicate()->setData(HANDLE_MODEL, begin, run, true, data);

		qDebug() << "Drag model send start data " << data.toHex().toUpper();
		m_pMyWindow->GetCommunicate()->sendData(data);

		ui.pushButton_TreatStop->setIcon(QIcon(":/LabelDragStopUnpush.png"));
	}
	else if (m_bTreatStarted)
	{
		ui.pushButton_TreatStart->setIcon(QIcon(":/LabelDragStartUnpush.png"));

		m_bTreatStarted = false;

		ui.pushButton_TreatStop->setIcon(QIcon(":/LabelDragStopPushed.png"));
	}
}

void ManualTreatDragBegin::On_pushButton_TreatStop_Clicked()
{
	if (!m_bTreatStarted)
	{
		ui.pushButton_TreatStop->setIcon(QIcon(":/LabelDragStopUnpush.png"));

		m_bTreatStarted = true;

		ui.pushButton_TreatStart->setIcon(QIcon(":/LabelDragStartPushed.png"));
	}
	else if (m_bTreatStarted)
	{
		ui.pushButton_TreatStop->setIcon(QIcon(":/LabelDragStopPushed.png"));

		m_bTreatStarted = false;

		int begin = 1;
		int run = 1;
		QByteArray data;
		m_pMyWindow->GetCommunicate()->setData(HANDLE_MODEL, begin, run, false, data);
		qDebug() << "Drag model send stop data " << data.toHex().toUpper();
		m_pMyWindow->GetCommunicate()->sendData(data);

		ui.pushButton_TreatStart->setIcon(QIcon(":/LabelDragStartUnpush.png"));
	}
}