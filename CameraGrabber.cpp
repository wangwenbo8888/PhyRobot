

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
    std::cout << "Depth saved:" << depthName << std::endl;
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
    std::cout << "Color saved:" << colorName << std::endl;
}


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

    OBColorPoint* point = (OBColorPoint*)frame->data();
    for (int i = 0; i < pointsSize; i++) {
        fprintf(fp, "%.3f %.3f %.3f %d %d %d\n", point->x, point->y, point->z, (int)point->r, (int)point->g, (int)point->b);
        point++;
    }

    fflush(fp);
    fclose(fp);
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
            auto                                    colorProfiles = pipeline.getStreamProfileList(OB_SENSOR_COLOR);
            std::shared_ptr<ob::VideoStreamProfile> colorProfile = nullptr;
            if (colorProfiles) {
                colorProfile = std::const_pointer_cast<ob::StreamProfile>(colorProfiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
            }
            config->enableStream(colorProfile);
        }
        catch (ob::Error& e)
        {
            // no Color Sensor
            colorCount = -1;
            std::cerr << "Current device is not support color sensor!" << std::endl;
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
                std::cout << "The frameset is null!" << std::endl;
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
                        std::cout << "Color format is not support!" << std::endl;
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
                std::cout << "The demo is over, please press ESC to exit manually!" << std::endl;
            }
        }

        return;
    }
    catch (ob::Error& e)
    {
        std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
        return;
    }
}