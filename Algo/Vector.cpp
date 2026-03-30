#include "Vector.h"


/*************************************************************************
*函数名称：	VectorDot
*函数功能：	矢量点乘
*输入参数： a--------------------第一个矢量
			b--------------------第二个矢量
*返回值：   返回点乘积
*************************************************************************/
double VectorDot(const Point3D &a, const Point3D &b)
{
	return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}
/*************************************************************************
*函数名称：	VectorCross
*函数功能：	矢量叉乘
*输入参数： a--------------------第一个矢量
			b--------------------第二个矢量
			res------------------结果矢量
*************************************************************************/
void VectorCross(const Point3D &a, const Point3D &b, Point3D &res)
{
	res.data[0] = a.y() * b.z() - a.z() * b.y();
	res.data[1] = a.z() * b.x() - a.x() * b.z();
	res.data[2] = a.x() * b.y() - a.y() * b.x();
	//Normalized(res);
}
/*************************************************************************
*函数名称：	Normalized
*函数功能：	矢量单位化
*输入参数： a--------------------输入参数
*输出参数： a--------------------输出参数
*************************************************************************/
bool Normalized(Point3D &a)
{
	double mag = sqrt(a.x() * a.x() + a.y() * a.y() + a.z() * a.z());
	// 输入参数为0 时，自动转为（1.0,0.0，0.0）
	if(mag == 0.0f)
	{
		a.data[0] = 1.0;
		return false;
	}
	double invMag = 1.0f / mag;
	a.data[0] *= invMag;
	a.data[1] *= invMag;
	a.data[2] *= invMag;
	return true;
}
/***************************************************************************
*函数名称： GetAngle
*函数功能： 计算两向量的角度（弧度值）
*输入参数： V1-------------------第一个矢量
*输入参数： V2-------------------第二个矢量
*返回值  ： double---------------两矢量之间的弧度值
****************************************************************************/
double GetAngle(const Point3D V1,const Point3D V2)
{
	double n1 = V1.Length(); 
	double n2 = V2.Length();
	double val = n1 * n2;
	if(fabs(val) < XJ_SMALL)
	{
		return 0;
	}

	double result =  V1.dot(V2)/val;
	if(result > 1.0)
	{
		result = 1.0;
	}
	else if(result < -1.0)
	{
		result = -1.0;
	}

	return acos(result);


//	Point3D dir1 = V1;
//	Point3D dir2 = V2;
//	dir1.Normalize();
//	dir2.Normalize();
//	double res = dir1.dot(dir2);
//	if(res > 1.0)
//	{
//		res = 1.0;
//	}
//	else if(res < -1.0)
//	{
//		res = -1.0;
//	}
//	
//	double rad = acos(res);
//	return rad;
}

int GetAngleLineWithPlane(Point3D lineDir,Point3D linrBace,Point3D planeDir, Point3D planeBace, double& resultAngle)
{
	if (lineDir.IsZero() || planeDir.IsZero())
		return -1;

	lineDir.Normalize();
	planeDir.Normalize();

	double TempAngle = GetAngle(lineDir, planeDir);
	if (TempAngle > Pi*0.5)
	{
		double reAngle = (TempAngle - Pi*0.5)*180/Pi;
		if (reAngle > 90.0 || fabs(reAngle-90.0)<0.1)
			resultAngle = 90.0;
		else if(fabs(reAngle)<XJ_SMALL)
			resultAngle = 0.0;
		else
			resultAngle = reAngle;
	}
	else
	{
		double reAngle = (Pi*0.5 - TempAngle)*180/Pi;
		if (reAngle > 90.0 || fabs(reAngle-90.0)<0.1)
			resultAngle = 90.0;
		else if(fabs(reAngle)<XJ_SMALL)
			resultAngle = 0.0;
		else
			resultAngle = reAngle;
	}
	return 0;
}

int GetAnglePlaneWithPlane(Point3D planeDir1,Point3D planeBace1,Point3D planeDir2, Point3D planeBace2, double& resultAngle)
{
	if (planeDir1.IsZero() || planeDir1.IsZero())
		return -1;

	planeDir1.Normalize();
	planeDir2.Normalize();

	double TempAngle = GetAngle(planeDir1, planeDir2)*180/Pi;

	if (TempAngle-90.0 > XJ_SMALL)
		resultAngle = 180-TempAngle;
	else if(TempAngle-90.0 < XJ_SMALL)
		resultAngle = TempAngle;
	else
		resultAngle = 90.0;

	return 0;
}

/***************************************************************************
*函数名称： TranslatePtByDir
*函数功能：	点沿特定方向移动一定距离
*point----------------点	I/O
*dir------------------方向  I
*distance-------------距离  I
****************************************************************************/
void TranslatePtByDir(Point3D& point, Point3D& dir, float distance)
{
	Normalized(dir);

	point.x() += dir.x() * distance;
	point.y() += dir.y() * distance;
	point.z() += dir.z() * distance;
}

int CalcTriNormal( Point3D& p0, Point3D& p1, Point3D& p2, Point3D &norm)
{
	Point3D xy = p1 - p0;
	Point3D xz = p2 - p0;

	VectorCross(xy, xz, norm);
	norm.Normalize();

	return 0;
}

#if 0
int CalcIntersectPointLineWithLine(Point3D p0, Point3D dir0, Point3D p1, Point3D dir1, Point3D& intersectPoint)
{
	if(dir0.IsZero() || dir1.IsZero())
		return -1;

	Vector P0(p0.x(), p0.y(), p0.z());
	Vector D0(dir0.x(), dir0.y(), dir0.z());
	Vector P1(p1.x(), p1.y(), p1.z());
	Vector D1(dir1.x(), dir1.y(), dir1.z());

	Vector InterP;
	if(!XJAL_LineIntersectWithLine(P0, D0, P1, D1, InterP))
		return -1;

	intersectPoint.set(InterP[0], InterP[1], InterP[2]);
	return 0;
}
#endif