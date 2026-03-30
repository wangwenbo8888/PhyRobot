#pragma once
#include "Vector.h"
#include "Matrix.h"
#include "Common.h"

Matrix4::Matrix4()
{
	row = 4;
	column = 4;
	pt = &(m_v[0][0]);
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
		{
			if(i ==j)
				pt[i*4+j] =1.0;
			else
				pt[i*4+j] =0.0;
		}
}

//拷贝构造函数 
Matrix4::Matrix4(const Matrix4& m)
{
	row = m.row; 
	column = m.column; 
	pt = &(m_v[0][0]);
	const double * pt_temp = &(m.m_v[0][0]);
	for(int i=0; i<row*column; i++) 
		pt[i] = pt_temp[i]; 
}

Matrix4::~Matrix4()
{
	if(pt != NULL) 
	{ 
		pt = NULL; 
	} 
}

void Matrix4::setUnit()
{
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
		{
			if(i ==j)
				pt[i*4+j] =1.0;
			else
				pt[i*4+j] =0.0;
		}
}

bool Matrix4::isUnit()
{
	int count = 0;
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
		{
			if(i == j)
			{	
				if(pt[i*4+j] == 1.0)
					count++;
			}
			else
			{
				if(pt[i*4+j] == 0.0)
					count++;
			}
		}
	if(count == 16)
		return true;
	else
		return false;
}

void Matrix4::setTranslation(double x, double y, double z)
{
	pt[3] = x;
	pt[7] = y;
	pt[11] = z;
}

void Matrix4::setValue(int i, double val)
{
	 m_v[(int)(i/4)][i%4] = val;
}

double Matrix4::getValue(int i) const
{
	double ret = m_v[(int)(i/4)][i%4];
	return ret;
}

void Matrix4::SetOneRowValue(int row, double a0, double a1, double a2,double a3)
{
	if (row>3 || row<0)
		return;

	m_v[row][0] = a0;
	m_v[row][1] = a1;
	m_v[row][2] = a2;
	m_v[row][3] = a3;
}
	
/*************************************************************************
*函数名称：	XJ_multiplyTransforms
*函数功能：	绕轴旋转
*输入参数： base-----------------------基点
			axis-----------------------基准轴
			angle----------------------旋转角度
*************************************************************************/
void Matrix4::getTransformByRotateAxis(const Point3D base, const Point3D axis, double angle)
{ 
	Point3D vect = axis;
    Normalized(vect);
    double u = vect.x();
    double v = vect.y();
    double w = vect.z();
    double a = base.x();
    double b = base.y();
    double c = base.z();
	
	double 	Radian = angle*XJPI/180;

    pt[0] = u*u+(v*v+w*w)*cos(Radian);
	pt[1] = u*v*(1.0-cos(Radian))-w*sin(Radian);
	pt[2] = u*w*(1.0-cos(Radian))+v*sin(Radian);
	pt[3] = (a*(v*v+w*w)-u*(b*v+c*w))*(1-cos(Radian))+(b*w-c*v)*sin(Radian);

	pt[4] = u*v*(1.0-cos(Radian))+w*sin(Radian);
	pt[5] = v*v+(u*u+w*w)*cos(Radian);
	pt[6] = v*w*(1.0-cos(Radian))-u*sin(Radian);
	pt[7] = (b*(u*u+w*w)-v*(a*u+c*w))*(1.0-cos(Radian))+(c*u-a*w)*sin(Radian);

	pt[8] = u*w*(1.0-cos(Radian))-v*sin(Radian);
	pt[9] = v*w*(1.0-cos(Radian))+u*sin(Radian);
	pt[10] = w*w+(u*u+v*v)*cos(Radian);
	pt[11] = (c*(u*u+v*v)-w*(a*u+b*v))*(1.0-cos(Radian))+(a*v-b*u)*sin(Radian);

	pt[12] = 0.0;
	pt[13] = 0.0;
	pt[14] = 0.0;
	pt[15] = 1.0;
}

Matrix4 Matrix4::operator+(const Matrix4& m) 
{ 
	if(row != m.row || column != m.column) 
		exit(0); 
	Matrix4 temp; 
	for(int i=0; i<row*column; i++) 
		temp.pt[i] = pt[i] + m.pt[i]; 
	return temp; 
} 

Matrix4 Matrix4:: operator-(const Matrix4& m) 
{ 
	if(row != m.row || column != m.column) 
		exit(0); 
	Matrix4 temp;  
	for(int i=0; i<row*column; i++) 
		temp.pt[i] = pt[i] - m.pt[i]; 
	return temp; 
} 

Point3D Matrix4::operator*(const Point3D& v)
{
	return Point3D(m_v[0][0]*v.x() + m_v[0][1]*v.y() + m_v[0][2]*v.z() + m_v[0][3],
				   m_v[1][0]*v.x() + m_v[1][1]*v.y() + m_v[1][2]*v.z() + m_v[1][3],
				   m_v[2][0]*v.x() + m_v[2][1]*v.y() + m_v[2][2]*v.z() + m_v[2][3]);
}


Matrix4 Matrix4::operator*(const Matrix4& m) 
{ 
	if(column != m.row) 
		exit(0); 
	Matrix4 temp;
	memset(temp.pt, 0, sizeof(double)*row*m.column); 
	
	//for (int i = 0;i<4;++i)
	//{
	//	for (int j = 0;j<4;++j)
	//	{
	//		temp.pt[i*4+j] =0.0;
	//		for (int k = 0;k<4;++k)
	//			temp.pt[i*4+j] += pt[i*4+k]*m.pt[k*4+j];
	//	}
	//}
	///*
	int k, count, temp1; 
	k = count = temp1 = 0; 
	for(int i=0; i<row; i++) 
	{ 
		while(k<m.column) 
		{ 
			for(int j=i*column; j<(i+1)*column; j++) 
			{ 
				temp.pt[count] += pt[j] * m.pt[temp1]; 
				temp1 += m.column; 
			} 
			k++; 
			temp1 = k; 
			count++; 
		} 
		temp1 = 0; 
		k = 0; 
	} 
	return temp; 
} 

Matrix4& Matrix4::operator=(const Matrix4& m) 
{ 
	row = m.row; 
	column = m.column; 
	const double * pt_temp = &(m.m_v[0][0]);
	for(int i=0; i<row*column; i++) 
		pt[i] = pt_temp[i]; 
	return *this; 
} 

Matrix4 Matrix4::operator/(Matrix4& m) 
{ 
	return(*this)* (m.Inverse()); 
}	
bool Matrix4::operator == (const Matrix4& m)
{
	for(int i=0; i<row*column; i++) 
	{
		if(pt[i] - m.pt[i] > XJ_TOL)
			return false;
	}
	return true;		
}
bool Matrix4::operator != (const Matrix4& m)
{
	for(int i=0; i<row*column; i++) 
	{
		if(pt[i] - m.pt[i] > XJ_TOL)
			return true;
	}
	return false;		
}
Matrix4 Matrix4::LeftDivide(Matrix4& m) 
{
	return(*this).Inverse()*m;
}

Matrix4& Matrix4::operator+=(const Matrix4& m) 
{ 
	if(row != m.row || column != m.column) 
		exit(0); 
	for(int i=0; i<row*column; i++) 
		pt[i] += m.pt[i]; 
	return *this; 
} 

Matrix4& Matrix4::operator-=(const Matrix4& m) 
{ 
	if(row != m.row || column != m.column) 
		exit(0); 
	for(int i=0; i<row*column; i++) 
		pt[i] -= m.pt[i]; 
	return *this; 
} 


ostream& operator<<(ostream& o, const Matrix4& m)//overload the cout
{ 
	o<<"Print the matrix as follows:"<<endl; 
	for(int i=0; i<m.row * m.column; i++) 
	{ 
		if((i+1) % m.column == 0) 
			o<<m.pt[i]<<endl; 

		else
			o<<m.pt[i]<<""; 
	}

	return o; 
} 

void XJ_transformPoint(const Matrix4& mtx, const Point3D& pt, Point3D& res)
{
	Point3D temp = pt;
	for(int i = 0; i < 3; i++) //row
	{ 
		res.data[i] = mtx.getValue(i*4) * temp.data[0] + mtx.getValue(i*4 + 1) * temp.data[1] 
		+ mtx.getValue(i*4 + 2) * temp.data[2] + mtx.getValue(i*4 + 3);
	} 
}

void XJ_transformVector(Matrix4 &mtx, const Point3D & v, Point3D & res)
{
	Point3D temp = v;
	for(int i=0; i<3; i++) //row
	{ 
		res.data[i] = mtx.getValue(i*4) * temp.data[0] + mtx.getValue(i*4 + 1) * temp.data[1] 
		+ mtx.getValue(i*4 + 2) * temp.data[2];
	} 

	Normalized(res);
}

Matrix4 Matrix4::Gauss(Matrix4& A)
{
	int i, j, k;
	double max, temp;
	double t[5][5];  
	Matrix4 B;
	//临时矩阵
	//将A矩阵存放在临时矩阵t[n][n]中
	for (i = 0; i < row; i++)        
	{
		for (j = 0; j < row; j++)
		{
			t[i][j] = A.m_v[i][j];
		}
	}
	//初始化B矩阵为单位阵
	B.setUnit();
	for (i = 0; i < row; i++)
	{
		//寻找主元
		max = t[i][i];
		k = i;
		for (j = i+1; j < row; j++)
		{
			if (fabs(t[j][i]) > fabs(max))
			{
				max = t[j][i];
				k = j;
			}
		}
		//如果主元所在行不是第i行，进行行交换
		if (k != i)
		{
			for (j = 0; j < row; j++)
			{
				temp = t[i][j];
				t[i][j] = t[k][j];
				t[k][j] = temp;
				//B伴随交换
				temp = B.m_v[i][j];
				B.m_v[i][j] =  B.m_v[k][j];
			     B.m_v[k][j] = temp;
			}
		}
		//判断主元是否为0, 若是, 则矩阵A不是满秩矩阵,不存在逆矩阵
		if (t[i][i] == 0)
		{
			return B;
		}
		//消去A的第i列除去i行以外的各行元素
		temp = t[i][i];
		for (j = 0; j < row; j++)
		{
			t[i][j] = t[i][j]/temp;        //主对角线上的元素变为1
			B.m_v[i][j] =  B.m_v[i][j]/temp;        //伴随计算
		}
		for (j = 0; j < row; j++)        //第0行->第n行
		{
			if (j != i)                //不是第i行
			{
				temp = t[j][i];
				for (k = 0; k < row; k++)        //第j行元素 - i行元素*j列i行元素
				{
					t[j][k] = t[j][k] - t[i][k]*temp;
					B.m_v[j][k] = B.m_v[j][k] -  B.m_v[i][k]*temp;
				}
			}
		}
	}
	return B;
}

Matrix4 Matrix4::Inverse() 
{ 
	Matrix4 val;
	val = Gauss(*this);
	return val;
}

Matrix4 Matrix4::Rotate() 
{ 
	Matrix4 temp; 
	for(int i=0; i<row; i++) 
		for(int j=0; j<column; j++) 
			temp.pt[j*row+i] = pt[i*column+j]; 
	return temp; 
} 

Matrix4 Matrix4::DateMult(double num) 
{ 
	Matrix4 temp;  
	for(int i=0; i<row*column; i++) 
		temp.pt[i] = num * pt[i]; 
	return temp; 
} 

double Matrix4::detMatrix(const Matrix4 M) 
{ 
	if(M.row != M.column) 
		exit(0); 
	int m, n, s, t, k=1; 
	double c, x, sn, f=1; 
	for(int i=0,j=0; i<M.row&&j<M.row; i++,j++) 
	{ 
		if(M.pt[i*M.column+j] == 0) 
		{ 
			for(m=i; M.pt[m*M.column+j]==0; m++); 
			if(m == M.row) 
			{ 
				sn = 0; 
				return sn; 
			} 
			else
				for(n=j; n<M.row; n++) 
				{ 
					c = M.pt[i*M.column+n]; 
					M.pt[i*M.column+n] = M.pt[m*M.column+n]; 
					M.pt[m*M.column+n] = c; 
				} 
				k *= (-1); 
		} 
		for(s=M.row-1; s>i; s--) 
		{ 
			x = M.pt[s*M.column+j]; 
			for(t=j; t<M.row; t++) 
				M.pt[s*M.column+t] -= M.pt[i*M.column+t]*(x / M.pt[i*M.column+j]); 
		} 
	} 
	for(int i=0; i<M.row; i++) 
		f *= M.pt[i*M.column+i]; 
	sn = k * f; 
	return sn; 
} 

//将axis3坐标系变化到与world坐标系重合
Matrix4 Matrix4::MakeMatrix4(const Axis3& axis3)
{
	//创建列矩阵
	SetOneRowValue(0, axis3.XDirection().x(), axis3.YDirection().x(), axis3.ZDirection().x(), axis3.Location().x());
	SetOneRowValue(1, axis3.XDirection().y(), axis3.YDirection().y(), axis3.ZDirection().y(), axis3.Location().y());
	SetOneRowValue(2, axis3.XDirection().z(), axis3.YDirection().z(), axis3.ZDirection().z(), axis3.Location().z());
	SetOneRowValue(3, 0, 0, 0, 1);

	return *this;
}

Matrix4 Matrix4::MakeMatrix4(const Axis3& fromAxis3, const Axis3& toAxis3)
{
	//先从fromAxis3转化到世界坐标系
	Matrix4 m1;
	m1.MakeMatrix4(fromAxis3);
	Matrix4 reverseM1 = m1.Inverse();

	Matrix4 m2;
	m2.MakeMatrix4(toAxis3);

	*this = m2*reverseM1;

	return *this;
}



