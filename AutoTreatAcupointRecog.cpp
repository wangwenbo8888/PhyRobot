#include "AutoTreatAcupointRecog.h"

#include "AutoTreat.h"
#include "CameraGrabber.h"
#include "MyWindow.h"

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
}

AutoTreatAcupointRecog::~AutoTreatAcupointRecog()
{
}

// 开始识别
void AutoTreatAcupointRecog::on_Pushbutton_RecogBegin_Clicked()
{
    //GetRGBDAndColor();
	std::vector<cv::Point3d> points;
	cv::Mat colorRawMat;
    m_pAutoTreat->GetWindow()->getImage(/*points,colorRawMat*/);


}

// 手动调整
void AutoTreatAcupointRecog::on_Pushbutton_Manual_Clicked()
{
}

void AutoTreatAcupointRecog::on_Pushbutton_LastStep_Clicked()
{
	m_pAutoTreat->SetWidgetSetTime();
}

void AutoTreatAcupointRecog::on_Pushbutton_NextStep_Clicked()
{
	m_pAutoTreat->SetWidgetToleranceTest();
}
