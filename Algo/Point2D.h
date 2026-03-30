#ifndef _POINT2D_
#define _POINT2D_

#include "Common.h"
#include <math.h>


//2维点
class Point2D
{
public:
	Point2D() : _x(0.0), _y(0.0){}
	~Point2D() {}

	typedef double Standard_Real;

	Point2D(Standard_Real X, Standard_Real Y) : _x(X), _y(Y){}

	inline Standard_Real& x() {return _x;}
	inline Standard_Real& y() {return _y;}
	inline Standard_Real x() const { return _x; }
	inline Standard_Real y() const { return _y; }

	bool IsZero() {return (xjdef::equivalent(_x, 0.0) && xjdef::equivalent(_y, 0.0));}

	void set(Standard_Real X, Standard_Real Y) {_x = X; _y = Y;}

	inline Point2D& operator = (const Point2D& rhs)
	{
		set(rhs.x(), rhs.y());
		return *this;
	}

	//加法运算
	inline Point2D operator + (const Point2D& rhs) const
	{
		return Point2D(_x + rhs.x(), _y + rhs.y());
	}

	inline Point2D& operator += (const Point2D& rhs)
	{
		_x += rhs.x();
		_y += rhs.y();
		return *this;
	}

	//减法运算
	inline Point2D operator - ( const Point2D& rhs) const	 //后面加const，则对于const 对象也能使用-操作符，否则编译不通过
	{
		return Point2D(_x-rhs.x(), _y-rhs.y());
	}

	inline Point2D& operator -= (const Point2D& rhs)
	{
		_x -= rhs.x();
		_y -= rhs.y();
		return *this;
	}

	//乘法运算
	inline double operator * (const Point2D& rhs) const 
	{
		return _x*rhs.x() + _y*rhs.y();
	}

	inline Point2D operator * (double rhs) const
	{
		return Point2D(_x*rhs, _y*rhs);
	}

	inline Point2D& operator *= (double rhs)
	{
		_x *= rhs;
		_y *= rhs;
		return *this;
	}

	inline Point2D operator / (double rhs) const
	{
		return Point2D(_x/rhs, _y/rhs);
	}

	inline bool operator == (const Point2D& rhs) const {return _x == rhs.x() && _y == rhs.y();}

	//求二维向量的模
	inline double Magnitude() const
	{
		return sqrt(_x * _x + _y * _y);
	}

	inline void Normalize()
	{
		double length = Magnitude();
		if(length <= 0.0 && length >= 1.0)
			return;

		_x = _x / length;
		_y = _y / length;
	}

	//求二维空间中的两点之间的距离
	double Distance(const Point2D& other) const;

	//二维点到二维线上的投影点
	Point2D Perpendicular(Point2D lineBase, Point2D linedir) const;


private:
	Standard_Real _x;
	Standard_Real _y;

};


//global functions

/// Multiplication of scalar with vector.
inline Point2D operator * (float rhs, const Point2D& rvs)
{
	return Point2D(rvs.x()*rhs, rvs.y()*rhs);
}

#endif