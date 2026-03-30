#pragma once

#include <pcl/point_types.h>
//#include <pcl/io/pcd_io.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/features/normal_3d_omp.h>
//#include <pcl/visualization/pcl_visualizer.h>
#include <boost/thread/thread.hpp>

void GetCloudsNormals(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud, pcl::PointCloud<pcl::Normal>::Ptr& normals);
