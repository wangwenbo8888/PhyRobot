#include "IntensiveTreatAcupointRecog.h"

#include "IntensiveTreat.h"

IntensiveTreatAcupointRecog::IntensiveTreatAcupointRecog(IntensiveTreat* treat, QWidget *parent)
	: m_pIntensiveTreat(treat)
	, QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_BackToHome, SIGNAL(clicked()), this, SLOT(On_pushButton_BackToHome_Clicked()));
	connect(ui.pushButton_BackToHome, SIGNAL(clicked()), this, SLOT(On_pushButton_BackToHome_Clicked()));

	disconnect(ui.pushButton_DragModel, SIGNAL(clicked()), this, SLOT(On_pushButton_DragModel_Clicked()));
	connect(ui.pushButton_DragModel, SIGNAL(clicked()), this, SLOT(On_pushButton_DragModel_Clicked()));

	disconnect(ui.pushButton_HandleModel, SIGNAL(clicked()), this, SLOT(On_pushButton_HandleModel_Clicked()));
	connect(ui.pushButton_HandleModel, SIGNAL(clicked()), this, SLOT(On_pushButton_HandleModel_Clicked()));

}

IntensiveTreatAcupointRecog::~IntensiveTreatAcupointRecog()
{

}

void IntensiveTreatAcupointRecog::On_pushButton_BackToHome_Clicked()
{
	m_pIntensiveTreat->FinishReturn();
}

void IntensiveTreatAcupointRecog::On_pushButton_DragModel_Clicked()
{
	ui.pushButton_DragModel->setIcon(QIcon(":/DragModelPushed.png"));
	m_pIntensiveTreat->SetWidgetDragBegin();
	ui.pushButton_DragModel->setIcon(QIcon(":/DragModelUnpush.png"));
}

void IntensiveTreatAcupointRecog::On_pushButton_HandleModel_Clicked()
{
	ui.pushButton_HandleModel->setIcon(QIcon(":/HandleModelPushed.png"));
	m_pIntensiveTreat->SetWidgetHandleBegin();

	ui.pushButton_HandleModel->setIcon(QIcon(":/HandleModelUnpush.png"));
}


