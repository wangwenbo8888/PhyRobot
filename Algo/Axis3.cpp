#include "Axis3.h"
#include <float.h>

Axis3::Axis3( const Point3D& p, const Point3D& N, const Point3D& Vx)
	: m_Location(p),m_vzdir(N),m_vydir(N),m_vxdir(N)
{
	m_vxdir.CrossCross(Vx, N);
	if(m_vxdir.Length() < 1e-10)
	{
		//yhm 20180717
		Axis3 temp(p,N);
		m_vxdir = temp.XDirection();
		m_vydir = temp.YDirection();
		m_vzdir = temp.ZDirection();
		return;
	}

	m_vxdir.Normalize();
	m_vydir.Cross(m_vxdir);
	m_vydir.Normalize();
	m_vzdir.Normalize();
}

Axis3::Axis3(const Point3D& p, const Point3D& v)
{	
	m_Location = p;
	m_vzdir = v;
	m_vzdir.Normalize();

	const double eps = DBL_EPSILON;
	const double f = m_vzdir.x()*m_vzdir.x() + m_vzdir.y()*m_vzdir.y();

	if (f > eps)
	{
		const double d = 1.0 / sqrt(f);

		m_vxdir.set(m_vzdir.y()*d, -m_vzdir.x()*d, 0.0);
		m_vydir.set(-m_vzdir.z()*m_vxdir.y(), m_vzdir.z()*m_vxdir.x(), m_vzdir.x()*m_vxdir.y() - m_vzdir.y()*m_vxdir.x());
	}
	else 
	{
		m_vxdir.set(m_vzdir.z()<0.0? -1.0:1.0, 0.0, 0.0);
		m_vydir.set(0.0, 1.0, 0.0);
	}

#if 0//yhm
	m_Location = p;

	double A = v.x();
	double B = v.y();
	double C = v.z();

	double Aabs = xjdef::absolute(A);
	double Babs = xjdef::absolute(B);
	double Cabs = xjdef::absolute(C);

	Point3D D;

	//  pour determiner l axe X :
	//  on dit que le produit scalaire Vx.V = 0. 
	//  et on recherche le max(A,B,C) pour faire la division.
	//  l une des coordonnees du vecteur est nulle. 

	if ( Babs <= Aabs && Babs <= Cabs)
	{
		if (Aabs > Cabs) D.set(-C,0., A);
		else             D.set( C,0.,-A);
	}
	else if( Aabs <= Babs && Aabs <= Cabs) 
	{
		if (Babs > Cabs) D.set(0.,-C, B);
		else             D.set(0., C,-B);
	}
	else 
	{
		if (Aabs > Babs) D.set(-B, A,0.);
		else             D.set( B,-A,0.);
	}

	m_vzdir = v;
	m_vydir = D;
	m_vxdir = m_vydir.Crossed(v);

	m_vzdir.Normalize();
	m_vxdir.Normalize();
	m_vydir.Normalize();
#endif
}

Axis3::Axis3(const Axis3& axis3 )
{
	m_Location = axis3.Location();
	m_vzdir = axis3.ZDirection();
	m_vydir = axis3.YDirection();
	m_vxdir = axis3.XDirection();
}

void Axis3::SetLocation( const Point3D& P )
{
	m_Location = P;
}

void Axis3::SetXDirection( const Point3D& Vx )
{
	m_vxdir = Vx;
	m_vydir = m_vzdir.Crossed(m_vxdir);
	m_vzdir = m_vxdir.Crossed(m_vydir);
	
}

void Axis3::SetYDirection( const Point3D& Vy )
{
	m_vydir = Vy;
	m_vxdir = m_vydir.Crossed(m_vzdir);
	m_vzdir = m_vxdir.Crossed(m_vydir);
}

void Axis3::SetZDirection( const Point3D& Vz )
{
	m_vzdir = Vz;
	m_vzdir.Normalize();
	m_vxdir = Vz.CrossCrossed(m_vxdir, Vz);
	m_vydir = m_vzdir.Crossed(m_vxdir);
}

void Axis3::operator=( const Axis3& axis3 )
{
	m_Location = axis3.Location();
	m_vzdir = axis3.ZDirection();
	m_vydir = axis3.YDirection();
	m_vxdir = axis3.XDirection();
}