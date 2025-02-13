

#include "CameraGrabber.h"

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
        auto                                    depthProfiles = pipeline.getStreamProfileList(OB_SENSOR_DEPTH);
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
//void saveRGBPointsToPly(std::shared_ptr<ob::Frame> frame, std::string fileName) {
//    int   pointsSize = frame->dataSize() / sizeof(OBColorPoint);
//    FILE *fp         = fopen(fileName.c_str(), "wb+");
//    fprintf(fp, "ply\n");
//    fprintf(fp, "format ascii 1.0\n");
//    fprintf(fp, "element vertex %d\n", pointsSize);
//    fprintf(fp, "property float x\n");
//    fprintf(fp, "property float y\n");
//    fprintf(fp, "property float z\n");
//    fprintf(fp, "property uchar red\n");
//    fprintf(fp, "property uchar green\n");
//    fprintf(fp, "property uchar blue\n");
//    fprintf(fp, "end_header\n");

//    OBColorPoint *point = (OBColorPoint *)frame->data();
//    for(int i = 0; i < pointsSize; i++) {
//        fprintf(fp, "%.3f %.3f %.3f %d %d %d\n", point->x, point->y, point->z, (int)point->r, (int)point->g, (int)point->b);
//        point++;
//    }

//    fflush(fp);
//    fclose(fp);
//}

int obCapture(cv::Mat &colorRawMat, OBColorPoint* point) try {
    ob::Context::setLoggerSeverity(OB_LOG_SEVERITY_WARN);
    // create pipeline
    ob::Pipeline pipeline;

    // Configure which streams to enable or disable for the Pipeline by creating a Config
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();

    // Turn on D2C alignment, which needs to be turned on when generating RGBD point clouds

    std::shared_ptr<ob::VideoStreamProfile> colorProfile = nullptr;
    try {
        // Get all stream profiles of the color camera, including stream resolution, frame rate, and frame format
        auto colorProfiles = pipeline.getStreamProfileList(OB_SENSOR_COLOR);
        if(colorProfiles) {
            auto profile = colorProfiles->getProfile(OB_PROFILE_DEFAULT);
            colorProfile = profile->as<ob::VideoStreamProfile>();
        }
        config->enableStream(colorProfile);
    }
    catch(ob::Error &e) {
        config->setAlignMode(ALIGN_DISABLE);
        std::cerr << "Current device is not support color sensor!" ;
    }

    // Get all stream profiles of the depth camera, including stream resolution, frame rate, and frame format
    std::shared_ptr<ob::StreamProfileList> depthProfileList;
    OBAlignMode                            alignMode = ALIGN_DISABLE;
    if(colorProfile) {
        // Try find supported depth to color align hardware mode profile
        depthProfileList = pipeline.getD2CDepthProfileList(colorProfile, ALIGN_D2C_HW_MODE);
        if(depthProfileList->count() > 0) {
            alignMode = ALIGN_D2C_HW_MODE;
        }
        else {
            // Try find supported depth to color align software mode profile
            depthProfileList = pipeline.getD2CDepthProfileList(colorProfile, ALIGN_D2C_SW_MODE);
            if(depthProfileList->count() > 0) {
                alignMode = ALIGN_D2C_SW_MODE;
            }
        }

        try {
            // Enable frame synchronization
            pipeline.enableFrameSync();
        }
        catch(ob::Error &e) {
            std::cerr << "Current device is not support frame sync!" ;
        }
    }
    else {
        depthProfileList = pipeline.getStreamProfileList(OB_SENSOR_DEPTH);
    }

    if(depthProfileList->count() > 0) {
        std::shared_ptr<ob::StreamProfile> depthProfile;
        try {
            // Select the profile with the same frame rate as color.
            if(colorProfile) {
                depthProfile = depthProfileList->getVideoStreamProfile(OB_WIDTH_ANY, OB_HEIGHT_ANY, OB_FORMAT_ANY, colorProfile->fps());
            }
        }
        catch(...) {
            depthProfile = nullptr;
        }

        if(!depthProfile) {
            // If no matching profile is found, select the default profile.
            depthProfile = depthProfileList->getProfile(OB_PROFILE_DEFAULT);
        }
        config->enableStream(depthProfile);
    }
    config->setAlignMode(alignMode);

    // start pipeline with config
    pipeline.start(config);

    // Create a point cloud Filter object (the device parameters will be obtained inside the Pipeline when the point cloud filter is created, so try to
    // configure the device before creating the filter)
    ob::PointCloudFilter pointCloud;

    // get camera intrinsic and extrinsic parameters form pipeline and set to point cloud filter
    auto cameraParam = pipeline.getCameraParam();
    pointCloud.setCameraParam(cameraParam);

    int count = 0;
    int n = 20;
    while(n--) {
        auto frameset = pipeline.waitForFrames(100);
            int key = 'R';
            // Press the ESC key to exit
            if(key == KEY_ESC) {
                break;
            }
            if(key == 'R' || key == 'r') {
                count = 0;
                // Limit up to 10 repetitions
                while(count++ < 10) {
                    // Wait for a frame of data, the timeout is 100ms
                    auto frameset = pipeline.waitForFrames(100);
                    if(frameset != nullptr && frameset->depthFrame() != nullptr && frameset->colorFrame() != nullptr) {
                        // point position value multiply depth value scale to convert uint to millimeter (for some devices, the default depth value uint is not
                        // millimeter)
                        auto depthValueScale = frameset->depthFrame()->getValueScale();
                        pointCloud.setPositionDataScaled(depthValueScale);
                        try {
                            // Generate a colored point cloud and save it
                            qDebug() << "Save RGBD PointCloud ply file..." ;
                            pointCloud.setCreatePointFormat(OB_FORMAT_RGB_POINT);
                            std::shared_ptr<ob::Frame> frame = pointCloud.process(frameset);
                            std::shared_ptr<ob::ColorFrame> colorFrame = frameset->colorFrame();

                            colorRawMat = cv::Mat(colorFrame->height(), colorFrame->width(), CV_8UC3, colorFrame->data());
                            int   pointsSize = frame->dataSize() / sizeof(OBColorPoint);
                            point = (OBColorPoint*)frame->data();

                            qDebug() << "RGBPoints.ply Saved" ;
                        }
                        catch(std::exception &e) {
                            qDebug() << "Get point cloud failed" ;
                        }
                        break;
                    }
                    else {
                        qDebug() << "Get color frame or depth frame failed!" ;
                    }
                }
            }
            else if(key == 'D' || key == 'd') {
                count = 0;
                // Limit up to 10 repetitions
                while(count++ < 10) {
                    // Wait for up to 100ms for a frameset in blocking mode.
                    auto frameset = pipeline.waitForFrames(100);
                    if(frameset != nullptr && frameset->depthFrame() != nullptr) {
                        // point position value multiply depth value scale to convert uint to millimeter (for some devices, the default depth value uint is not
                        // millimeter)
                        auto depthValueScale = frameset->depthFrame()->getValueScale();
                        pointCloud.setPositionDataScaled(depthValueScale);
                        try {
                            // generate point cloud and save
                            qDebug() << "Save Depth PointCloud to ply file..." ;
                            pointCloud.setCreatePointFormat(OB_FORMAT_POINT);
                            std::shared_ptr<ob::Frame> frame = pointCloud.process(frameset);
                            savePointsToPly(frame, "DepthPoints.ply");
                            qDebug() << "DepthPoints.ply Saved" ;
                        }
                        catch(std::exception &e) {
                            qDebug() << "Get point cloud failed" ;
                        }
                        break;
                    }
                }
            }
        }

    // stop the pipeline
    pipeline.stop();

    return 0;
}

catch(ob::Error &e) {
    std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() ;
    exit(EXIT_FAILURE);
}

