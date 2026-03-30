#include "ManualTreatFinish.h"

ManualTreatFinish::ManualTreatFinish(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	disconnect(ui.pushButton_Back,SIGNAL(clicked()),this,SLOT(On_pushButton_Back_Clicked()));
	connect(ui.pushButton_Back, SIGNAL(clicked()), this, SLOT(On_pushButton_Back_Clicked()));

	disconnect(ui.pushButton_TreatContinue, SIGNAL(clicked()), this, SLOT(On_pushButton_TreatContinue_Clicked()));
	connect(ui.pushButton_TreatContinue, SIGNAL(clicked()), this, SLOT(On_pushButton_TreatContinue_Clicked()));

	disconnect(ui.pushButton_ModelSelect, SIGNAL(clicked()), this, SLOT(On_pushButton_ModelSelect_Clicked()));
	connect(ui.pushButton_ModelSelect, SIGNAL(clicked()), this, SLOT(On_pushButton_ModelSelect_Clicked()));
}

ManualTreatFinish::~ManualTreatFinish()
{

}

void ManualTreatFinish::On_pushButton_Back_Clicked()
{

}

void ManualTreatFinish::On_pushButton_TreatContinue_Clicked()
{

}

void ManualTreatFinish::On_pushButton_ModelSelect_Clicked()
{

}
