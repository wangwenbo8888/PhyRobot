#include "QtHomePagesOrganizeInstruct.h"

QtHomePagesOrganizeInstruct::QtHomePagesOrganizeInstruct(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);

	ui.label_Instruction->setWordWrap(true);                     // true£º×Ô¶¯»»ÐÐ
	ui.label_Instruction->setAlignment(Qt::AlignVCenter);
}

QtHomePagesOrganizeInstruct::~QtHomePagesOrganizeInstruct()
{

}
