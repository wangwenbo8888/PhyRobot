#ifndef _AXIS3_
#define _AXIS3_

//#include "Common/Xjdef.h"
#include "Point3D.h"

//此坐标系为右手坐标系
class Axis3
{
public:

	//! Creates an object corresponding to the reference
	//! coordinate system (OXYZ).
	Axis3() : m_Location(0.0, 0.0, 0.0),
		m_vzdir(0.0, 0.0, 1.0),
		m_vydir(0.0, 1.0, 0.0),
		m_vxdir(1.0, 0.0, 0.0)
	{}

	//! Creates a  right handed axis placement with the
	//! "Location"  point  P  and  two  directions, N    gives the
	//! "Direction" and Vx gives the "XDirection".
	//! Raises ConstructionError if N and Vx are parallel (same or opposite orientation).
	Axis3(const Point3D& p, const Point3D& N, const Point3D& Vx);


	//! Creates an axis placement with the  "Location" point <P>
	//! and the normal direction <V>.
	Axis3(const Point3D& p, const Point3D& v);

	//copy construction
	Axis3(const Axis3& axis3);

	//! Changes the "Location" point (origin) of <me>.
	void SetLocation (const Point3D& P) ;

	void SetXDirection(const Point3D& Vx);

	void SetYDirection(const Point3D& Vy);

	void SetZDirection(const Point3D& Vz);
	
	//! Returns the "Location" point (origin) of <me>.
	const  Point3D& Location()  const {return m_Location;}

	Point3D& Location() {return m_Location;}

	//! Returns the main direction of <me>.
	const  Point3D& ZDirection()  const {return m_vzdir;}

	Point3D& ZDirection()  {return m_vzdir;}

	//! Returns the "XDirection" of <me>.
	const  Point3D& XDirection()  const {return m_vxdir;}

	Point3D& XDirection() {return m_vxdir;}

	//! Returns the "YDirection" of <me>.
	const  Point3D& YDirection()  const {return m_vydir;}

	Point3D& YDirection() {return m_vydir;}

	//operator =
	inline void operator = (const Axis3& axis3); 
private:
	Point3D m_Location;
	Point3D m_vzdir;
	Point3D m_vydir;
	Point3D m_vxdir;
};


#endif

