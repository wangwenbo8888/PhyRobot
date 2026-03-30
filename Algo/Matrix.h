#pragma once

#include "Point3D.h"
#include "Axis3.h"
#include <iostream>
using namespace std;

/*************************************************************************
定义结构体Matrix4，表示4X4矩阵。具体对应关系见以下：
LM_transf.mat[0][0] = m_matrix[0];
LM_transf.mat[0][1] = m_matrix[1];
LM_transf.mat[0][2] = m_matrix[2];
LM_transf.mat[1][0] = m_matrix[4];
LM_transf.mat[1][1] = m_matrix[5];
LM_transf.mat[1][2] = m_matrix[6];
LM_transf.mat[2][0] = m_matrix[8];
LM_transf.mat[2][1] = m_matrix[9];
LM_transf.mat[2][2] = m_matrix[10];
LM_transf.shift[0]  = m_matrix[3];
LM_transf.shift[1]  = m_matrix[7];
LM_transf.shift[2]  = m_matrix[11];
**************************************************************************/
class Matrix4 
{
public:
	 Matrix4();
	 Matrix4(const Matrix4& m) ;
	 ~Matrix4();
	 //判断是否单位化
	 bool isUnit();
	//单位化
	void setUnit(); 
	//设置矩阵数组的值
	void setValue(int i,double val);
	//设置矩阵数组一行的值
	void SetOneRowValue(int row, double a0, double a1, double a2, double a3);
	//获取矩阵数组中值
	double getValue(int i) const;
	//设置平移的空间3个方向上移动（正负表示矢量方向，数值表示移动距离）
	void setTranslation(double x, double y, double z);
	//绕轴旋转某个角度值(不是弧度)
	void getTransformByRotateAxis(const Point3D base, const Point3D axis, double angle);

	Matrix4 operator+(const Matrix4&); 
	Matrix4 operator-(const Matrix4&); 
	Matrix4 operator*(const Matrix4&); 
	Point3D operator*(const Point3D&);
	Matrix4 operator/(Matrix4&); //右除 （x=b/a是方程x*a=b的解） x = b*a的逆矩阵
	Matrix4 LeftDivide(Matrix4&); //左除 （x=a\b是方程a*x =b的解）x = a的逆矩阵*b
	Matrix4& operator=(const Matrix4&); 
	Matrix4& operator+=(const Matrix4&); 
	Matrix4& operator-=(const Matrix4&); 
	friend ostream& operator<<(ostream&, const Matrix4&);
	Matrix4 operator[](int); //取某行，对操作符[]重载
	Matrix4 operator()(int); //取某列，对操作符()重载


	bool operator == (const Matrix4& m);
	bool operator != (const Matrix4& m);

	Matrix4 DateMult(double); //数乘
	Matrix4 Rotate(); //转秩
	Matrix4 Inverse(); //逆矩阵
	Matrix4 Gauss(Matrix4& A);
	double detMatrix(const Matrix4); //求方阵的行列式值

	//从当前坐标系axis3转到世界坐标系下的转化矩阵
	Matrix4 MakeMatrix4(const Axis3& axis3);

	//从fromAxis3坐标系转化到toAxis3坐标系
	Matrix4 MakeMatrix4(const Axis3& fromAxis3, const Axis3& toAxis3);


private:
	double m_v[4][4];
	int row; 
	int column; 
	double* pt; 
};

/*************************************************************************
*函数名称：	XJ_transformPoint
*函数功能：	移动点（平移或者旋转）
*输入参数： mtx------------------转换矩阵
			pt-------------------------原始点
*输出参数： res------------------------目标点
*************************************************************************/
extern "C" void XJ_transformPoint(const Matrix4 &mtx, const Point3D & pt, Point3D & res);

/*************************************************************************
*函数名称：	XJ_transformVector
*函数功能：	转换矢量
*输入参数： mtx------------------转换矩阵
			v--------------------------原始矢量
*输出参数： res------------------------目标矢量
*************************************************************************/
extern "C" void XJ_transformVector(Matrix4 &mtx, const Point3D & v, Point3D & res); 




