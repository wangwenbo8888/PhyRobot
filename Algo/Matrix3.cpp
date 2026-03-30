#include "Matrix3.h"

CMatrix3::CMatrix3(void)
{
	int i,j;
	for(i=0;i<CM3;i++)
		for(j=0;j<CM3;j++)
		{
			if(i == j)
				m[i][j] = 1;
			else
				m[i][j] = 0;
		}
}

CMatrix3::CMatrix3(const double a[CM3][CM3])
{
	int i,j;
	for(i=0;i<CM3;i++)
		for(j=0;j<CM3;j++)
			m[i][j]=a[i][j];
}

CMatrix3::CMatrix3(CMatrix3 &b)
{
	int i,j;
	for(i=0;i<CM3;i++)
		for(j=0;j<CM3;j++)
			m[i][j]=b.m[i][j];
}

CMatrix3::CMatrix3(const double a[CM3*CM3])
{
	for (int i=0;i<CM3;i++)
		for(int j=0;j<CM3;j++)
			m[i][j]= a[i*CM3+j];
}

CMatrix3::CMatrix3(const double x[CM3],const double y[CM3],int iType/* = 0*/)
{
	double z[CM3] ={0,0,1};


	Vec3Cross(x,y,z);

	switch(iType)
	{	
	case 1:	//	X + Z （Y）
		for (int i=0;i<CM3;i++)
		{
			m[0][i]=x[i];
			m[1][i]=z[i];
			m[2][i]=y[i];
		}
		break;
	case 2:	//	Y + Z （X）
		for (int i=0;i<CM3;i++)
		{
			m[0][i]=z[i];
			m[1][i]=x[i];
			m[2][i]=y[i];
		}
		break;	
	case 3:	//	Z + X （Y）
		for (int i=0;i<CM3;i++)
		{
			m[0][i]=y[i];
			m[1][i]=z[i];
			m[2][i]=x[i];
		}
		break;	
	default:
		for (int i=0;i<CM3;i++)
		{
			m[0][i]=x[i];
			m[1][i]=y[i];
			m[2][i]=z[i];
		}
		break;
	}	
}

CMatrix3::~CMatrix3(void)
{
}

CMatrix3 CMatrix3::Add(CMatrix3 &b)//加法运算
{
	int i,j;
	CMatrix3*c=(CMatrix3*)malloc(sizeof(CMatrix3));
	for(i=0;i<CM3;i++)
		for(j=0;j<CM3;j++)
			c->m[i][j]=m[i][j]+b.m[i][j];
	return(*c);
}
CMatrix3 CMatrix3::Sub(CMatrix3 &b)//减法运算
{
	int i,j;
	CMatrix3 *c = new CMatrix3();
	for(i=0;i<CM3;i++)
		for(j=0;j<CM3;j++)
			c->m[i][j]=m[i][j]-b.m[i][j];
	CMatrix3 ret = *c;
	delete c;
	return ret;
}
CMatrix3 CMatrix3::Mul(CMatrix3 &b)//乘法运算
{
	int i,j,k;
	double sum=0;
	CMatrix3 *c = new CMatrix3();
	for(i=0;i<CM3;i++)
	{
		for(j=0;j<CM3;j++)
		{
			for(k=0;k<CM3;k++)
				sum+=m[i][k]*(b.m[k][j]);

			if (fabs(sum)<MINTOL)
				sum = 0;
			c->m[i][j]=sum;
			sum=0;
		}
	}
	CMatrix3 ret = *c;
	delete c;
	return ret;
}
//----------------------------------------------
//功能: 采用部分主元的高斯消去法求方阵A的逆矩阵B
//入口参数: 输入方阵，输出方阵，方阵阶数
//返回值: true or false
//----------------------------------------------
bool CMatrix3::Gauss(const double A[][CM3], double B[][CM3], int n)
{
	int i, j, k;
	double max, temp;
	double t[100][100];                //临时矩阵
	//将A矩阵存放在临时矩阵t[n][n]中
	for (i = 0; i < n; i++)        
	{
		for (j = 0; j < n; j++)
		{
			t[i][j] = A[i][j];
		}
	}
	//初始化B矩阵为单位阵
	for (i = 0; i < n; i++)        
	{
		for (j = 0; j < n; j++)
		{
			B[i][j] = (i == j) ? (float)1 : 0;
		}
	}
	for (i = 0; i < n; i++)
	{
		//寻找主元
		max = t[i][i];
		k = i;
		for (j = i+1; j < n; j++)
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
			for (j = 0; j < n; j++)
			{
				temp = t[i][j];
				t[i][j] = t[k][j];
				t[k][j] = temp;
				//B伴随交换
				temp = B[i][j];
				B[i][j] = B[k][j];
				B[k][j] = temp;
			}
		}
		//判断主元是否为0, 若是, 则矩阵A不是满秩矩阵,不存在逆矩阵
		if (t[i][i] == 0)
		{
			cout << "There is no inverse matrix!";
			return false;
		}
		//消去A的第i列除去i行以外的各行元素
		temp = t[i][i];
		for (j = 0; j < n; j++)
		{
			t[i][j] = t[i][j] / temp;        //主对角线上的元素变为1
			B[i][j] = B[i][j] / temp;        //伴随计算
		}
		for (j = 0; j < n; j++)        //第0行->第n行
		{
			if (j != i)                //不是第i行
			{
				temp = t[j][i];
				for (k = 0; k < n; k++)        //第j行元素 - i行元素*j列i行元素
				{
					t[j][k] = t[j][k] - t[i][k]*temp;
					B[j][k] = B[j][k] - B[i][k]*temp;
				}
			}
		}
	}
	return true;
}
CMatrix3 CMatrix3::Inverse()//求逆运算
{                  
	double ds[CM3][CM3];

	CMatrix3 *c= new CMatrix3();

	int i=0,j=0;
	if (Gauss(m,ds))
	{
		for(i=0;i<CM3;i++)           
			for(j=0;j<CM3;j++)        
				c->m[i][j]=ds[i][j];   
	}
	CMatrix3 ret = *c;
	delete c;
	return ret;
}

CMatrix3 CMatrix3::Transpose()//转置运算
{                  
	CMatrix3 *c= new CMatrix3(*this);

	double t;
	int i=0,j=0;
	for(i=0;i<CM3;i++)           
	{
		for(j=i;j<CM3;j++)        
		{
			if (j>i)
			{
				t = c->m[i][j];
				c->m[i][j] = c->m[j][i];
				c->m[j][i] = t;
			}
		}
	}
	CMatrix3 ret = *c;
	delete c;
	return ret;
}

void CMatrix3::display()
{
	int i,j;	
	cout << setiosflags(ios::fixed)<< setiosflags(ios_base::showpoint)<< setprecision(4);

	for(i=0;i<CM3;i++)
	{
		for(j=0;j<CM3;j++)
			cout << setw(10) <<m[i][j];
		cout<<endl;
	}
	cout<<"-------------------------------------"<<endl;
}

CMatrix3& CMatrix3::operator=(const CMatrix3& v)
{
	for(int i=0;i<CM3;i++)
		for(int j=0;j<CM3;j++)
			m[i][j] = v.m[i][j];
	return *this;
}

bool CMatrix3::operator==(const CMatrix3& v)
{
	for(int i=0;i<CM3;i++)
		for(int j=0;j<CM3;j++)
		{
			if (fabs(m[i][j] - v.m[i][j])>MINTOL)
				return false;
		}
	return true;
}

CMatrix3 CMatrix3::operator+(CMatrix3& v)
{
	for(int i=0;i<CM3;i++)
		for(int j=0;j<CM3;j++)
			m[i][j] += v.m[i][j];
	return *this;
}

CMatrix3 CMatrix3::operator-(CMatrix3& v)
{
	for(int i=0;i<CM3;i++)
		for(int j=0;j<CM3;j++)
			m[i][j] -= v.m[i][j];
	return *this;
}

CMatrix3 CMatrix3::operator*(CMatrix3& v)
{
	int i,j,k;
	double sum=0;
	CMatrix3 *c = new CMatrix3();
	for(i=0;i<CM3;i++)
	{
		for(j=0;j<CM3;j++)
		{
			for(k=0;k<CM3;k++)
				sum+=m[i][k]*(v.m[k][j]);

			if (fabs(sum)<MINTOL)
				sum = 0;
			c->m[i][j]=sum;
			sum=0;
		}
	}
	CMatrix3 ret = *c;
	delete c;
	return ret;
}

CMatrix3 CMatrix3::operator*(double a)
{
	for(int i=0;i<CM3;i++)
		for(int j=0;j<CM3;j++)
			m[i][j] *= a;
	return *this;
}

inline void CMatrix3::GetVecX(double x[CM3])
{
	for (int i=0;i<CM3;i++)
		x[i]=m[0][i];
}

inline void CMatrix3::GetVecY(double y[CM3])
{
	for (int i=0;i<CM3;i++)
		y[i]=m[1][i];
}

inline void CMatrix3::GetVecZ(double z[CM3])
{
	for (int i=0;i<CM3;i++)
		z[i]=m[2][i];
}

inline void CMatrix3::SetVecX(const double x[CM3])
{
	for (int i=0;i<CM3;i++)
		m[0][i]=x[i];
}

inline void CMatrix3::SetVecY(const double y[CM3])
{
	for (int i=0;i<CM3;i++)
		m[1][i]=y[i];
}

inline void CMatrix3::SetVecZ(const double z[CM3])
{
	for (int i=0;i<CM3;i++)
		m[2][i] = z[i];
}

void CMatrix3::Vec3Cross(const double V1[CM3],const double V2[CM3],double VDir[CM3])
{
	VDir[0] = V1[1]*V2[2] - V1[2]*V2[1];
	VDir[1] = -(V1[0]*V2[2] - V1[2]*V2[0]);
	VDir[2] = V1[0]*V2[1] - V1[1]*V2[0];
}

void CMatrix3::Mul_Vec3(const double vecs[CM3],double vect[CM3])
{
	double sum = 0;
	double ds[3]={0};
	for (int i=0;i<CM3;i++)
	{
		sum = 0;
		for (int j=0;j<CM3;j++)
			sum += vecs[j]*m[j][i];
		if (fabs(sum)<MINTOL)
			sum = 0;
		ds[i] = sum;
	}
	for (int k=0;k<CM3;k++)
		vect[k] = ds[k];
}

void CMatrix3::Mul_Vec3_Left(const double vecs[CM3],double vect[CM3])
{
	double sum = 0;
	double ds[3]={0};
	for (int i=0;i<CM3;i++)
	{
		sum = 0;
		for (int j=0;j<CM3;j++)
			sum += m[i][j]*vecs[j];

		ds[i] = sum;
	}

	for (int k=0;k<CM3;k++)
		vect[k] = ds[k];
}

void CMatrix3::GetXYZ(double xyz[CM3*CM3])
{
	int i,j;
	for(i=0;i<CM3;i++)
		for(j=0;j<CM3;j++)
			xyz[i*3+j]=m[i][j];
}

double CMatrix3::Vec3Dot(const double V1[CM3],const double V2[CM3])
{
	return V1[0]*V2[0] +V1[1]*V2[1]+V1[2]*V2[2];
}

double CMatrix3::GetAngle(const double V1[CM3],const double V2[CM3])
{
	double M1 = sqrt(V1[0]*V1[0]+V1[1]*V1[1]+V1[2]*V1[2]);
	double M2 = sqrt(V2[0]*V2[0]+V2[1]*V2[1]+V2[2]*V2[2]);
	double d = CMatrix3::Vec3Dot(V1,V2)/(M1 * M2);
	return acos(d);	
}

double CMatrix3::GetSmallAngle(double ang)
{
	double dNewAngle = fabs(ang);

	dNewAngle = (dNewAngle>180)?(dNewAngle-180):(dNewAngle);

	if (dNewAngle >45 && dNewAngle<=90)
	{
		dNewAngle = 90 - dNewAngle;
	}
	else if (dNewAngle >90 && dNewAngle<=135)
	{
		dNewAngle = dNewAngle -90;
	}
	else if (dNewAngle>135 &&dNewAngle<=180)
	{
		dNewAngle = 180 - dNewAngle;
	}
	return dNewAngle;
}
