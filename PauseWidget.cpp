#include "PauseWidget.h"

PauseWidget::PauseWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
}

PauseWidget::~PauseWidget()
{

}
