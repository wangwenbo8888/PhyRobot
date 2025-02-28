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

QImage  cvMatToQImage(const cv::Mat& inMat)
{
    switch (inMat.type())
    {
        // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image(inMat.data,
            inMat.cols, inMat.rows,
            static_cast<int>(inMat.step),
            QImage::Format_ARGB32);

        return image;
    }

    // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage image(inMat.data,
            inMat.cols, inMat.rows,
            static_cast<int>(inMat.step),
            QImage::Format_RGB888);

        return image.rgbSwapped();
    }

    // 8-bit, 1 channel
    case CV_8UC1:
    {
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
        QImage image(inMat.data,
            inMat.cols, inMat.rows,
            static_cast<int>(inMat.step),
            QImage::Format_Grayscale8);//Format_Alpha8 and Format_Grayscale8 were added in Qt 5.5
#else//这里还有一种写法，最后给出
        static QVector<QRgb>  sColorTable;

        // only create our color table the first time
        if (sColorTable.isEmpty())
        {
            sColorTable.resize(256);

            for (int i = 0; i < 256; ++i)
            {
                sColorTable[i] = qRgb(i, i, i);
            }
        }

        QImage image(inMat.data,
            inMat.cols, inMat.rows,
            static_cast<int>(inMat.step),
            QImage::Format_Indexed8);

        image.setColorTable(sColorTable);
#endif

        return image;
    }

    default:
        qWarning() << "CVS::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
        break;
    }

    return QImage();
}

QPixmap cvMatToQPixmap(const cv::Mat& inMat)
{
    return QPixmap::fromImage(cvMatToQImage(inMat));
}

// 开始识别
void AutoTreatAcupointRecog::on_Pushbutton_RecogBegin_Clicked()
{
    //GetRGBDAndColor();
	std::vector<cv::Point3d> points;
	cv::Mat img;
    m_pAutoTreat->GetWindow()->getImage(img/*points,colorRawMat*/);
    cv::Mat imgR = img.t();
    cv::rotate(img, imgR, cv::ROTATE_90_COUNTERCLOCKWISE);
    cv::Mat image_part = imgR(cv::Rect(200, 140, 920, 460));
	ui.label_Image->setPixmap(QPixmap(cvMatToQPixmap(image_part)));
    //ui.label_Image->resize(imgR.cols,imgR.rows);

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
