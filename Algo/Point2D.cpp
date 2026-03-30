#include "Point2D.h"
#include "Point3D.h"


double Point2D::Distance( const Point2D& other )	const
{
	Point2D vec2(_x - other.x(), _y - other.y());
	double dis = vec2.Magnitude();

	return dis;
}


Point2D Point2D::Perpendicular( Point2D lineBase, Point2D linedir ) const
{
	Point3D pt(_x, _y, 0.0);

	Point3D lineBase3d(lineBase.x(), lineBase.y(), 0.0);
	Point3D lineDir3d(linedir.x(), linedir.y(), 0.0);

	Point3D res3d = pt.Perpedicular(lineBase3d, lineDir3d);

	return Point2D(res3d.x(), res3d.y());
}
