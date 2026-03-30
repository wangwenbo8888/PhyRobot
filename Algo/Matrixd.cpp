//此矩阵类放置osg::Matrix，OGS中的矩阵是行矩阵，
//平移向量位于第四行，那么就是向量坐标在左，变换矩阵在右，一直右乘

#include "Matrixd.h"

#define SET_ROW(row, v1, v2, v3, v4 )    \
	_mat[(row)][0] = (v1); \
	_mat[(row)][1] = (v2); \
	_mat[(row)][2] = (v3); \
	_mat[(row)][3] = (v4);

#define INNER_PRODUCT(a,b,r,c) \
	((a)._mat[r][0] * (b)._mat[0][c]) \
	+((a)._mat[r][1] * (b)._mat[1][c]) \
	+((a)._mat[r][2] * (b)._mat[2][c]) \
	+((a)._mat[r][3] * (b)._mat[3][c])



void Matrixd::makeIdentity()
{
	SET_ROW(0,    1, 0, 0, 0 )
	SET_ROW(1,    0, 1, 0, 0 )
	SET_ROW(2,    0, 0, 1, 0 )
	SET_ROW(3,    0, 0, 0, 1 )
}


void Matrixd::makeScale( const Point3D& v)
{
	makeScale(v.x(), v.y(), v.z());
}


void Matrixd::makeScale( value_type x, value_type y, value_type z )
{
	SET_ROW(0,    x, 0, 0, 0 )
	SET_ROW(1,    0, y, 0, 0 )
	SET_ROW(2,    0, 0, z, 0 )
	SET_ROW(3,    0, 0, 0, 1 )
}

void Matrixd::makeTranslate( const Point3D& v)
{
	makeTranslate(v.x(), v.y(), v.z());
}

void Matrixd::makeTranslate( value_type x, value_type y, value_type z )
{
	SET_ROW(0,    1, 0, 0, 0 )
	SET_ROW(1,    0, 1, 0, 0 )
	SET_ROW(2,    0, 0, 1, 0 )
	SET_ROW(3,    x, y, z, 1 )
}

bool Matrixd::invert_4x3( const Matrixd& mat )
{
	if (&mat==this)
	{
		Matrixd tm(mat);
		return invert_4x3(tm);
	}

	register value_type r00, r01, r02,
		r10, r11, r12,
		r20, r21, r22;
	// Copy rotation components directly into registers for speed
	r00 = mat._mat[0][0]; r01 = mat._mat[0][1]; r02 = mat._mat[0][2];
	r10 = mat._mat[1][0]; r11 = mat._mat[1][1]; r12 = mat._mat[1][2];
	r20 = mat._mat[2][0]; r21 = mat._mat[2][1]; r22 = mat._mat[2][2];

	// Partially compute inverse of rot
	_mat[0][0] = r11*r22 - r12*r21;
	_mat[0][1] = r02*r21 - r01*r22;
	_mat[0][2] = r01*r12 - r02*r11;

	// Compute determinant of rot from 3 elements just computed
	register value_type one_over_det = 1.0/(r00*_mat[0][0] + r10*_mat[0][1] + r20*_mat[0][2]);
	r00 *= one_over_det; r10 *= one_over_det; r20 *= one_over_det;  // Saves on later computations

	// Finish computing inverse of rot
	_mat[0][0] *= one_over_det;
	_mat[0][1] *= one_over_det;
	_mat[0][2] *= one_over_det;
	_mat[0][3] = 0.0;
	_mat[1][0] = r12*r20 - r10*r22; // Have already been divided by det
	_mat[1][1] = r00*r22 - r02*r20; // same
	_mat[1][2] = r02*r10 - r00*r12; // same
	_mat[1][3] = 0.0;
	_mat[2][0] = r10*r21 - r11*r20; // Have already been divided by det
	_mat[2][1] = r01*r20 - r00*r21; // same
	_mat[2][2] = r00*r11 - r01*r10; // same
	_mat[2][3] = 0.0;
	_mat[3][3] = 1.0;

	// We no longer need the rxx or det variables anymore, so we can reuse them for whatever we want.  But we will still rename them for the sake of clarity.

#define d r22
	d  = mat._mat[3][3];

	if( (d-1.0)*(d-1.0) > 1.0e-6 )  // Involves perspective, so we must
	{                       // compute the full inverse

		Matrixd TPinv;
		_mat[3][0] = _mat[3][1] = _mat[3][2] = 0.0;

#define px r00
#define py r01
#define pz r02
#define one_over_s  one_over_det
#define a  r10
#define b  r11
#define c  r12

		a  = mat._mat[0][3]; b  = mat._mat[1][3]; c  = mat._mat[2][3];
		px = _mat[0][0]*a + _mat[0][1]*b + _mat[0][2]*c;
		py = _mat[1][0]*a + _mat[1][1]*b + _mat[1][2]*c;
		pz = _mat[2][0]*a + _mat[2][1]*b + _mat[2][2]*c;

#undef a
#undef b
#undef c
#define tx r10
#define ty r11
#define tz r12

		tx = mat._mat[3][0]; ty = mat._mat[3][1]; tz = mat._mat[3][2];
		one_over_s  = 1.0/(d - (tx*px + ty*py + tz*pz));

		tx *= one_over_s; ty *= one_over_s; tz *= one_over_s;  // Reduces number of calculations later on

		// Compute inverse of trans*corr
		TPinv._mat[0][0] = tx*px + 1.0;
		TPinv._mat[0][1] = ty*px;
		TPinv._mat[0][2] = tz*px;
		TPinv._mat[0][3] = -px * one_over_s;
		TPinv._mat[1][0] = tx*py;
		TPinv._mat[1][1] = ty*py + 1.0;
		TPinv._mat[1][2] = tz*py;
		TPinv._mat[1][3] = -py * one_over_s;
		TPinv._mat[2][0] = tx*pz;
		TPinv._mat[2][1] = ty*pz;
		TPinv._mat[2][2] = tz*pz + 1.0;
		TPinv._mat[2][3] = -pz * one_over_s;
		TPinv._mat[3][0] = -tx;
		TPinv._mat[3][1] = -ty;
		TPinv._mat[3][2] = -tz;
		TPinv._mat[3][3] = one_over_s;

		preMult(TPinv); // Finish computing full inverse of mat

#undef px
#undef py
#undef pz
#undef one_over_s
#undef d
	}
	else // Rightmost column is [0; 0; 0; 1] so it can be ignored
	{
		tx = mat._mat[3][0]; ty = mat._mat[3][1]; tz = mat._mat[3][2];

		// Compute translation components of mat'
		_mat[3][0] = -(tx*_mat[0][0] + ty*_mat[1][0] + tz*_mat[2][0]);
		_mat[3][1] = -(tx*_mat[0][1] + ty*_mat[1][1] + tz*_mat[2][1]);
		_mat[3][2] = -(tx*_mat[0][2] + ty*_mat[1][2] + tz*_mat[2][2]);

#undef tx
#undef ty
#undef tz
	}

	return true;

}

template <class T>
inline T SGL_ABS(T a)
{
	return (a >= 0 ? a : -a);
}

#ifndef SGL_SWAP
#define SGL_SWAP(a,b,temp) ((temp)=(a),(a)=(b),(b)=(temp))
#endif

bool Matrixd::invert_4x4( const Matrixd& mat )
{
	if (&mat == this) {
		Matrixd tm(mat);
		return invert_4x4(tm);
	}

	unsigned int indxc[4], indxr[4], ipiv[4];
	unsigned int i,j,k,l,ll;
	unsigned int icol = 0;
	unsigned int irow = 0;
	double temp, pivinv, dum, big;

	// copy in place this may be unnecessary
	*this = mat;

	for (j=0; j<4; j++) ipiv[j]=0;

	for(i=0;i<4;i++)
	{
		big=0.0;
		for (j=0; j<4; j++)
			if (ipiv[j] != 1)
				for (k=0; k<4; k++)
				{
					if (ipiv[k] == 0)
					{
						if (SGL_ABS(operator()(j,k)) >= big)
						{
							big = SGL_ABS(operator()(j,k));
							irow=j;
							icol=k;
						}
					}
					else if (ipiv[k] > 1)
						return false;
				}
				++(ipiv[icol]);
				if (irow != icol)
					for (l=0; l<4; l++) SGL_SWAP(operator()(irow,l),
						operator()(icol,l),
						temp);

				indxr[i]=irow;
				indxc[i]=icol;
				if (operator()(icol,icol) == 0)
					return false;

				pivinv = 1.0/operator()(icol,icol);
				operator()(icol,icol) = 1;
				for (l=0; l<4; l++) operator()(icol,l) *= pivinv;
				for (ll=0; ll<4; ll++)
					if (ll != icol)
					{
						dum=operator()(ll,icol);
						operator()(ll,icol) = 0;
						for (l=0; l<4; l++) operator()(ll,l) -= operator()(icol,l)*dum;
					}
	}
	for (int lx=4; lx>0; --lx)
	{
		if (indxr[lx-1] != indxc[lx-1])
			for (k=0; k<4; k++) SGL_SWAP(operator()(k,indxr[lx-1]),
				operator()(k,indxc[lx-1]),temp);
	}

	return true;

}

void Matrixd::mult( const Matrixd& lhs, const Matrixd& rhs )
{
	if (&lhs==this)
	{
		postMult(rhs);
		return;
	}
	if (&rhs==this)
	{
		preMult(lhs);
		return;
	}

	// PRECONDITION: We assume neither &lhs nor &rhs == this
	// if it did, use preMult or postMult instead
	_mat[0][0] = INNER_PRODUCT(lhs, rhs, 0, 0);
	_mat[0][1] = INNER_PRODUCT(lhs, rhs, 0, 1);
	_mat[0][2] = INNER_PRODUCT(lhs, rhs, 0, 2);
	_mat[0][3] = INNER_PRODUCT(lhs, rhs, 0, 3);
	_mat[1][0] = INNER_PRODUCT(lhs, rhs, 1, 0);
	_mat[1][1] = INNER_PRODUCT(lhs, rhs, 1, 1);
	_mat[1][2] = INNER_PRODUCT(lhs, rhs, 1, 2);
	_mat[1][3] = INNER_PRODUCT(lhs, rhs, 1, 3);
	_mat[2][0] = INNER_PRODUCT(lhs, rhs, 2, 0);
	_mat[2][1] = INNER_PRODUCT(lhs, rhs, 2, 1);
	_mat[2][2] = INNER_PRODUCT(lhs, rhs, 2, 2);
	_mat[2][3] = INNER_PRODUCT(lhs, rhs, 2, 3);
	_mat[3][0] = INNER_PRODUCT(lhs, rhs, 3, 0);
	_mat[3][1] = INNER_PRODUCT(lhs, rhs, 3, 1);
	_mat[3][2] = INNER_PRODUCT(lhs, rhs, 3, 2);
	_mat[3][3] = INNER_PRODUCT(lhs, rhs, 3, 3);
}

void Matrixd::preMult( const Matrixd& other)
{
	// brute force method requiring a copy
	//Matrix_implementation tmp(other* *this);
	// *this = tmp;

	// more efficient method just use a value_type[4] for temporary storage.
	value_type t[4];
	for(int col=0; col<4; ++col) {
		t[0] = INNER_PRODUCT( other, *this, 0, col );
		t[1] = INNER_PRODUCT( other, *this, 1, col );
		t[2] = INNER_PRODUCT( other, *this, 2, col );
		t[3] = INNER_PRODUCT( other, *this, 3, col );
		_mat[0][col] = t[0];
		_mat[1][col] = t[1];
		_mat[2][col] = t[2];
		_mat[3][col] = t[3];
	}

}

void Matrixd::postMult( const Matrixd& other)
{
	// brute force method requiring a copy
	//Matrix_implementation tmp(*this * other);
	// *this = tmp;

	// more efficient method just use a value_type[4] for temporary storage.
	value_type t[4];
	for(int row=0; row<4; ++row)
	{
		t[0] = INNER_PRODUCT( *this, other, row, 0 );
		t[1] = INNER_PRODUCT( *this, other, row, 1 );
		t[2] = INNER_PRODUCT( *this, other, row, 2 );
		t[3] = INNER_PRODUCT( *this, other, row, 3 );
		SET_ROW(row, t[0], t[1], t[2], t[3] )
	}
}

void Matrixd::setTrans( value_type tx, value_type ty, value_type tz )
{
	_mat[3][0] = tx;
	_mat[3][1] = ty;
	_mat[3][2] = tz;
}

void Matrixd::setTrans( const Point3D& v )
{
	_mat[3][0] = v.x();
	_mat[3][1] = v.y();
	_mat[3][2] = v.z();
}

void Matrixd::makeMatrix( const Axis3& axis3 )
{
	SET_ROW(0, axis3.XDirection().x(), axis3.XDirection().y(), axis3.XDirection().z(), 0);
	SET_ROW(1, axis3.YDirection().x(), axis3.YDirection().y(), axis3.YDirection().z(), 0);
	SET_ROW(2, axis3.ZDirection().x(), axis3.ZDirection().y(), axis3.ZDirection().z(), 0);
	SET_ROW(3, axis3.Location().x(), axis3.Location().y(), axis3.Location().z(), 1);
}


void Matrixd::makeReverseMatrix( const Axis3& axis3 )
{
	Matrixd mat;
	mat.makeMatrix(axis3);
	Matrixd matInverse = inverse(matInverse);

	SET_ROW(0, matInverse(0,0), matInverse(0,1), matInverse(0,2), matInverse(0,3));
	SET_ROW(1, matInverse(1,0), matInverse(1,1), matInverse(1,2), matInverse(1,3));
	SET_ROW(2, matInverse(2,0), matInverse(2,1), matInverse(2,2), matInverse(2,3));
	SET_ROW(3,matInverse(3,0), matInverse(3,1), matInverse(3,2), matInverse(3,3));
}

void Matrixd::makeMatrix( const Axis3& fromAxis3, const Axis3& toAxis3 )
{
	//先从fromAxis3转化到世界坐标系
	Matrixd m1;
	m1.makeMatrix(fromAxis3);

	Matrixd m1Inverse = inverse(m1);

	Matrixd m2;
	m2.makeMatrix(toAxis3);

	//Matrixd m2Inverse = inverse(m2);
	
	m1Inverse.postMult(m2);

	SET_ROW(0, m1Inverse(0,0), m1Inverse(0,1), m1Inverse(0,2), m1Inverse(0,3));
	SET_ROW(1, m1Inverse(1,0), m1Inverse(1,1), m1Inverse(1,2), m1Inverse(1,3));
	SET_ROW(2, m1Inverse(2,0), m1Inverse(2,1), m1Inverse(2,2), m1Inverse(2,3));
	SET_ROW(3, m1Inverse(3,0), m1Inverse(3,1), m1Inverse(3,2), m1Inverse(3,3));
}

Point3D Matrixd::operator*( const Point3D& v ) const
{
	return postMult(v);
}
