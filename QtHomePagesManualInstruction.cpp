#include "QtHomePagesManualInstruction.h"

QtHomePagesManualInstruction::QtHomePagesManualInstruction(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	ui.label_Instruction->setWordWrap(true);                     // true£º×Ô¶¯»»ÐÐ
	ui.label_Instruction->setAlignment(Qt::AlignVCenter);
}

QtHomePagesManualInstruction::~QtHomePagesManualInstruction()
{}
