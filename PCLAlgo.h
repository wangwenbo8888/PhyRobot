#pragma once

#include <pcl/point_types.h>
//#include <pcl/io/pcd_io.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/search/kdtree.h>
#include <pcl/features/normal_3d_omp.h>   // 同时提供 NormalEstimation / NormalEstimationOMP
//#include <pcl/visualization/pcl_visualizer.h>
#include <boost/thread/thread.hpp>
#include <vector>

// 对整幅点云计算法线(很慢,92万点约需数分钟,尽量别用)
void GetCloudsNormals(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud, pcl::PointCloud<pcl::Normal>::Ptr& normals);

// 只对指定下标的点计算法线(邻域搜索仍用整幅点云做 KdTree),
// 用于穴位点等少量点,避免对全云计算导致卡死。
// 结果按传入 indices 的顺序保存到 normals 中。
bool GetCloudsNormalsAt(const pcl::PointCloud<pcl::PointXYZ>::Ptr cloud,
                        const std::vector<int>& indices,
                        std::vector<pcl::Normal>& normals);
