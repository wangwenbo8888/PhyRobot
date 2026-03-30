
#include "Point3D.h"

void Point3D::Cross( const Point3D& rhs )
{
	Point3D dir;
	dir.set(data[1]*rhs.z() - data[2]*rhs.y(),
		data[2]*rhs.x() - data[0]*rhs.z() ,
		data[0]*rhs.y() - data[1]*rhs.x());
	//dir.Normalize();

	data[0] = dir.x();
	data[1] = dir.y();
	data[2] = dir.z();
}


Point3D Point3D::Crossed( const Point3D& rhs ) const
{
	Point3D dir;
	dir.set(data[1]*rhs.z() - data[2]*rhs.y(),
		data[2]*rhs.x() - data[0]*rhs.z() ,
		data[0]*rhs.y() - data[1]*rhs.x());
	//dir.Normalize();

	return dir;
}


void Point3D::CrossCross( const Point3D& v1, const Point3D& v2 )
{
	double Xresult = 
		y() * (v1.x() * v2.y() - v1.y() * v2.x()) -
		z() * (v1.z() * v2.x() - v1.x() * v2.z());

	double Yresult = 
		z() * (v1.y() * v2.z() - v1.z() * v2.y()) -
		x() * (v1.x() * v2.y() - v1.y() * v2.x());

	double Zresult = 
		x() * (v1.z() * v2.x() - v1.x() * v2.z()) -
		y() * (v1.y() * v2.z() - v1.z() * v2.y());

	data[0] = Xresult;
	data[1] = Yresult;
	data[2] = Zresult;
}

Point3D Point3D::CrossCrossed( const Point3D& V1, const Point3D& V2 ) const
{
	Point3D V0 = *this;
	V0.CrossCross(V1, V2);
	V0.Normalize();
	return V0;
}

Point3D Point3D::Perpedicular( const Point3D& rclBase, const Point3D& rclDir ) const
{
	double t = ((*this - rclBase)*rclDir) / (rclDir*rclDir);
	return rclBase + rclDir * t;
}
