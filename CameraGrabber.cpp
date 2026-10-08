

#include "CameraGrabber.h"

#include <QElapsedTimer>

#include "opencv2/opencv.hpp"

// Save point cloud data to ply
void savePointsToPly(std::shared_ptr<ob::Frame> frame, std::string fileName) {
    int   pointsSize = frame->dataSize() / sizeof(OBPoint);
    FILE* fp;
    fopen_s(&fp,fileName.c_str(), "wb+");
    fprintf(fp, "ply\n");
    fprintf(fp, "format ascii 1.0\n");
    fprintf(fp, "element vertex %d\n", pointsSize);
    fprintf(fp, "property float x\n");
    fprintf(fp, "property float y\n");
    fprintf(fp, "property float z\n");
    fprintf(fp, "end_header\n");

    OBPoint* point = (OBPoint*)frame->data();
    for (int i = 0; i < pointsSize; i++) {
        fprintf(fp, "%.3f %.3f %.3f\n", point->x, point->y, point->z);
        point++;
    }

    fflush(fp);
    fclose(fp);
}

// Save the depth map in png format
void saveDepth(std::shared_ptr<ob::DepthFrame> depthFrame, int index)
{
    std::vector<int> compression_params;
    compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(0);
    compression_params.push_back(cv::IMWRITE_PNG_STRATEGY);
    compression_params.push_back(cv::IMWRITE_PNG_STRATEGY_DEFAULT);
    std::string depthName = "Depth_" + std::to_string(depthFrame->width()) + "x" + std::to_string(depthFrame->height()) + "_" + std::to_string(index) + "_"
        + std::to_string(depthFrame->timeStamp()) + "ms.png";
    cv::Mat depthMat(depthFrame->height(), depthFrame->width(), CV_16UC1, depthFrame->data());
    cv::imwrite(depthName, depthMat, compression_params);
    std::cout << "Depth saved:" << depthName ;
}

// Save the color image in png format
void saveColor(std::shared_ptr<ob::ColorFrame> colorFrame, int index)
{
    std::vector<int> compression_params;
    compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(0);
    compression_params.push_back(cv::IMWRITE_PNG_STRATEGY);
    compression_params.push_back(cv::IMWRITE_PNG_STRATEGY_DEFAULT);
    std::string colorName = "Color_" + std::to_string(colorFrame->width()) + "x" + std::to_string(colorFrame->height()) + "_" + std::to_string(index) + "_"
        + std::to_string(colorFrame->timeStamp()) + "ms.png";
    cv::Mat colorRawMat(colorFrame->height(), colorFrame->width(), CV_8UC3, colorFrame->data());
    cv::imwrite(colorName, colorRawMat, compression_params);
    std::cout << "Color saved:" << colorName ;
}

void GetRGBDAndColor()
{
    try {
        // create pipeline
        ob::Pipeline pipeline;
        // Configure which streams to enable or disable for the Pipeline by creating a Config
        std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();

        int colorCount = 0;
        int depthCount = 0;
        try {
            // Get all stream profiles of the color camera, including stream resolution, frame rate, and frame format
            auto  colorProfiles = pipeline.getStreamProfileList(OB_SENSOR_COLOR);
            std::shared_ptr<ob::VideoStreamProfile> colorProfile = nullptr;
            if (colorProfiles)
            {
                colorProfile = std::const_pointer_cast<ob::StreamProfile>(colorProfiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
            }
            config->enableStream(colorProfile);
        }
        catch (ob::Error& e)
        {
            // no Color Sensor
            colorCount = -1;
            std::cerr << "Current device is not support color sensor!" ;
        }

        // Get all stream profiles of the depth camera, including stream resolution, frame rate, and frame format
        auto  depthProfiles = pipeline.getStreamProfileList(OB_SENSOR_DEPTH);
        std::shared_ptr<ob::VideoStreamProfile> depthProfile = nullptr;
        if (depthProfiles)
        {
            depthProfile = std::const_pointer_cast<ob::StreamProfile>(depthProfiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
        }
        config->enableStream(depthProfile);

        // Create a format conversion Filter
        ob::FormatConvertFilter formatConvertFilter;

        // Start the pipeline with config
        pipeline.start(config);

        int frameCount = 0;
        while (true) 
        {
            // Wait for up to 100ms for a frameset in blocking mode.
            auto frameset = pipeline.waitForFrames(100);
            if (frameset == nullptr)
            {
                qDebug() << "The frameset is null!" ;
                continue;
            }

            // Filter the first 5 frames of data, and save it after the data is stable
            if (frameCount < 5)
            {
                frameCount++;
                continue;
            }

            // Get color and depth frames
            auto colorFrame = frameset->colorFrame();
            auto depthFrame = frameset->depthFrame();

            if (colorFrame != nullptr && colorCount < 5)
            {
                // save the colormap
                if (colorFrame->format() != OB_FORMAT_RGB)
                {
                    if (colorFrame->format() == OB_FORMAT_MJPG)
                    {
                        formatConvertFilter.setFormatConvertType(FORMAT_MJPG_TO_RGB);
                    }
                    else if (colorFrame->format() == OB_FORMAT_UYVY)
                    {
                        formatConvertFilter.setFormatConvertType(FORMAT_UYVY_TO_RGB);
                    }
                    else if (colorFrame->format() == OB_FORMAT_YUYV)
                    {
                        formatConvertFilter.setFormatConvertType(FORMAT_YUYV_TO_RGB);
                    }
                    else
                    {
                        qDebug() << "Color format is not support!" ;
                        continue;
                    }
                    colorFrame = formatConvertFilter.process(colorFrame)->as<ob::ColorFrame>();
                }
                formatConvertFilter.setFormatConvertType(FORMAT_RGB_TO_BGR);
                colorFrame = formatConvertFilter.process(colorFrame)->as<ob::ColorFrame>();
                saveColor(colorFrame, colorCount);
                colorCount++;
            }

            if (depthFrame != nullptr && depthCount < 5)
            {
                // save the depth map
                saveDepth(depthFrame, depthCount);
                depthCount++;
            }

            // Press the ESC key to exit the program when both the color image and the depth image are saved 5
            if (depthCount == 5 && (colorCount == 5 || colorCount == -1))
            {
                // stop the pipeline
                pipeline.stop();
                qDebug() << "The demo is over, please press ESC to exit manually!" ;
            }
        }

        return;
    }
    catch (ob::Error& e)
    {
        std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() ;
        return;
    }
}

#define KEY_ESC 27
#define KEY_R 82
#define KEY_r 114

// Save point cloud data to ply
//void savePointsToPly(std::shared_ptr<ob::Frame> frame, std::string fileName) {
//    int   pointsSize = frame->dataSize() / sizeof(OBPoint);
//    FILE *fp         = fopen(fileName.c_str(), "wb+");
//    fprintf(fp, "ply\n");
//    fprintf(fp, "format ascii 1.0\n");
//    fprintf(fp, "element vertex %d\n", pointsSize);
//    fprintf(fp, "property float x\n");
//    fprintf(fp, "property float y\n");
//    fprintf(fp, "property float z\n");
//    fprintf(fp, "end_header\n");

//    OBPoint *point = (OBPoint *)frame->data();
//    for(int i = 0; i < pointsSize; i++) {
//        fprintf(fp, "%.3f %.3f %.3f\n", point->x, point->y, point->z);
//        point++;
//    }

//    fflush(fp);
//    fclose(fp);
//}

// Save colored point cloud data to ply
void saveRGBPointsToPly(std::shared_ptr<ob::Frame> frame, std::string fileName) {
    int   pointsSize = frame->dataSize() / sizeof(OBColorPoint);
    FILE* fp;
    fopen_s(&fp,fileName.c_str(), "wb+");
    fprintf(fp, "ply\n");
    fprintf(fp, "format ascii 1.0\n");
    fprintf(fp, "element vertex %d\n", pointsSize);
    fprintf(fp, "property float x\n");
    fprintf(fp, "property float y\n");
    fprintf(fp, "property float z\n");
    fprintf(fp, "property uchar red\n");
    fprintf(fp, "property uchar green\n");
    fprintf(fp, "property uchar blue\n");
    fprintf(fp, "end_header\n");

    OBColorPoint *point = (OBColorPoint *)frame->data();
    for(int i = 0; i < pointsSize; i++) {
        fprintf(fp, "%.3f %.3f %.3f %d %d %d\n", point->x, point->y, point->z, (int)point->r, (int)point->g, (int)point->b);
        point++;
    }

    fflush(fp);
    fclose(fp);
}

// ===== 常驻相机 Pipeline =====
// 之前每次拍照都在 obCapture 里 new 一个 Pipeline 并 start()，Orbbec 冷启动 pipeline
// 每次约 1.6s。这里改为全局只初始化并启动一次，之后拍照只 waitForFrames()。
namespace {
std::shared_ptr<ob::Pipeline>            g_pipeline;
std::shared_ptr<ob::Config>              g_config;
std::shared_ptr<ob::PointCloudFilter>    g_pointCloud;
std::shared_ptr<ob::FormatConvertFilter> g_formatConvert;
bool                                     g_sensorStarted = false;

// 初始化并常驻启动相机 pipeline，只执行一次
bool ensureSensorStarted()
{
    if(g_sensorStarted) {
        return true;
    }

    ob::Context::setLoggerSeverity(OB_LOG_SEVERITY_WARN);

    g_pipeline = std::make_shared<ob::Pipeline>();
    g_config   = std::make_shared<ob::Config>();

    std::shared_ptr<ob::VideoStreamProfile> colorProfile = nullptr;
    try {
        // Get all stream profiles of the color camera, including stream resolution, frame rate, and frame format
        auto colorProfiles = g_pipeline->getStreamProfileList(OB_SENSOR_COLOR);
        if(colorProfiles) {
            auto profile = colorProfiles->getProfile(OB_PROFILE_DEFAULT);
            colorProfile = profile->as<ob::VideoStreamProfile>();
        }
        g_config->enableStream(colorProfile);
    }
    catch(ob::Error &e) {
        g_config->setAlignMode(ALIGN_DISABLE);
        std::cerr << "Current device is not support color sensor!";
    }

    // Get all stream profiles of the depth camera, including stream resolution, frame rate, and frame format
    std::shared_ptr<ob::StreamProfileList> depthProfileList;
    OBAlignMode                            alignMode = ALIGN_DISABLE;
    if(colorProfile) {
        depthProfileList = g_pipeline->getD2CDepthProfileList(colorProfile, ALIGN_D2C_HW_MODE);
        if(depthProfileList->count() > 0) {
            alignMode = ALIGN_D2C_HW_MODE;
        }
        else {
            depthProfileList = g_pipeline->getD2CDepthProfileList(colorProfile, ALIGN_D2C_SW_MODE);
            if(depthProfileList->count() > 0) {
                alignMode = ALIGN_D2C_SW_MODE;
            }
        }

        try {
            // Enable frame synchronization
            g_pipeline->enableFrameSync();
        }
        catch(ob::Error &e) {
            std::cerr << "Current device is not support frame sync!";
        }
    }
    else {
        depthProfileList = g_pipeline->getStreamProfileList(OB_SENSOR_DEPTH);
    }

    if(depthProfileList->count() > 0) {
        std::shared_ptr<ob::StreamProfile> depthProfile;
        try {
            if(colorProfile) {
                depthProfile = depthProfileList->getVideoStreamProfile(OB_WIDTH_ANY, OB_HEIGHT_ANY, OB_FORMAT_ANY, colorProfile->fps());
            }
        }
        catch(...) {
            depthProfile = nullptr;
        }

        if(!depthProfile) {
            depthProfile = depthProfileList->getProfile(OB_PROFILE_DEFAULT);
        }
        g_config->enableStream(depthProfile);
    }
    g_config->setAlignMode(alignMode);
    qDebug() << "[CAM] colorProfile w/h/fps/fmt:" << (colorProfile ? colorProfile->width() : 0)
             << (colorProfile ? colorProfile->height() : 0)
             << (colorProfile ? colorProfile->fps() : 0)
             << (colorProfile ? (int)colorProfile->format() : -1)
             << " alignMode:" << (int)alignMode
             << " depthProfileCount:" << (depthProfileList ? (int)depthProfileList->count() : -1);

    // 只启动一次，之后常驻运行
    g_pipeline->start(g_config);

    // 预热:丢弃前面几帧,等自动曝光/白平衡稳定。
    // 否则刚 start 后第一帧图像偏暗,模型检测不到穴位点(实测会出现 0 kpts)。
    for(int i = 0; i < 5; ++i) {
        try {
            auto warmupFrames = g_pipeline->waitForFrames(200);
            (void)warmupFrames;
        }
        catch(...) {
            break;
        }
    }

    // Create a point cloud Filter object (the device parameters will be obtained inside the Pipeline when the point cloud filter is created, so try to
    // configure the device before creating the filter)
    g_pointCloud = std::make_shared<ob::PointCloudFilter>();

    // get camera intrinsic and extrinsic parameters form pipeline and set to point cloud filter
    auto cameraParam = g_pipeline->getCameraParam();
    g_pointCloud->setCameraParam(cameraParam);

    g_formatConvert = std::make_shared<ob::FormatConvertFilter>();

    g_sensorStarted = true;
    qDebug() << "[CAM] persistent pipeline started";
    return true;
}
} // namespace

int obCapture(cv::Mat &colorRawMat,std::vector<OBColorPoint>& pointCloud_frame_data)
try {
    QElapsedTimer perfTimer;
    perfTimer.start();

    // 首次调用时初始化并启动相机，之后直接复用常驻 pipeline
    if(!ensureSensorStarted()) {
        qDebug() << "[CAM] ensureSensorStarted failed";
        return -1;
    }
    qDebug() << "[PERF][obCapture] 0.ensureSensorStarted:" << perfTimer.restart() << "ms";

    auto frameset = g_pipeline->waitForFrames(1000);
    int waitTry = 0;
    while((frameset == nullptr || frameset->colorFrame() == nullptr || frameset->depthFrame() == nullptr) && waitTry < 10) {
        waitTry++;
        qDebug() << "[CAM] frameset retry" << waitTry
                 << " frameset:" << (frameset != nullptr)
                 << " color:" << (frameset != nullptr && frameset->colorFrame() != nullptr)
                 << " depth:" << (frameset != nullptr && frameset->depthFrame() != nullptr);
        frameset = g_pipeline->waitForFrames(1000);
    }
    qDebug() << "[PERF][obCapture] 1.waitForFrames:" << perfTimer.restart() << "ms";

    if(frameset != nullptr && frameset->depthFrame() != nullptr && frameset->colorFrame() != nullptr) {
        // point position value multiply depth value scale to convert uint to millimeter (for some devices, the default depth value uint is not
        // millimeter)
        auto depthValueScale = frameset->depthFrame()->getValueScale();
        g_pointCloud->setPositionDataScaled(depthValueScale);
        try {
            // Generate a colored point cloud and save it
            qDebug() << "Save RGBD PointCloud ply file..." ;
            g_pointCloud->setCreatePointFormat(OB_FORMAT_RGB_POINT);
            pointCloud_frame = g_pointCloud->process(frameset);
            qDebug() << "[PERF][obCapture] 2.pointCloud.process:" << perfTimer.restart() << "ms";

            std::shared_ptr<ob::ColorFrame> colorFrame = frameset->colorFrame();
            qDebug() << "colorFrame->height():" << colorFrame->height() << colorFrame->width()
                     << colorFrame->format() << colorFrame->type() ;

            if(colorFrame->format() != OB_FORMAT_RGB) {
                if(colorFrame->format() == OB_FORMAT_MJPG) {
                    g_formatConvert->setFormatConvertType(FORMAT_MJPG_TO_RGB);
                }
                else if(colorFrame->format() == OB_FORMAT_UYVY) {
                    g_formatConvert->setFormatConvertType(FORMAT_UYVY_TO_RGB);
                }
                else if(colorFrame->format() == OB_FORMAT_YUYV) {
                    g_formatConvert->setFormatConvertType(FORMAT_YUYV_TO_RGB);
                }
                else {
                    qDebug() << "Color format is not support!" ;
                }
                colorFrame = g_formatConvert->process(colorFrame)->as<ob::ColorFrame>();
            }
            g_formatConvert->setFormatConvertType(FORMAT_RGB_TO_BGR);
            colorFrame = g_formatConvert->process(colorFrame)->as<ob::ColorFrame>();

            // 只做一次深拷贝。原实现先 _memccpy 到 temp 再 copyTo，随后又被
            // cv::Mat(...,colorFrame->data()) 浅包装覆盖(ptrs 在函数返回后失效)，属于悬挂指针隐患。
            colorRawMat = cv::Mat(colorFrame->height(), colorFrame->width(), CV_8UC3, colorFrame->data()).clone();
            qDebug() << "[PERF][obCapture] 3.formatConvert:" << perfTimer.restart() << "ms";

            int pointsSize = pointCloud_frame->dataSize() / sizeof(OBColorPoint);
            // 一次性拷贝，避免逐点 push_back(原实现还多自增一次导致整体错位一个点)
            OBColorPoint* pts = (OBColorPoint*)pointCloud_frame->data();
            pointCloud_frame_data.assign(pts, pts + pointsSize);
            qDebug() << "[PERF][obCapture] 4.copyLoop:" << perfTimer.restart() << "ms";

            qDebug() << "Vector size is "<< pointCloud_frame_data.size();
            qDebug() << "RGBPoints.ply Saved" ;
        }
        catch(std::exception &e) {
            qDebug() << "Get point cloud failed" ;
        }
        return 0;
    }
    else {
        qDebug() << "Get color frame or depth frame failed!" ;
    }

    return 0;
}

catch(ob::Error &e) {
    std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() ;
    return -1;
}

