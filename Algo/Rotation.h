    /***************************************************************************

 ***************************************************************************/


#ifndef BASE_ROTATION_H
#define BASE_ROTATION_H

#include "Point3D.h"

class CRotation
{
public:
    /** Construction. */
    //@{
    CRotation();
    CRotation(const Point3D& axis, const double fAngle);
    CRotation(const double q[4]);
    CRotation(const double q0, const double q1, const double q2, const double q3);
    CRotation(const Point3D& rotateFrom, const Point3D& rotateTo);
    CRotation(const CRotation& rot);
    //@}

    /** Methods to get or set rotations. */
    //@{
    const double * getValue(void) const;
    void getValue(double & q0, double & q1, double & q2, double & q3) const;
    void setValue(const double q0, const double q1, const double q2, const double q3);
    void getValue(Point3D & axis, double & rfAngle) const;
    void setValue(const double q[4]);
    void setValue(const Point3D & axis, const double fAngle);
    void setValue(const Point3D & rotateFrom, const Point3D & rotateTo);
    /// Euler angles in yaw,pitch,roll notation
    void setYawPitchRoll(double y, double p, double r);
    /// Euler angles in yaw,pitch,roll notation
    void getYawPitchRoll(double& y, double& p, double& r) const;
    //@}

    /** Invert rotations. */
    //@{
    CRotation & invert(void);
    CRotation inverse(void) const;
    //@}

    /** Operators. */
    //@{
    CRotation & operator*=(const CRotation & q);
    CRotation operator *(const CRotation & q) const;
    bool operator==(const CRotation & q) const;
    bool operator!=(const CRotation & q) const;
    double & operator [] (unsigned short usIndex){return quat[usIndex];}
    const double & operator [] (unsigned short usIndex) const{return quat[usIndex];}

    void multVec(const Point3D & src, Point3D & dst) const;
    void scaleAngle(const double scaleFactor);
    //@}

    static CRotation slerp(const CRotation & rot0, const CRotation & rot1, double t);
    static CRotation identity(void);

private:
    void normalize();
    double quat[4];
};

#endif // BASE_ROTATION_H
