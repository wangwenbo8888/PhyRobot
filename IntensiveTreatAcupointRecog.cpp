#include "IntensiveTreatAcupointRecog.h"

#include "IntensiveTreat.h"

IntensiveTreatAcupointRecog::IntensiveTreatAcupointRecog(IntensiveTreat* treat, QWidget *parent)
	: m_pIntensiveTreat(treat)
	, QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_LastStep_Clicked()));
	connect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_LastStep_Clicked()));

	disconnect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_NextStep_Clicked()));
	connect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_NextStep_Clicked()));
}

IntensiveTreatAcupointRecog::~IntensiveTreatAcupointRecog()
{

}

void IntensiveTreatAcupointRecog::on_Pushbutton_LastStep_Clicked()
{
	m_pIntensiveTreat->SetWidgetSetTime();
}

void IntensiveTreatAcupointRecog::on_Pushbutton_NextStep_Clicked()
{
	m_pIntensiveTreat->SetWidgetToleranceTest();
}

void IntensiveTreatAcupointRecog::on_pushButton_ReconBegin_clicked()
{
//    std::vector<cv::Point> points =
//            m_pIntensiveTreat->GetWindow()->detect(u8"debug/ikju_png.rf.548cc0a09e4d068d884d35d8ed43f10c.jpg",
//                                           u8"debug/yolov8_640_640_v15.onnx");
}
