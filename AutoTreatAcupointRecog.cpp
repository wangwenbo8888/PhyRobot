#include "AutoTreatAcupointRecog.h"

#include "AutoTreat.h"
#include "CameraGrabber.h"
#include "MyWindow.h"
#include "qevent.h"

AutoTreatAcupointRecog::AutoTreatAcupointRecog(AutoTreat* treat, QWidget *parent)
	: m_pAutoTreat(treat)
	, QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_LastStep_Clicked()));
	connect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_LastStep_Clicked()));

	disconnect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_NextStep_Clicked()));
	connect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_NextStep_Clicked()));

	disconnect(ui.pushButton_ReconBegin, SIGNAL(clicked()), this, SLOT(on_Pushbutton_RecogBegin_Clicked()));
	connect(ui.pushButton_ReconBegin, SIGNAL(clicked()), this, SLOT(on_Pushbutton_RecogBegin_Clicked()));

	disconnect(ui.pushButton_Manual, SIGNAL(clicked()), this, SLOT(on_Pushbutton_Manual_Clicked()));
	connect(ui.pushButton_Manual, SIGNAL(clicked()), this, SLOT(on_Pushbutton_Manual_Clicked()));

	this->setMouseTracking(true);
	m_bManualMove = false;
}

AutoTreatAcupointRecog::~AutoTreatAcupointRecog()
{
}

void AutoTreatAcupointRecog::mouseMoveEvent(QMouseEvent* ev)
{
	if (!m_bManualMove)
	{
		return;
	}

	for (int i = 0; i < m_vAcupoints.size();++i)
	{
		int a = ev->x();
		int b = ev->y();

		QSharedPointer<QLabel>& label = m_vAcupoints[i];
		qDebug() << "mouse x is " << a;
		qDebug() << "mouse y is " << b;
		qDebug() << "label left is " << label->geometry().left();
		qDebug() << "label top is " << label->geometry().top();
		qDebug() << "label width is " << label->geometry().width();
		qDebug() << "label height is " << label->geometry().height();

		if (label->geometry().contains(a,b))
		{
				// 鼠标在控件内
				label->move(a,b);
				return;
		}
	}
	
	return;
}

// 开始识别
void AutoTreatAcupointRecog::on_Pushbutton_RecogBegin_Clicked()
{
    //GetRGBDAndColor();
	cv::Mat img;
    m_pAutoTreat->GetWindow()->getImage(img/*points,colorRawMat*/);
    //cv::Mat imgR = img.t();
    //cv::rotate(img, imgR, cv::ROTATE_90_COUNTERCLOCKWISE);
    cv::Mat image_part = img(cv::Rect(200, 140, 920, 460));
	ui.label_Image->setPixmap(QPixmap(m_pAutoTreat->GetWindow()->cvMatToQPixmap(image_part)));

	std::vector<RobotPoint>& points = m_pAutoTreat->GetWindow()->GetPlanPoints();
	m_vAcupoints.clear();
	for (int i = 0; i < points.size(); ++i)
	{
		const cv::Point& p = points[i].p2d;
		QSharedPointer<QLabel> label;
		label.reset(new QLabel(ui.label_Image));
		label->move(p.y - 200 - 30, 460 + 140 - p.x - 56);
		label->setPixmap(QPixmap(":/untreat.png"));
		label->show();
		m_vAcupoints.push_back(label);
	}
    //ui.label_Image->resize(imgR.cols,imgR.rows);

}

// 手动调整
void AutoTreatAcupointRecog::on_Pushbutton_Manual_Clicked()
{
	m_bManualMove = !m_bManualMove;
}

void AutoTreatAcupointRecog::on_Pushbutton_LastStep_Clicked()
{
	m_pAutoTreat->SetWidgetSetTime();
}

void AutoTreatAcupointRecog::on_Pushbutton_NextStep_Clicked()
{
	m_pAutoTreat->SetWidgetToleranceTest();
}
