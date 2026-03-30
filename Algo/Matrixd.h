#ifndef _MATRIXD
#define _MATRIXD

#include "Point3D.h"
#include "Axis3.h"

class Matrixd
{
public:

	typedef double value_type;

	inline Matrixd(void) {makeIdentity();}
	~Matrixd(void) {}

	inline value_type& operator()(int row, int col) { return _mat[row][col]; }
	inline value_type operator()(int row, int col) const { return _mat[row][col]; }


	 void makeIdentity();

	 void makeScale(const Point3D&);
	 void makeScale( value_type, value_type, value_type );

	 void makeTranslate( const Point3D& );
	 void makeTranslate( value_type, value_type, value_type );

	 bool isIdentity() const
	 {
		 return _mat[0][0]==1.0 && _mat[0][1]==0.0 && _mat[0][2]==0.0 &&  _mat[0][3]==0.0 &&
			 _mat[1][0]==0.0 && _mat[1][1]==1.0 && _mat[1][2]==0.0 &&  _mat[1][3]==0.0 &&
			 _mat[2][0]==0.0 && _mat[2][1]==0.0 && _mat[2][2]==1.0 &&  _mat[2][3]==0.0 &&
			 _mat[3][0]==0.0 && _mat[3][1]==0.0 && _mat[3][2]==0.0 &&  _mat[3][3]==1.0;
	 }

	 /** invert the matrix rhs, automatically select invert_4x3 or invert_4x4. */
	 inline bool invert( const Matrixd& rhs)
	 {
		 bool is_4x3 = (rhs._mat[0][3]==0.0 && rhs._mat[1][3]==0.0 &&  rhs._mat[2][3]==0.0 && rhs._mat[3][3]==1.0);
		 return is_4x3 ? invert_4x3(rhs) :  invert_4x4(rhs);
	 }

	 /** 4x3 matrix invert, not right hand column is assumed to be 0,0,0,1. */
	 bool invert_4x3( const Matrixd& mat);

	 /** full 4x4 matrix invert. */
	 bool invert_4x4( const Matrixd& mat);

	 inline static Matrixd inverse( const Matrixd& matrix);


	 // basic Matrixd multiplication, our workhorse methods.
	 void mult( const Matrixd&, const Matrixd& );
	 void preMult( const Matrixd& );
	 void postMult( const Matrixd& );

	 //从当前坐标系axis3转到世界坐标系下的转化矩阵
	 void makeMatrix(const Axis3& axis3);

	 //从世界坐标系转化到当前坐标系axis3的转化矩阵
	 void makeReverseMatrix(const Axis3& axis3);

	 //从fromAxis坐标系转化到toAxis3坐标下的变换矩阵
	 void makeMatrix(const Axis3& fromAxis3, const Axis3& toAxis3);

	 inline Point3D preMult(const Point3D& v) const;
	 inline Point3D postMult(const Point3D& v) const;
	 inline Point3D operator* (const Point3D& v) const;


	 void setTrans( value_type tx, value_type ty, value_type tz );
	 void setTrans( const Point3D& v );

	 inline Point3D getTrans() const { return Point3D(_mat[3][0],_mat[3][1],_mat[3][2]); } 

protected:
	value_type _mat[4][4];
};


inline Point3D Matrixd::preMult( const Point3D& v ) const
{
	value_type d = 1.0f/(_mat[0][3]*v.x()+_mat[1][3]*v.y()+_mat[2][3]*v.z()+_mat[3][3]) ;

	return Point3D( (_mat[0][0]*v.x() + _mat[1][0]*v.y() + _mat[2][0]*v.z() + _mat[3][0])*d,
		(_mat[0][1]*v.x() + _mat[1][1]*v.y() + _mat[2][1]*v.z() + _mat[3][1])*d,
		(_mat[0][2]*v.x() + _mat[1][2]*v.y() + _mat[2][2]*v.z() + _mat[3][2])*d);
}

inline Point3D Matrixd::postMult( const Point3D& v ) const
{
	value_type d = 1.0f/(_mat[3][0]*v.x()+_mat[3][1]*v.y()+_mat[3][2]*v.z()+_mat[3][3]) ;

	return Point3D( (_mat[0][0]*v.x() + _mat[0][1]*v.y() + _mat[0][2]*v.z() + _mat[0][3])*d,
		(_mat[1][0]*v.x() + _mat[1][1]*v.y() + _mat[1][2]*v.z() + _mat[1][3])*d,
		(_mat[2][0]*v.x() + _mat[2][1]*v.y() + _mat[2][2]*v.z() + _mat[2][3])*d) ;
}


inline Matrixd Matrixd::inverse( const Matrixd& matrix)
{
	Matrixd m;
	m.invert(matrix);
	return m;
}

inline Point3D operator* (const Point3D& v, const Matrixd& m)
{
	return m.preMult(v);
}



#endif

