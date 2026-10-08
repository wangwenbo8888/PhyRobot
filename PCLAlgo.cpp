
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

// 只对指定下标的点计算法线:邻域搜索仍用整幅点云(KdTree 只建一次),
// 但只输出 indices 中这些点的法线,结果按 indices 顺序存放。
// 相比对全部 92 万点算,可从数分钟降到毫秒级。
bool GetCloudsNormalsAt(const pcl::PointCloud<pcl::PointXYZ>::Ptr cloud,
                        const std::vector<int>& indices,
                        std::vector<pcl::Normal>& normals)
{
	normals.clear();
	if(!cloud || cloud->empty() || indices.empty()) {
		return false;
	}

	pcl::IndicesPtr idx(new pcl::Indices(indices));

	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
	ne.setInputCloud(cloud);
	ne.setIndices(idx);
	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>);
	ne.setSearchMethod(tree);
	ne.setKSearch(10);

	pcl::PointCloud<pcl::Normal>::Ptr out(new pcl::PointCloud<pcl::Normal>);
	ne.compute(*out);

	normals.assign(out->begin(), out->end());
	return true;
}
