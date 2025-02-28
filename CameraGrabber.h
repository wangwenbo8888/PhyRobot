#pragma once

#include "libobsensor/ObSensor.hpp"
//#include "utils.hpp"
#include <QDebug>


#include <fstream>
#include <iostream>

#include "libobsensor/ObSensor.hpp"
// #include "opencv2/opencv.hpp"
#include <fstream>
#include <iostream>
#include "Utils.hpp"
#include "inference.h"

// 获取RGBD图和彩色图
void GetRGBDAndColor();
int obCapture(cv::Mat &colorRawMat,std::vector<OBColorPoint>& pointCloud_frame_data);

std::vector<cv::Point3d> get3Dpoints (std::vector<cv::Point> base);

static std::shared_ptr<ob::Frame> pointCloud_frame;
static OBColorPoint* Colorpoint;



