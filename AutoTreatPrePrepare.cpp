#include "AutoTreatPrePrepare.h"

#include "AutoTreat.h"

AutoTreatPrePrepare::AutoTreatPrePrepare(AutoTreat* treat, QWidget *parent)
	: m_pAutoTreat(treat)
	, QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);
	
	disconnect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(on_PushButton_Confirm()));
	connect(ui.pushButton_Confirm, SIGNAL(clicked()), this, SLOT(on_PushButton_Confirm()));

	disconnect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_LastStep_Clicked()));
	connect(ui.pushButton_LastStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_LastStep_Clicked()));

	disconnect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_NextStep_Clicked()));
	connect(ui.pushButton_NextStep, SIGNAL(clicked()), this, SLOT(on_Pushbutton_NextStep_Clicked()));

	m_eOperation = PREPARE_ONE;

	ui.label_Instruction->hide();
	ui.label_Glass->hide();
}

AutoTreatPrePrepare::~AutoTreatPrePrepare()
{
}

void AutoTreatPrePrepare::on_Pushbutton_NextStep_Clicked()
{
	if (m_eOperation == PREPARE_ONE)
	{
		//m_pAutoTreat->SetWidgetSetTime();

		ui.label_Instruction->show();
		ui.label_Glass->show();
		ui.label_GreenTriangle1->setPixmap(QPixmap(":/GreenTriangle.png"));
		//ui.label_WorkTest->setStyleSheet("background-color:white;border:2px solid rgb(101,158,213);");
		//ui.label_BalmApply->setStyleSheet("background-color:rgb(101,158,213)");
		//ui.label_Hook->setPixmap(QPixmap(":/GrayHook.png"));

		//ui.pushButton_NextStep->setEnabled(false);

		m_eOperation = PREPARE_TWO;
		//ui.pushButton->setText(QStringLiteral("请涂抹凝胶"));
	}
	else if (m_eOperation== PREPARE_TWO)
	{
		//ui.label_BalmApply->setStyleSheet("background-color:white;border:2px solid rgb(101,158,213);");
		//ui.label_MetalDetect->setStyleSheet("background-color:rgb(101,158,213)");
		//ui.pushButton_NextStep->setEnabled(false);

		//ui.label_Hook->setPixmap(QPixmap(":/GrayHook.png"));

		//m_eOperation = PREPARE_THREE;
		//ui.pushButton->setText(QStringLiteral("人体金属探测"));
	}
	else if (m_eOperation == PREPARE_THREE)
	{
		//ui.label_MetalDetect->setStyleSheet("background-color:white;border:2px solid rgb(101,158,213);");
		//ui.label_OperDebug->setStyleSheet("background-color:rgb(101,158,213)");
		//ui.pushButton_NextStep->setEnabled(false);
		ui.label_GreenTriangle1->clear();
		ui.label_GreenTriangle2->setPixmap(QPixmap(":/GreenTriangle.png"));
		ui.label_Human->setPixmap(QPixmap(":/HumanAndXuewei.png"));
		ui.label_Instruction->setText(QStringLiteral("请按照人体图片指示负极穴位区，依次根据电极贴线缆贴纸上穴位名称，对应将负极电极贴粘贴到治疗对象身体上的穴位上，确定粘贴完成后点击下面确定按钮!"));
		//ui.label_Hook->setPixmap(QPixmap(":/GrayHook.png"));

		m_eOperation = PREPARE_FOUR;
		//ui.pushButton->setText(QStringLiteral("操作调试"));
	}
	else if (m_eOperation == PREPARE_THIRTEEN)
	{
		//ui.label_OperDebug->setStyleSheet("background-color:white;border:2px solid rgb(101,158,213);");
		//ui.label_WorkTest->setStyleSheet("background-color:rgb(101,158,213)");
		//ui.pushButton_NextStep->setEnabled(false);

		//ui.label_Hook->setPixmap(QPixmap(":/GrayHook.png"));
		ui.label_GreenTriangle2->clear();
		ui.label_GreenTriangle3->setPixmap(QPixmap(":/GreenTriangle.png"));
		ui.label_Glass->show();
		ui.label_Human->setPixmap(QPixmap(":/Human1.png"));
		ui.label_Instruction->setText(QStringLiteral("请涂抹导电凝胶，涂抹完成后点击下面确定按钮!"));

		m_eOperation = PREPARE_FOURTEEN;
		//ui.pushButton->setText(QStringLiteral("整机作业测试"));
	}
}


void AutoTreatPrePrepare::on_Pushbutton_LastStep_Clicked()
{
	m_pAutoTreat->SetWidgetInstruction();
}

void AutoTreatPrePrepare::on_PushButton_Confirm()
{
	// ui.pushButton_NextStep->setEnabled(true);
	// ui.label_Hook->setPixmap(QPixmap(":/GreenHook.png"));
	if (m_eOperation == PREPARE_TWO)
	{
		m_eOperation = PREPARE_THREE;
	}
	else if (m_eOperation == PREPARE_FOUR)
	{
		ui.label->move(790,10);
		ui.label->setPixmap(QPixmap(":/FuGuGou.png"));
		ui.label_Human->setPixmap(QPixmap(":/HumanAndXuewei1.png"));
		m_eOperation = PREPARE_FIVE;
	}
	else if (m_eOperation == PREPARE_FIVE)
	{
		ui.label->setPixmap(QPixmap(":/HuanTiao.png"));
		ui.label_Human->setPixmap(QPixmap(":/HumanAndXuewei2.png"));
		m_eOperation = PREPARE_SIX;
	}
	else if (m_eOperation == PREPARE_SIX)
	{
		ui.label->setPixmap(QPixmap(":/NeiGuan.png"));
		ui.label_Human->setPixmap(QPixmap(":/HumanAndXuewei3.png"));
		m_eOperation = PREPARE_SEVEN;
	}
	else if (m_eOperation == PREPARE_SEVEN)
	{
		ui.label->setPixmap(QPixmap(":/WaiGuan.png"));
		ui.label_Human->setPixmap(QPixmap(":/HumanAndXuewei4.png"));
		m_eOperation = PREPARE_EIGHT;
	}
	else if (m_eOperation == PREPARE_EIGHT)
	{
		ui.label->setPixmap(QPixmap(":/WeiZhong.png"));
		ui.label_Human->setPixmap(QPixmap(":/HumanAndXuewei5.png"));
		m_eOperation = PREPARE_NINE;
	}
	else if (m_eOperation == PREPARE_NINE)
	{
		ui.label->setPixmap(QPixmap(":/WeiYang.png"));
		ui.label_Human->setPixmap(QPixmap(":/HumanAndXuewei6.png"));
		m_eOperation = PREPARE_TEN;
	}
	else if (m_eOperation == PREPARE_TEN)
	{
		ui.label->setPixmap(QPixmap(":/KunLun.png"));
		ui.label_Human->setPixmap(QPixmap(":/HumanAndXuewei7.png"));
		m_eOperation = PREPARE_ELEVEN;
	}
	else if (m_eOperation == PREPARE_ELEVEN)
	{
		ui.label->setPixmap(QPixmap(":/JieXi.png"));
		ui.label_Human->setPixmap(QPixmap(":/HumanAndXuewei8.png"));
		m_eOperation = PREPARE_TWELVE;
	}
	else if (m_eOperation == PREPARE_TWELVE)
	{
		ui.label->move(890,10);
		ui.label->setPixmap(QPixmap(":/Gel.png"));
		ui.label_Human->setPixmap(QPixmap(":/HumanAndXuewei.png"));
		m_eOperation = PREPARE_THIRTEEN;
	}
	else if (m_eOperation == PREPARE_FOURTEEN)
	{
		m_pAutoTreat->SetWidgetSelectAndSetting();
	}
}