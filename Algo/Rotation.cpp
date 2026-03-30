
#ifndef _PreComp_
# include <cmath>
# include <climits>
#endif

#include "Rotation.h"
#include <float.h>

CRotation::CRotation()
{
    quat[0]=quat[1]=quat[2]=0.0;quat[3]=1.0;
}

/** Construct a rotation by rotation axis and angle */
CRotation::CRotation(const Point3D& axis, const double fAngle)
{
    this->setValue(axis, fAngle);
}


/** Construct a rotation initialized with the given quaternion components:
 * q[0] = x, q[1] = y, q[2] = z and q[3] = w,
 * where the quaternion is specified by q=w+xi+yj+zk.
 */
CRotation::CRotation(const double q[4])
{
    this->setValue(q);
}

/** Construct a rotation initialized with the given quaternion components:
 * q0 = x, q1 = y, q2 = z and q3 = w,
 * where the quaternion is specified by q=w+xi+yj+zk.
 */
CRotation::CRotation(const double q0, const double q1, const double q2, const double q3)
{
    this->setValue(q0, q1, q2, q3);
}

CRotation::CRotation(const Point3D & rotateFrom, const Point3D& rotateTo)
{
    this->setValue(rotateFrom, rotateTo);
}

CRotation::CRotation(const CRotation& rot)
{
    this->quat[0] = rot.quat[0];
    this->quat[1] = rot.quat[1];
    this->quat[2] = rot.quat[2];
    this->quat[3] = rot.quat[3];
}

const double * CRotation::getValue(void) const
{
    return &this->quat[0];
}

void CRotation::getValue(double & q0, double & q1, double & q2, double & q3) const
{
    q0 = this->quat[0];
    q1 = this->quat[1];
    q2 = this->quat[2];
    q3 = this->quat[3];
}

void CRotation::setValue(const double q0, const double q1, const double q2, const double q3)
{
    this->quat[0] = q0;
    this->quat[1] = q1;
    this->quat[2] = q2;
    this->quat[3] = q3;
    this->normalize();
}

void CRotation::getValue(Point3D& axis, double & rfAngle) const
{
    // Taken from <http://de.wikipedia.org/wiki/Quaternionen>
    //
    // Note: -1 < w < +1 (|w| == 1 not allowed, with w:=quat[3]) 
    if((this->quat[3] > -1.0) && (this->quat[3] < 1.0)) {
        rfAngle = double(acos(this->quat[3])) * 2.0;
        double scale = (double)sin(rfAngle / 2.0);
        // Get a normalized vector 
        axis.x() = this->quat[0] / scale;
        axis.y() = this->quat[1] / scale;
        axis.z() = this->quat[2] / scale;
    }
    else {
        // The quaternion doesn't describe a rotation, so we can setup any value we want 
        axis.set(0.0, 0.0, 1.0);
        rfAngle = 0.0;
    }
}

void CRotation::setValue(const double q[4])
{
    this->quat[0] = q[0];
    this->quat[1] = q[1];
    this->quat[2] = q[2];
    this->quat[3] = q[3];
    this->normalize();
}

void CRotation::setValue(const Point3D & axis, const double fAngle)
{
    // Taken from <http://de.wikipedia.org/wiki/Quaternionen>
    //
    this->quat[3] = (double)cos(fAngle/2.0);
    Point3D norm = axis;
    norm.Normalize();
    double scale = (double)sin(fAngle/2.0);
    this->quat[0] = norm.x() * scale;
    this->quat[1] = norm.y() * scale;
    this->quat[2] = norm.z() * scale;
}

void CRotation::setValue(const Point3D & rotateFrom, const Point3D & rotateTo)
{
    Point3D u(rotateFrom); u.Normalize();
    Point3D v(rotateTo); v.Normalize();

    // The vector from x to is the roatation axis because it's the normal of the plane defined by (0,u,v) 
    const double dot = u * v;
    Point3D w = u.Crossed(v);
    const double wlen = w.Length();

    if (wlen == 0.0) { // Parallel vectors
        // Check if they are pointing in the same direction.
        if (dot > 0.0) {
            this->setValue(0.0, 0.0, 0.0, 1.0);
        }
        else {
            // We can use any axis perpendicular to u (and v)
            Point3D t = u.Crossed(Point3D(1.0,0.0,0.0));
            if(t.Length() < FLT_EPSILON) 
                t = u.Crossed(Point3D(0.0,1.0,0.0));
            this->setValue(t.x(), t.y(), t.z(), 0.0);
        }
    }
    else { // Vectors are not parallel
        // Note: A quaternion is not well-defined by specifying a point and its transformed point.
        // Every quaternion with a rotation axis having the same angle to the vectors of both points is okay.
        double angle = (double)acos(dot);
        this->setValue(w, angle);
    }
}

void CRotation::normalize()
{
    double len = (double)sqrt(this->quat[0]*this->quat[0]+
                              this->quat[1]*this->quat[1]+
                              this->quat[2]*this->quat[2]+
                              this->quat[3]*this->quat[3]);
    if (len != 0) {
        this->quat[0] /= len;
        this->quat[1] /= len;
        this->quat[2] /= len;
        this->quat[3] /= len;
    }
}

CRotation & CRotation::invert(void)
{
    this->quat[0] = -this->quat[0];
    this->quat[1] = -this->quat[1];
    this->quat[2] = -this->quat[2];
    return *this;
}

CRotation CRotation::inverse(void) const
{
    CRotation rot;
    rot.quat[0] = -this->quat[0];
    rot.quat[1] = -this->quat[1];
    rot.quat[2] = -this->quat[2];
    rot.quat[3] =  this->quat[3];
    return rot;
}

CRotation & CRotation::operator*=(const CRotation & q)
{
    // Taken from <http://de.wikipedia.org/wiki/Quaternionen>
    double x0, y0, z0, w0;
    this->getValue(x0, y0, z0, w0);
    double x1, y1, z1, w1;
    q.getValue(x1, y1, z1, w1);

    this->setValue(w0*x1 + x0*w1 + y0*z1 - z0*y1,
                   w0*y1 - x0*z1 + y0*w1 + z0*x1,
                   w0*z1 + x0*y1 - y0*x1 + z0*w1,
                   w0*w1 - x0*x1 - y0*y1 - z0*z1);
    return *this;
}

CRotation CRotation::operator*(const CRotation & q) const
{
    CRotation quat(*this);
    quat *= q;
    return quat;
}

bool CRotation::operator==(const CRotation & q) const
{
    bool equal = true;
    for (int i=0; i<4;i++)
        equal &= (fabs(this->quat[i] - q.quat[i]) < 0.005 );
    return equal;
}

bool CRotation::operator!=(const CRotation & q) const
{
    return !(*this == q);
}

void CRotation::multVec(const Point3D & src, Point3D & dst) const
{
    double x = this->quat[0];
    double y = this->quat[1];
    double z = this->quat[2];
    double w = this->quat[3];
    double x2 = x * x;
    double y2 = y * y;
    double z2 = z * z;
    double w2 = w * w;

    double dx = (x2+w2-y2-z2)*src.x() + 2.0*(x*y-z*w)*src.y() + 2.0*(x*z+y*w)*src.z();
    double dy = 2.0*(x*y+z*w)*src.x() + (w2-x2+y2-z2)*src.y() + 2.0*(y*z-x*w)*src.z();
    double dz = 2.0*(x*z-y*w)*src.x() + 2.0*(x*w+y*z)*src.y() + (w2-x2-y2+z2)*src.z();
    dst.x() = dx;
    dst.y() = dy;
    dst.z() = dz;
}

void CRotation::scaleAngle(const double scaleFactor)
{
    Point3D axis;
    double fAngle;
    this->getValue(axis, fAngle);
    this->setValue(axis, fAngle * scaleFactor);
}

CRotation CRotation::slerp(const CRotation & q0, const CRotation & q1, double t)
{
    // Taken from <http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/>
    // q = [q0*sin((1-t)*theta)+q1*sin(t*theta)]/sin(theta), 0<=t<=1
    if (t<0.0) t=0.0;
    else if (t>1.0) t=1.0;
    //return q0;

    double scale0 = 1.0 - t;
    double scale1 = t;
    double dot = q0.quat[0]*q1.quat[0]+q0.quat[1]*q1.quat[1]+q0.quat[2]*q1.quat[2]+q0.quat[3]*q1.quat[3];
    bool neg=false;
    if(dot < 0.0) {
        dot = -dot;
        neg = true;
    }

    if ((1.0 - dot) > FLT_EPSILON) {
        double angle = (double)acos(dot);
        double sinangle = (double)sin(angle);
        // If possible calculate spherical interpolation, otherwise use linear interpolation
        if (sinangle > FLT_EPSILON) {
            scale0 = double(sin((1.0 - t) * angle)) / sinangle;
            scale1 = double(sin(t * angle)) / sinangle;
        }
    }

    if (neg)
        scale1 = -scale1;

    double x = scale0 * q0.quat[0] + scale1 * q1.quat[0];
    double y = scale0 * q0.quat[1] + scale1 * q1.quat[1];
    double z = scale0 * q0.quat[2] + scale1 * q1.quat[2];
    double w = scale0 * q0.quat[3] + scale1 * q1.quat[3];
    return CRotation(x, y, z, w);
}

CRotation CRotation::identity(void)
{
    return CRotation(0.0, 0.0, 0.0, 1.0);
}

void CRotation::setYawPitchRoll(double y, double p, double r)
{
    // The Euler angles (yaw,pitch,roll) are in XY'Z''-notation
    // convert to radians
    y = (y/180.0)*XJPI;
    p = (p/180.0)*XJPI;
    r = (r/180.0)*XJPI;

    double c1 = cos(y/2.0);
    double s1 = sin(y/2.0);
    double c2 = cos(p/2.0);
    double s2 = sin(p/2.0);
    double c3 = cos(r/2.0);
    double s3 = sin(r/2.0);

    quat[0] = c1*c2*s3 - s1*s2*c3;
    quat[1] = c1*s2*c3 + s1*c2*s3;
    quat[2] = s1*c2*c3 - c1*s2*s3;
    quat[3] = c1*c2*c3 + s1*s2*s3;
}

void CRotation::getYawPitchRoll(double& y, double& p, double& r) const
{
    double q00 = quat[0]*quat[0];
    double q11 = quat[1]*quat[1];
    double q22 = quat[2]*quat[2];
    double q33 = quat[3]*quat[3];
    double q01 = quat[0]*quat[1];
    double q02 = quat[0]*quat[2];
    double q03 = quat[0]*quat[3];
    double q12 = quat[1]*quat[2];
    double q13 = quat[1]*quat[3];
    double q23 = quat[2]*quat[3];
    double qd2 = 2.0*(q13-q02);

    y = atan2(2.0*(q01+q23),(q00+q33)-(q11+q22));
    p = qd2 > 1.0 ? XJPI/2.0 : (qd2 < -1.0 ? -XJPI/2.0 : asin (qd2));
    r = atan2(2.0*(q12+q03),(q22+q33)-(q00+q11));

    // convert to degree
    y = (y/XJPI)*180;
    p = (p/XJPI)*180;
    r = (r/XJPI)*180;

    if (fabs(y)>5.0)
    {
        if (y>0.0)
        {
            y = 5.0;
        }
        else
        {
            y = -5.0;
        }
    }

    if (fabs(p) > 5.0)
    {
        if (p > 0.0)
        {
            p = 5.0;
        }
        else
        {
            p = -5.0;
        }
    }

    if (fabs(r) > 5.0)
    {
        if (r > 0.0)
        {
            r = 5.0;
        }
        else
        {
            r = -5.0;
        }
    }
}
