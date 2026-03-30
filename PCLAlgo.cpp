
#include "PCLAlgo.h"

void GetCloudsNormals(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud, pcl::PointCloud<pcl::Normal>::Ptr& normals)
{
	//pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);

	pcl::NormalEstimationOMP<pcl::PointXYZ, pcl::Normal> n;
    normals.reset(new pcl::PointCloud<pcl::Normal>);
	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>);
	n.setNumberOfThreads(10);
	n.setInputCloud(cloud);
	n.setSearchMethod(tree);
	n.setKSearch(10);
	n.compute(*normals);
}