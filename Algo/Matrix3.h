#ifndef _CMATRIX3_H_
#define _CMATRIX3_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iomanip>
#include <iostream>
using namespace std;

#define CM3	(3)			//	矩阵维数	
#define MINTOL	(0.00001)	//	最小公差

class CMatrix3
{
public:
	CMatrix3();									//	无参数的构造函数
	~CMatrix3();									//	析构函数声明
	CMatrix3(const double a[CM3*CM3]);				//	一维数组

	//*****************************************************
	// Method:    CXJMatrix3	构造函数
	// Para<I>: double x[CM3]	已知第一矢量
	// Para<I>: double y[CM3]	已知第二矢量
	// Para<I>: int iType		0 已知XY 1 已知XZ 2 已知YZ
	//*****************************************************
	CMatrix3(const double x[CM3],const double y[CM3],int iType = 0);	//	已知2个矢量构造

	CMatrix3(const double a[CM3][CM3]);				//	有参数的构造函数
	CMatrix3(CMatrix3 &b);						//	有参数的构造函数
	CMatrix3 Add(CMatrix3 &b);					//	加法运算声明
	CMatrix3 Sub(CMatrix3 &b);					//	减法运算声明
	CMatrix3 Mul(CMatrix3 &b);					//	乘法运算声明
	CMatrix3 Div(CMatrix3 &b);					//	除法运算声明
	CMatrix3 Inverse();							//	求逆运算声明	
	CMatrix3 Transpose();							//	转置运算
	void display();									//	显示函数声明
	
	inline void GetVecX(double x[CM3]);			//	获取X方向矢量
	inline void GetVecY(double y[CM3]);			//	获取Y方向矢量
	inline void GetVecZ(double z[CM3]);			//	获取Z方向矢量
	void GetXYZ(double xyz[CM3*CM3]);			//	获取Z方向矢量

	inline void SetVecX(const double x[CM3]);			//	修改X方向矢量
	inline void SetVecY(const double y[CM3]);			//	修改Y方向矢量
	inline void SetVecZ(const double z[CM3]);			//	修改Z方向矢量

	//	2个矢量 叉乘 计算与2个矢量垂直的矢量
	static void Vec3Cross(const double V1[CM3],const double V2[CM3],double VDir[CM3]);

	//	2个矢量 点乘
	static double Vec3Dot(const double V1[CM3],const double V2[CM3]);

	//	计算2个矢量之间的夹角 返回值为弧度 
	static double GetAngle(const double V1[CM3],const double V2[CM3]);

	static double GetSmallAngle(double ang);
	//  矢量 乘以 矩阵 矩阵变换
	void Mul_Vec3(const double vecs[CM3],double vect[CM3]);
	void Mul_Vec3_Left(const double vecs[CM3],double vect[CM3]);
	//赋值运算
	CMatrix3& operator=(const CMatrix3& v);
	//比较运算
	bool operator==(const CMatrix3& v);
	//矢量运算
	CMatrix3 operator+( CMatrix3& v);
	CMatrix3 operator-( CMatrix3& v);
	CMatrix3 operator*(double a);
	CMatrix3 operator*(CMatrix3& v);
private:
	double m[CM3][CM3];							//	矩阵设置为私有的

private:
	bool Gauss(const double A[][CM3], double B[][CM3], int n=CM3);	//求逆运算声明
};

#endif