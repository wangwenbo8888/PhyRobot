#include "MyWindow.h"
#include "PhysicalTherapyRobot.h"

#include "Communicate.h"

#include <QTime>
#include <QStringLiteral>
#include <QDebug>

#include "AdmittanceControl.h"

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <cmath>
#include <algorithm>
#include <cstdlib>

class LowPassFilter {
private:
    std::deque<double> m_buffer;
    size_t m_windowSize;
    double m_alpha;

public:
    LowPassFilter(size_t windowSize = 5, double alpha = 0.3) 
        : m_windowSize(windowSize), m_alpha(alpha) {}

    double filter(double newValue) {
        m_buffer.push_back(newValue);
        if (m_buffer.size() > m_windowSize) {
            m_buffer.pop_front();
        }
        
        if (m_buffer.empty()) {
            return newValue;
        }

        double smoothed = 0.0;
        double weightSum = 0.0;
        
        for (size_t i = 0; i < m_buffer.size(); ++i) {
            double weight = pow(m_alpha, m_buffer.size() - 1 - i);
            smoothed += m_buffer[i] * weight;
            weightSum += weight;
        }
        
        return smoothed / weightSum;
    }

    void reset() {
        m_buffer.clear();
    }

    double getCurrent() const {
        if (m_buffer.empty()) return 0.0;
        return m_buffer.back();
    }
};

static LowPassFilter g_forceFilterX(5, 0.4);
static LowPassFilter g_forceFilterY(5, 0.4);
static LowPassFilter g_forceFilterZ(5, 0.4);
static LowPassFilter g_forceMagFilter(5, 0.3);
static LowPassFilter g_angleXFilter(3, 0.5);
static LowPassFilter g_angleYFilter(3, 0.5);

static int g_lowForceCounter = 0;
static int g_highForceCounter = 0;
static int g_stableCounter = 0;
static double g_lastValidZ = 0.0;
static double g_lastValidAngleX = -178.0;
static double g_lastValidAngleY = 0.0;

bool MyWindow::MoveToNextAcupointNew_Improved(int group, int row, DETECTED_XUEWEI currentxuewei,
    DETECTED_XUEWEI nextxuewei, std::vector<double>& next)
{
    bool bFirst = true;
    double maxForce = 20.0;
    double midForce = 10.0;
    double touchForce = 5.0;
    
    double deadZoneLow = 3.0;
    double deadZoneHigh = 3.0;
    
    double step = 3.0;
    
    g_forceFilterX.reset();
    g_forceFilterY.reset();
    g_forceFilterZ.reset();
    g_forceMagFilter.reset();
    g_lowForceCounter = 0;
    g_highForceCounter = 0;
    g_stableCounter = 0;
    
    double dis2D = sqrt((next[0] - m_vCurrentPos[0]) * (next[0] - m_vCurrentPos[0]) + 
                        (next[1] - m_vCurrentPos[1]) * (next[1] - m_vCurrentPos[1]));
    int divid = dis2D / step;
    if (divid < 1) divid = 1;
    
    double deltaZ = 0.0;
    double currentZ = m_vCurrentPos[2];
    double xRaw = m_vCurrentPos[0];
    double yRaw = m_vCurrentPos[1];
    
    double adaptiveMidForce = midForce;
    int consecutiveAdjustments = 0;
    
    for (int i = 1; i < divid; ++i)
    {
        double xPos = xRaw + (next[0] - xRaw) * i / divid;
        double yPos = yRaw + (next[1] - yRaw) * i / divid;
        currentZ += deltaZ;
        
        GetPose();
        
        double rawFx = m_vForces[0] - m_vRawForces[0];
        double rawFy = m_vForces[1] - m_vRawForces[1];
        double rawFz = m_vForces[2] - m_vRawForces[2];
        
        double filteredFx = g_forceFilterX.filter(rawFx);
        double filteredFy = g_forceFilterY.filter(rawFy);
        double filteredFz = g_forceFilterZ.filter(rawFz);
        
        Point3D filteredForce(filteredFx, filteredFy, filteredFz);
        double filteredMag = g_forceMagFilter.filter(filteredForce.Magnitude());
        
        double y = 0.0;
        double p = 0.0;
        double r = 0.0;
        
        if (filteredForce.x() > 10.0) {
            y = std::fabs(filteredForce.x()) * 0.2;
        }
        else if (filteredForce.x() > 1.0) {
            y = std::fabs(filteredForce.x()) * 0.05;
        }
        else if (filteredForce.x() < -10.0) {
            y = std::fabs(filteredForce.x()) * -0.2;
        }
        else if (filteredForce.x() < -1.0) {
            y = std::fabs(filteredForce.x()) * -0.05;
        }

        if (filteredForce.y() > 10.0) {
            p = std::fabs(filteredForce.y()) * -0.2;
        }
        else if (filteredForce.y() > 1.0) {
            p = std::fabs(filteredForce.y()) * -0.05;
        }
        else if (filteredForce.y() < -10.0) {
            p = std::fabs(filteredForce.y()) * 0.2;
        }
        else if (filteredForce.y() < -1.0) {
            p = std::fabs(filteredForce.y()) * 0.05;
        }

        double a = 0.0;
        double b = 0.0;
        
        if (m_vForces[3] < -0.1) {
            a = -1.0 * std::fabs(m_vForces[3]);
        }
        else if (m_vForces[3] > 0.1) {
            a = 6.0 * std::fabs(m_vForces[3]);
        }

        if (m_vForces[4] < -0.1) {
            b = 6.0 * std::fabs(m_vForces[4]);
        }
        else if (m_vForces[4] > 0.1) {
            b = -1.0 * std::fabs(m_vForces[4]);
        }

        double xAngle = m_vCurrentPos[3] - y - a;
        double yAngle = m_vCurrentPos[4] - p - b;
        double zAngle = m_vCurrentPos[5] - r;

        if (xAngle < -200.0) xAngle = -200.0;
        if (xAngle > -160.0) xAngle = -160.0;

        if (yAngle < -15.0) yAngle = -15.0;
        if (yAngle > 15.0) yAngle = 15.0;

        if (zAngle > 210.0) zAngle = 210.0;
        if (zAngle < 150.0) zAngle = 150.0;

        if (currentxuewei == ZHIYANG_DETECTED && bFirst) {
            xAngle = -178.0;
            yAngle = 0.0;
            bFirst = false;
        }

        if (!bFirst) {
            xAngle = -178.0;
            yAngle = 0.0;
        }

        qDebug() << "Move to next: " << xPos << " " << yPos << " " << currentZ 
                 << " " << xAngle << " " << yAngle << " " << zAngle;
        
        MovL(xPos, yPos, currentZ, xAngle, yAngle, NORMAL_ANGLE);

        Wait_Done();
        
        m_vCurrentPos[0] = xPos;
        m_vCurrentPos[1] = yPos;
        m_vCurrentPos[2] = currentZ;
        m_vCurrentPos[3] = xAngle;
        m_vCurrentPos[4] = yAngle;
        m_vCurrentPos[5] = zAngle;

        m_CurrForce = Point3D(filteredFx, filteredFy, filteredFz);
        
        qDebug() << "Force mag is : " << filteredMag;
        qDebug() << "Detected force is : " << filteredFx << " " << filteredFy << " " << filteredFz 
                 << " " << group << " " << row;
        
        deltaZ = 0.0;
        
        double safeZoneMin = adaptiveMidForce - deadZoneLow;
        double safeZoneMax = adaptiveMidForce + deadZoneHigh;
        
        if (filteredMag > maxForce) {
            g_highForceCounter++;
            g_lowForceCounter = 0;
            g_stableCounter = 0;
            
            if (g_highForceCounter >= 2) {
                double excess = filteredMag - maxForce;
                if (excess > 20) {
                    deltaZ = 4.0;
                }
                else if (excess > 10) {
                    deltaZ = 2.5;
                }
                else {
                    deltaZ = 1.5;
                }
                g_highForceCounter = 0;
                consecutiveAdjustments++;
            }
        }
        else if (filteredMag < touchForce) {
            g_lowForceCounter++;
            g_highForceCounter = 0;
            g_stableCounter = 0;
            
            if (g_lowForceCounter >= 3) {
                if (g_lastValidZ == 0.0) {
                    g_lastValidZ = currentZ;
                }
                
                double deficit = touchForce - filteredMag;
                if (deficit > 15) {
                    deltaZ = -2.5;
                }
                else if (deficit > 8) {
                    deltaZ = -1.5;
                }
                else {
                    deltaZ = -0.8;
                }
                g_lowForceCounter = 0;
                consecutiveAdjustments++;
            }
        }
        else if (filteredMag >= safeZoneMin && filteredMag <= safeZoneMax) {
            g_stableCounter++;
            g_lowForceCounter = 0;
            g_highForceCounter = 0;
            
            if (g_stableCounter >= 3) {
                g_lastValidZ = currentZ;
                
                if (consecutiveAdjustments > 5) {
                    adaptiveMidForce = adaptiveMidForce * 0.95 + filteredMag * 0.05;
                }
                consecutiveAdjustments = 0;
            }
            
            deltaZ = 0.0;
        }
        else {
            g_lowForceCounter = 0;
            g_highForceCounter = 0;
            g_stableCounter = 0;
            
            if (filteredMag > adaptiveMidForce && filteredMag < maxForce) {
                double excess = filteredMag - adaptiveMidForce;
                if (excess > 5) {
                    deltaZ = 1.0;
                }
                else {
                    deltaZ = 0.5;
                }
                consecutiveAdjustments++;
            }
            else if (filteredMag < adaptiveMidForce && filteredMag > touchForce) {
                double deficit = adaptiveMidForce - filteredMag;
                if (deficit > 5) {
                    deltaZ = -1.0;
                }
                else {
                    deltaZ = -0.5;
                }
                consecutiveAdjustments++;
            }
        }

        if (deltaZ != 0.0) {
            currentZ += deltaZ;
            m_vCurrentPos[2] = currentZ;
        }

        qDebug() << "Delta Z: " << deltaZ << " | Stable: " << g_stableCounter 
                 << " | Low: " << g_lowForceCounter << " | High: " << g_highForceCounter
                 << " | Adaptive Mid: " << adaptiveMidForce;
    }

    next[2] = currentZ;

    qDebug() << "Finish from one acu to other !";
    qDebug() << "Last valid Z: " << g_lastValidZ;

    return true;
}

bool MyWindow::MoveToNextAcupointNew_ImprovedV4(int group, int row, DETECTED_XUEWEI currentxuewei,
    DETECTED_XUEWEI nextxuewei, std::vector<double>& next)
{
    bool bFirst = true;
    
    bool isNeckRegion = (currentxuewei == FENGFU_DETECTED || 
                         currentxuewei == FENGCHI_DETECTED || 
                         currentxuewei == DAZHUI_DETECTED ||
                         nextxuewei == FENGFU_DETECTED || 
                         nextxuewei == FENGCHI_DETECTED || 
                         nextxuewei == DAZHUI_DETECTED);
    
    bool needHemisphereFit = !isNeckRegion;
    
    double neckBaseAngleX = -165.0;
    double bodyBaseAngleX = -178.0;
    
    const double hemisphericalRadius = 52.5;
    
    const double targetForce = 15.0;
    const double tolerance = 2.0;
    const double minForce = 12.0;
    const double maxForce = 18.0;
    
    const double hysteresisLow = 13.0;
    const double hysteresisHigh = 17.0;
    
    double angleP = 1.5;
    double angleD = 1.0;
    double maxAngleAdjust = 5.0;
    
    if (isNeckRegion) {
        angleP = 1.5;
        angleD = 1.0;
        maxAngleAdjust = 2.0;
    }
    else {
        angleP = 0.3;
        angleD = 0.2;
        maxAngleAdjust = 1.0;
    }
    
    double step = 3.0;
    
    g_forceFilterX.reset();
    g_forceFilterY.reset();
    g_forceFilterZ.reset();
    g_forceMagFilter.reset();
    g_angleXFilter.reset();
    g_angleYFilter.reset();
    
    g_lowForceCounter = 0;
    g_highForceCounter = 0;
    g_stableCounter = 0;
    g_lastValidZ = m_vCurrentPos[2];
    
    static double lastAngleErrorX = 0.0;
    static double lastAngleErrorY = 0.0;
    lastAngleErrorX = 0.0;
    lastAngleErrorY = 0.0;
    
    double dis2D = sqrt((next[0] - m_vCurrentPos[0]) * (next[0] - m_vCurrentPos[0]) + 
                        (next[1] - m_vCurrentPos[1]) * (next[1] - m_vCurrentPos[1]));
    int divid = dis2D / step;
    if (divid < 1) divid = 1;
    
    double deltaZ = 0.0;
    double currentZ = m_vCurrentPos[2];
    double xRaw = m_vCurrentPos[0];
    double yRaw = m_vCurrentPos[1];
    
    double adaptiveMidForce = targetForce;
    int consecutiveAdjustments = 0;
    
    for (int i = 1; i < divid; ++i)
    {
        double xPos = xRaw + (next[0] - xRaw) * i / divid;
        double yPos = yRaw + (next[1] - yRaw) * i / divid;
        currentZ += deltaZ;
        
        GetPose();
        
        double rawFx = m_vForces[0] - m_vRawForces[0];
        double rawFy = m_vForces[1] - m_vRawForces[1];
        double rawFz = m_vForces[2] - m_vRawForces[2];
        
        double filteredFx = g_forceFilterX.filter(rawFx);
        double filteredFy = g_forceFilterY.filter(rawFy);
        double filteredFz = g_forceFilterZ.filter(rawFz);
        
        Point3D filteredForce(filteredFx, filteredFy, filteredFz);
        double filteredMag = g_forceMagFilter.filter(filteredForce.Magnitude());
        
        double y_comp = 0.0;
        double p_comp = 0.0;
        
        if (filteredForce.x() > 10.0) {
            y_comp = std::fabs(filteredForce.x()) * 0.2;
        }
        else if (filteredForce.x() > 1.0) {
            y_comp = std::fabs(filteredForce.x()) * 0.05;
        }
        else if (filteredForce.x() < -10.0) {
            y_comp = std::fabs(filteredForce.x()) * -0.2;
        }
        else if (filteredForce.x() < -1.0) {
            y_comp = std::fabs(filteredForce.x()) * -0.05;
        }

        if (filteredForce.y() > 10.0) {
            p_comp = std::fabs(filteredForce.y()) * -0.2;
        }
        else if (filteredForce.y() > 1.0) {
            p_comp = std::fabs(filteredForce.y()) * -0.05;
        }
        else if (filteredForce.y() < -10.0) {
            p_comp = std::fabs(filteredForce.y()) * 0.2;
        }
        else if (filteredForce.y() < -1.0) {
            p_comp = std::fabs(filteredForce.y()) * 0.05;
        }

        double a = 0.0;
        double b = 0.0;
        
        if (m_vForces[3] < -0.1) {
            a = -1.0 * std::fabs(m_vForces[3]);
        }
        else if (m_vForces[3] > 0.1) {
            a = 6.0 * std::fabs(m_vForces[3]);
        }

        if (m_vForces[4] < -0.1) {
            b = 6.0 * std::fabs(m_vForces[4]);
        }
        else if (m_vForces[4] > 0.1) {
            b = -1.0 * std::fabs(m_vForces[4]);
        }
        
        double angleAdjustX = 0.0;
        double angleAdjustY = 0.0;
        
        if (needHemisphereFit) {
            if (std::fabs(filteredFx) > 8.0) {
                angleAdjustX = -(filteredFx - std::fabs(filteredFx) / filteredFx * 8.0) * 0.1;
            }
            if (std::fabs(filteredFy) > 8.0) {
                angleAdjustY = -(filteredFy - std::fabs(filteredFy) / filteredFy * 8.0) * 0.1;
            }
            
            angleAdjustX = std::max(-maxAngleAdjust, std::min(maxAngleAdjust, angleAdjustX));
            angleAdjustY = std::max(-maxAngleAdjust, std::min(maxAngleAdjust, angleAdjustY));
        }
        
        double baseAngleX = bodyBaseAngleX;
        double baseAngleY = 0.0;
        
        if (currentxuewei == ZHIYANG_DETECTED && bFirst) {
            baseAngleX = bodyBaseAngleX;
            baseAngleY = 0.0;
            bFirst = false;
        }
        else if (!bFirst) {
            baseAngleX = bodyBaseAngleX;
            baseAngleY = 0.0;
        }
        else if (isNeckRegion) {
            baseAngleX = neckBaseAngleX;
        }
        
        double targetAngleX = baseAngleX + angleAdjustX - y_comp - a;
        double targetAngleY = baseAngleY + angleAdjustY - p_comp - b;
        
        double xAngle = g_angleXFilter.filter(targetAngleX);
        double yAngle = g_angleYFilter.filter(targetAngleY);
        double zAngle = m_vCurrentPos[5];

        if (xAngle < -200.0) xAngle = -200.0;
        if (xAngle > -160.0) xAngle = -160.0;

        if (yAngle < -15.0) yAngle = -15.0;
        if (yAngle > 15.0) yAngle = 15.0;

        if (zAngle > 210.0) zAngle = 210.0;
        if (zAngle < 150.0) zAngle = 150.0;

        qDebug() << "Move to next: " << xPos << " " << yPos << " " << currentZ 
                 << " " << xAngle << " " << yAngle << " " << zAngle
                 << " Neck:" << isNeckRegion << " Fit:" << needHemisphereFit;
        
        MovL(xPos, yPos, currentZ, xAngle, yAngle, NORMAL_ANGLE);

        Wait_Done();
        
        m_vCurrentPos[0] = xPos;
        m_vCurrentPos[1] = yPos;
        m_vCurrentPos[2] = currentZ;
        m_vCurrentPos[3] = xAngle;
        m_vCurrentPos[4] = yAngle;
        m_vCurrentPos[5] = zAngle;

        m_CurrForce = Point3D(filteredFx, filteredFy, filteredFz);
        
        qDebug() << "Force mag: " << filteredMag << " | Fx:" << filteredFx << " Fy:" << filteredFy;
        
        deltaZ = 0.0;
        
        double safeZoneMin = adaptiveMidForce - tolerance;
        double safeZoneMax = adaptiveMidForce + tolerance;
        
        if (filteredMag > hysteresisHigh) {
            g_highForceCounter++;
            g_lowForceCounter = 0;
            g_stableCounter = 0;
            
            if (g_highForceCounter >= 2) {
                double excess = filteredMag - targetForce;
                if (excess > 5) {
                    deltaZ = 2.0;
                }
                else if (excess > 2) {
                    deltaZ = 1.0;
                }
                else {
                    deltaZ = 0.5;
                }
                g_highForceCounter = 0;
                consecutiveAdjustments++;
            }
        }
        else if (filteredMag < hysteresisLow) {
            g_lowForceCounter++;
            g_highForceCounter = 0;
            g_stableCounter = 0;
            
            if (g_lowForceCounter >= 2) {
                if (g_lastValidZ == 0.0) {
                    g_lastValidZ = currentZ;
                }
                
                double deficit = targetForce - filteredMag;
                if (deficit > 5) {
                    deltaZ = -2.0;
                }
                else if (deficit > 2) {
                    deltaZ = -1.0;
                }
                else {
                    deltaZ = -0.5;
                }
                g_lowForceCounter = 0;
                consecutiveAdjustments++;
            }
        }
        else if (filteredMag >= safeZoneMin && filteredMag <= safeZoneMax) {
            g_stableCounter++;
            g_lowForceCounter = 0;
            g_highForceCounter = 0;
            
            if (g_stableCounter >= 3) {
                g_lastValidZ = currentZ;
                
                if (consecutiveAdjustments > 8) {
                    adaptiveMidForce = adaptiveMidForce * 0.95 + filteredMag * 0.05;
                }
                consecutiveAdjustments = 0;
            }
            
            deltaZ = 0.0;
        }
        else {
            g_lowForceCounter = 0;
            g_highForceCounter = 0;
            g_stableCounter = 0;
            
            if (filteredMag > adaptiveMidForce && filteredMag < maxForce) {
                double excess = filteredMag - adaptiveMidForce;
                if (excess > 3) {
                    deltaZ = 0.8;
                }
                else {
                    deltaZ = 0.4;
                }
                consecutiveAdjustments++;
            }
            else if (filteredMag < adaptiveMidForce && filteredMag > minForce) {
                double deficit = adaptiveMidForce - filteredMag;
                if (deficit > 3) {
                    deltaZ = -0.8;
                }
                else {
                    deltaZ = -0.4;
                }
                consecutiveAdjustments++;
            }
        }
        
        if (needHemisphereFit && std::fabs(filteredFx) > 6.0) {
            double lateralForce = std::fabs(filteredFx);
            double hemisphereDepth = hemisphericalRadius - std::sqrt(
                std::max(0.0, hemisphericalRadius * hemisphericalRadius - 
                        (lateralForce * lateralForce / (angleP * angleP))));
            if (hemisphereDepth > 0 && hemisphereDepth < hemisphericalRadius) {
                deltaZ += hemisphereDepth * 0.03;
            }
        }
        
        if (deltaZ != 0.0) {
            currentZ += deltaZ;
            m_vCurrentPos[2] = currentZ;
        }

        qDebug() << "Delta Z: " << deltaZ << " | Stable: " << g_stableCounter 
                 << " | Low: " << g_lowForceCounter << " | High: " << g_highForceCounter
                 << " | Adaptive Mid: " << adaptiveMidForce;
    }

    next[2] = currentZ;

    qDebug() << "Finish from one acu to other (V4 Hemisphere) !";
    qDebug() << "Last valid Z: " << g_lastValidZ;

    return true;
}

bool MyWindow::MoveToNextAcupointNew_ImprovedV2(int group, int row, DETECTED_XUEWEI currentxuewei,
    DETECTED_XUEWEI nextxuewei, std::vector<double>& next)
{
    bool bFirst = true;
    const double targetForce = 12.0;
    const double tolerance = 3.0;
    const double minForce = 6.0;
    const double maxForce = 20.0;
    
    const double P_gain = 0.5;
    const double I_gain = 0.1;
    const double D_gain = 2.0;
    
    static double integralError = 0.0;
    static double lastError = 0.0;
    
    double step = 3.0;
    
    g_forceFilterX.reset();
    g_forceFilterY.reset();
    g_forceFilterZ.reset();
    g_forceMagFilter.reset();
    
    double dis2D = sqrt((next[0] - m_vCurrentPos[0]) * (next[0] - m_vCurrentPos[0]) + 
                        (next[1] - m_vCurrentPos[1]) * (next[1] - m_vCurrentPos[1]));
    int divid = dis2D / step;
    if (divid < 1) divid = 1;
    
    double currentZ = m_vCurrentPos[2];
    double xRaw = m_vCurrentPos[0];
    double yRaw = m_vCurrentPos[1];
    
    integralError = 0.0;
    lastError = 0.0;
    
    for (int i = 1; i < divid; ++i)
    {
        double xPos = xRaw + (next[0] - xRaw) * i / divid;
        double yPos = yRaw + (next[1] - yRaw) * i / divid;
        
        GetPose();
        
        double rawFx = m_vForces[0] - m_vRawForces[0];
        double rawFy = m_vForces[1] - m_vRawForces[1];
        double rawFz = m_vForces[2] - m_vRawForces[2];
        
        double filteredFx = g_forceFilterX.filter(rawFx);
        double filteredFy = g_forceFilterY.filter(rawFy);
        double filteredFz = g_forceFilterZ.filter(rawFz);
        
        Point3D filteredForce(filteredFx, filteredFy, filteredFz);
        double filteredMag = g_forceMagFilter.filter(filteredForce.Magnitude());
        
        double y = 0.0;
        double p = 0.0;
        
        if (filteredForce.x() > 10.0) y = abs(filteredForce.x()) * 0.2;
        else if (filteredForce.x() > 1.0) y = abs(filteredForce.x()) * 0.05;
        else if (filteredForce.x() < -10.0) y = abs(filteredForce.x()) * -0.2;
        else if (filteredForce.x() < -1.0) y = abs(filteredForce.x()) * -0.05;

        if (filteredForce.y() > 10.0) p = abs(filteredForce.y()) * -0.2;
        else if (filteredForce.y() > 1.0) p = abs(filteredForce.y()) * -0.05;
        else if (filteredForce.y() < -10.0) p = abs(filteredForce.y()) * 0.2;
        else if (filteredForce.y() < -1.0) p = abs(filteredForce.y()) * 0.05;

        double a = 0.0;
        double b = 0.0;
        
        if (m_vForces[3] < -0.1) a = -1.0 * abs(m_vForces[3]);
        else if (m_vForces[3] > 0.1) a = 6.0 * abs(m_vForces[3]);

        if (m_vForces[4] < -0.1) b = 6.0 * abs(m_vForces[4]);
        else if (m_vForces[4] > 0.1) b = -1.0 * abs(m_vForces[4]);

        double xAngle = m_vCurrentPos[3] - y - a;
        double yAngle = m_vCurrentPos[4] - p - b;
        double zAngle = m_vCurrentPos[5];

        xAngle = std::max(-200.0, std::min(-160.0, xAngle));
        yAngle = std::max(-15.0, std::min(15.0, yAngle));
        zAngle = std::max(150.0, std::min(210.0, zAngle));

        if (currentxuewei == ZHIYANG_DETECTED && bFirst) {
            xAngle = -178.0;
            yAngle = 0.0;
            bFirst = false;
        }
        else if (!bFirst) {
            xAngle = -178.0;
            yAngle = 0.0;
        }

        qDebug() << "Move to next: " << xPos << " " << yPos << " " << currentZ 
                 << " " << xAngle << " " << yAngle << " " << zAngle;
        
        MovL(xPos, yPos, currentZ, xAngle, yAngle, NORMAL_ANGLE);
        Wait_Done();
        
        m_vCurrentPos[0] = xPos;
        m_vCurrentPos[1] = yPos;
        m_vCurrentPos[2] = currentZ;
        m_vCurrentPos[3] = xAngle;
        m_vCurrentPos[4] = yAngle;
        m_vCurrentPos[5] = zAngle;

        m_CurrForce = Point3D(filteredFx, filteredFy, filteredFz);
        
        double error = filteredMag - targetForce;
        
        if (std::abs(error) < tolerance) {
            integralError = 0.0;
        }
        else {
            integralError += error * 0.1;
            integralError = std::max(-5.0, std::min(5.0, integralError));
        }
        
        double derivative = lastError - error;
        lastError = error;
        
        double deltaZ = P_gain * error + I_gain * integralError + D_gain * derivative;
        deltaZ = std::max(-3.0, std::min(3.0, deltaZ));
        
        if (filteredMag < minForce) {
            deltaZ = std::min(deltaZ, -0.5);
        }
        else if (filteredMag > maxForce) {
            deltaZ = std::max(deltaZ, 2.0);
        }
        
        currentZ += deltaZ;
        
        qDebug() << "Force: " << filteredMag << " | Error: " << error 
                 << " | DeltaZ: " << deltaZ << " | Z: " << currentZ;
    }

    next[2] = currentZ;

    qDebug() << "Finish from one acu to other (PID version) !";

    return true;
}

bool MyWindow::MoveToNextAcupointNew_ImprovedV3(int group, int row, DETECTED_XUEWEI currentxuewei,
    DETECTED_XUEWEI nextxuewei, std::vector<double>& next)
{
    bool bFirst = true;
    
    bool isNeckRegion = (currentxuewei == FENGFU_DETECTED || 
                         currentxuewei == FENGCHI_DETECTED || 
                         currentxuewei == DAZHUI_DETECTED ||
                         nextxuewei == FENGFU_DETECTED || 
                         nextxuewei == FENGCHI_DETECTED || 
                         nextxuewei == DAZHUI_DETECTED);
    
    bool needHemisphereFit = !isNeckRegion;
    
    double neckBaseAngleX = -165.0;
    double bodyBaseAngleX = -178.0;
    
    const double targetForce = 15.0;
    const double tolerance = 2.0;
    const double minForce = 12.0;
    const double maxForce = 18.0;
    
    const double hysteresisLow = 13.0;
    const double hysteresisHigh = 17.0;
    
    const double P_gain = 0.3;
    const double I_gain = 0.05;
    const double D_gain = 1.5;
    
    double angleP_gain = 2.0;
    double angleD_gain = 1.5;
    double maxAngleAdjustment = 8.0;
    
    if (isNeckRegion) {
        angleP_gain = 0.8;
        angleD_gain = 0.5;
        maxAngleAdjustment = 3.0;
    }
    
    const double hemisphericalRadius = 52.5;
    
    static double integralError = 0.0;
    static double lastError = 0.0;
    static double lastAngleErrorX = 0.0;
    static double lastAngleErrorY = 0.0;
    
    double step = 3.0;
    
    g_forceFilterX.reset();
    g_forceFilterY.reset();
    g_forceFilterZ.reset();
    g_forceMagFilter.reset();
    g_angleXFilter.reset();
    g_angleYFilter.reset();
    
    g_lowForceCounter = 0;
    g_highForceCounter = 0;
    g_stableCounter = 0;
    g_lastValidZ = m_vCurrentPos[2];
    g_lastValidAngleX = m_vCurrentPos[3];
    g_lastValidAngleY = m_vCurrentPos[4];
    
    double dis2D = sqrt((next[0] - m_vCurrentPos[0]) * (next[0] - m_vCurrentPos[0]) + 
                        (next[1] - m_vCurrentPos[1]) * (next[1] - m_vCurrentPos[1]));
    int divid = dis2D / step;
    if (divid < 1) divid = 1;
    
    double currentZ = m_vCurrentPos[2];
    double xRaw = m_vCurrentPos[0];
    double yRaw = m_vCurrentPos[1];
    
    double currentAngleX = m_vCurrentPos[3];
    double currentAngleY = m_vCurrentPos[4];
    
    integralError = 0.0;
    lastError = 0.0;
    lastAngleErrorX = 0.0;
    lastAngleErrorY = 0.0;
    
    for (int i = 1; i < divid; ++i)
    {
        double xPos = xRaw + (next[0] - xRaw) * i / divid;
        double yPos = yRaw + (next[1] - yRaw) * i / divid;
        
        GetPose();
        
        double rawFx = m_vForces[0] - m_vRawForces[0];
        double rawFy = m_vForces[1] - m_vRawForces[1];
        double rawFz = m_vForces[2] - m_vRawForces[2];
        
        double filteredFx = g_forceFilterX.filter(rawFx);
        double filteredFy = g_forceFilterY.filter(rawFy);
        double filteredFz = g_forceFilterZ.filter(rawFz);
        
        Point3D filteredForce(filteredFx, filteredFy, filteredFz);
        double filteredMag = g_forceMagFilter.filter(filteredForce.Magnitude());
        
        double y_comp = 0.0;
        double p_comp = 0.0;
        
        if (filteredForce.x() > 10.0) y_comp = abs(filteredForce.x()) * 0.2;
        else if (filteredForce.x() > 1.0) y_comp = abs(filteredForce.x()) * 0.05;
        else if (filteredForce.x() < -10.0) y_comp = abs(filteredForce.x()) * -0.2;
        else if (filteredForce.x() < -1.0) y_comp = abs(filteredForce.x()) * -0.05;

        if (filteredForce.y() > 10.0) p_comp = abs(filteredForce.y()) * -0.2;
        else if (filteredForce.y() > 1.0) p_comp = abs(filteredForce.y()) * -0.05;
        else if (filteredForce.y() < -10.0) p_comp = abs(filteredForce.y()) * 0.2;
        else if (filteredForce.y() < -1.0) p_comp = abs(filteredForce.y()) * 0.05;

        double a = 0.0;
        double b = 0.0;
        
        if (m_vForces[3] < -0.1) a = -1.0 * abs(m_vForces[3]);
        else if (m_vForces[3] > 0.1) a = 6.0 * abs(m_vForces[3]);

        if (m_vForces[4] < -0.1) b = 6.0 * abs(m_vForces[4]);
        else if (m_vForces[4] > 0.1) b = -1.0 * abs(m_vForces[4]);
        
        double angleErrorX = 0.0;
        double angleErrorY = 0.0;
        
        if (needHemisphereFit) {
            if (abs(filteredFx) > 2.0) {
                angleErrorX = -filteredFx * angleP_gain;
            }
            if (abs(filteredFy) > 2.0) {
                angleErrorY = -filteredFy * angleP_gain;
            }
            
            double angleDerivativeX = lastAngleErrorX - angleErrorX;
            double angleDerivativeY = lastAngleErrorY - angleErrorY;
            lastAngleErrorX = angleErrorX;
            lastAngleErrorY = angleErrorY;
            
            angleErrorX += angleD_gain * angleDerivativeX;
            angleErrorY += angleD_gain * angleDerivativeY;
            
            angleErrorX = std::max(-maxAngleAdjustment, std::min(maxAngleAdjustment, angleErrorX));
            angleErrorY = std::max(-maxAngleAdjustment, std::min(maxAngleAdjustment, angleErrorY));
        }
        
        double baseAngleX = bodyBaseAngleX;
        double baseAngleY = 0.0;
        
        if (isNeckRegion) {
            baseAngleX = neckBaseAngleX;
            baseAngleY = 0.0;
        }
        
        if (currentxuewei == ZHIYANG_DETECTED && bFirst) {
            baseAngleX = bodyBaseAngleX;
            baseAngleY = 0.0;
            bFirst = false;
        }
        else if (!bFirst) {
            baseAngleX = bodyBaseAngleX;
            baseAngleY = 0.0;
        }
        
        double targetAngleX = baseAngleX + angleErrorX - y_comp - a;
        double targetAngleY = baseAngleY + angleErrorY - p_comp - b;
        
        double smoothedAngleX = g_angleXFilter.filter(targetAngleX);
        double smoothedAngleY = g_angleYFilter.filter(targetAngleY);
        
        double xAngle = smoothedAngleX;
        double yAngle = smoothedAngleY;
        double zAngle = m_vCurrentPos[5];

        xAngle = std::max(-200.0, std::min(-160.0, xAngle));
        yAngle = std::max(-15.0, std::min(15.0, yAngle));
        zAngle = std::max(150.0, std::min(210.0, zAngle));

        qDebug() << "Move to next: " << xPos << " " << yPos << " " << currentZ 
                 << " " << xAngle << " " << yAngle << " " << zAngle
                 << " Neck:" << isNeckRegion;
        
        MovL(xPos, yPos, currentZ, xAngle, yAngle, NORMAL_ANGLE);
        Wait_Done();
        
        m_vCurrentPos[0] = xPos;
        m_vCurrentPos[1] = yPos;
        m_vCurrentPos[2] = currentZ;
        m_vCurrentPos[3] = xAngle;
        m_vCurrentPos[4] = yAngle;
        m_vCurrentPos[5] = zAngle;

        m_CurrForce = Point3D(filteredFx, filteredFy, filteredFz);
        
        double error = filteredMag - targetForce;
        
        double deltaZ = 0.0;
        
        if (filteredMag > hysteresisHigh) {
            g_highForceCounter++;
            g_lowForceCounter = 0;
            g_stableCounter = 0;
            
            if (g_highForceCounter >= 2) {
                double excess = filteredMag - targetForce;
                if (excess > 5) {
                    deltaZ = 2.0;
                }
                else if (excess > 2) {
                    deltaZ = 1.0;
                }
                else {
                    deltaZ = 0.5;
                }
                g_highForceCounter = 0;
            }
        }
        else if (filteredMag < hysteresisLow) {
            g_lowForceCounter++;
            g_highForceCounter = 0;
            g_stableCounter = 0;
            
            if (g_lowForceCounter >= 2) {
                double deficit = targetForce - filteredMag;
                if (deficit > 5) {
                    deltaZ = -2.0;
                }
                else if (deficit > 2) {
                    deltaZ = -1.0;
                }
                else {
                    deltaZ = -0.5;
                }
                g_lowForceCounter = 0;
            }
        }
        else {
            g_stableCounter++;
            g_lowForceCounter = 0;
            g_highForceCounter = 0;
            
            if (g_stableCounter >= 3) {
                g_lastValidZ = currentZ;
            }
            
            integralError = 0.0;
        }
        
        if (needHemisphereFit && abs(filteredFx) > 8.0) {
            double hemisphericalCorrection = sqrt(hemisphericalRadius * hemisphericalRadius - 
                                                 (filteredFx * filteredFx / (angleP_gain * angleP_gain)));
            if (hemisphericalCorrection < hemisphericalRadius && hemisphericalRadius > 0) {
                deltaZ += (hemisphericalRadius - hemisphericalCorrection) * 0.05;
            }
        }
        
        currentZ += deltaZ;
        
        qDebug() << "Force: " << filteredMag << " | Fx: " << filteredFx << " | Fy: " << filteredFy
                 << " | Stable: " << g_stableCounter << " | Low: " << g_lowForceCounter << " | High: " << g_highForceCounter
                 << " | DeltaZ: " << deltaZ << " | Z: " << currentZ;
    }

    next[2] = currentZ;

    qDebug() << "Finish from one acu to other (Hemispherical v3) !";

    return true;
}

bool MyWindow::MoveToNextAcupointNew_ImprovedV5(int group, int row, DETECTED_XUEWEI currentxuewei,
    DETECTED_XUEWEI nextxuewei, std::vector<double>& next)
{
    bool bFirst = true;
    
    bool isNeckRegion = (currentxuewei == FENGFU_DETECTED || 
                         currentxuewei == FENGCHI_DETECTED || 
                         currentxuewei == DAZHUI_DETECTED ||
                         nextxuewei == FENGFU_DETECTED || 
                         nextxuewei == FENGCHI_DETECTED || 
                         nextxuewei == DAZHUI_DETECTED);
    
    bool isLeftSideRoute = (currentxuewei == ZUOJIANJING_DETECTED && nextxuewei == ZUOQIHAIYU_DETECTED);
    bool isRightSideRoute = (currentxuewei == YOUJIANJING_DETECTED && nextxuewei == YOUQIHAIYU_DETECTED);
    bool isShoulderToQiHai = isLeftSideRoute || isRightSideRoute;
    
    double dis2D = sqrt((next[0] - m_vCurrentPos[0]) * (next[0] - m_vCurrentPos[0]) + 
                        (next[1] - m_vCurrentPos[1]) * (next[1] - m_vCurrentPos[1]));
    
    bool forceUpright = isShoulderToQiHai && (dis2D > 50.0);
    
    bool needHemisphereFit = !isNeckRegion && !forceUpright;
    
    double neckBaseAngleX = -165.0;
    double bodyBaseAngleX = -178.0;
    
    const double hemisphericalRadius = 52.5;
    
    const double targetForce = 15.0;
    const double tolerance = 2.0;
    const double minForce = 12.0;
    const double maxForce = 18.0;
    
    const double hysteresisLow = 13.0;
    const double hysteresisHigh = 17.0;
    
    double angleP = 1.5;
    double angleD = 1.0;
    double maxAngleAdjust = 5.0;
    
    if (isNeckRegion) {
        angleP = 1.5;
        angleD = 1.0;
        maxAngleAdjust = 2.0;
    }
    else {
        angleP = 0.3;
        angleD = 0.2;
        maxAngleAdjust = 1.0;
    }
    
    double step = 3.0;
    
    g_forceFilterX.reset();
    g_forceFilterY.reset();
    g_forceFilterZ.reset();
    g_forceMagFilter.reset();
    g_angleXFilter.reset();
    g_angleYFilter.reset();
    
    g_lowForceCounter = 0;
    g_highForceCounter = 0;
    g_stableCounter = 0;
    g_lastValidZ = m_vCurrentPos[2];
    
    static double lastAngleErrorX = 0.0;
    static double lastAngleErrorY = 0.0;
    lastAngleErrorX = 0.0;
    lastAngleErrorY = 0.0;
    
    int divid = dis2D / step;
    if (divid < 1) divid = 1;
    
    double deltaZ = 0.0;
    double currentZ = m_vCurrentPos[2];
    double xRaw = m_vCurrentPos[0];
    double yRaw = m_vCurrentPos[1];
    
    double adaptiveMidForce = targetForce;
    int consecutiveAdjustments = 0;
    
    for (int i = 1; i < divid; ++i)
    {
        double xPos = xRaw + (next[0] - xRaw) * i / divid;
        double yPos = yRaw + (next[1] - yRaw) * i / divid;
        currentZ += deltaZ;
        
        GetPose();
        
        double rawFx = m_vForces[0] - m_vRawForces[0];
        double rawFy = m_vForces[1] - m_vRawForces[1];
        double rawFz = m_vForces[2] - m_vRawForces[2];
        
        double filteredFx = g_forceFilterX.filter(rawFx);
        double filteredFy = g_forceFilterY.filter(rawFy);
        double filteredFz = g_forceFilterZ.filter(rawFz);
        
        Point3D filteredForce(filteredFx, filteredFy, filteredFz);
        double filteredMag = g_forceMagFilter.filter(filteredForce.Magnitude());
        
        double y_comp = 0.0;
        double p_comp = 0.0;
        
        if (filteredForce.x() > 10.0) {
            y_comp = std::fabs(filteredForce.x()) * 0.2;
        }
        else if (filteredForce.x() > 1.0) {
            y_comp = std::fabs(filteredForce.x()) * 0.05;
        }
        else if (filteredForce.x() < -10.0) {
            y_comp = std::fabs(filteredForce.x()) * -0.2;
        }
        else if (filteredForce.x() < -1.0) {
            y_comp = std::fabs(filteredForce.x()) * -0.05;
        }

        if (filteredForce.y() > 10.0) {
            p_comp = std::fabs(filteredForce.y()) * -0.2;
        }
        else if (filteredForce.y() > 1.0) {
            p_comp = std::fabs(filteredForce.y()) * -0.05;
        }
        else if (filteredForce.y() < -10.0) {
            p_comp = std::fabs(filteredForce.y()) * 0.2;
        }
        else if (filteredForce.y() < -1.0) {
            p_comp = std::fabs(filteredForce.y()) * 0.05;
        }

        double a = 0.0;
        double b = 0.0;
        
        if (m_vForces[3] < -0.1) {
            a = -1.0 * std::fabs(m_vForces[3]);
        }
        else if (m_vForces[3] > 0.1) {
            a = 6.0 * std::fabs(m_vForces[3]);
        }

        if (m_vForces[4] < -0.1) {
            b = 6.0 * std::fabs(m_vForces[4]);
        }
        else if (m_vForces[4] > 0.1) {
            b = -1.0 * std::fabs(m_vForces[4]);
        }
        
        double angleAdjustX = 0.0;
        double angleAdjustY = 0.0;
        
        double currentDist = sqrt((xPos - xRaw) * (xPos - xRaw) + (yPos - yRaw) * (yPos - yRaw));
        bool forceUprightThisStep = isShoulderToQiHai && (currentDist > 100.0);
        bool needHemisphereFitThisStep = !isNeckRegion && !forceUprightThisStep;
        
        if (needHemisphereFitThisStep) {
            if (std::fabs(filteredFx) > 8.0) {
                angleAdjustX = -(filteredFx - std::fabs(filteredFx) / filteredFx * 8.0) * 0.1;
            }
            if (std::fabs(filteredFy) > 8.0) {
                angleAdjustY = -(filteredFy - std::fabs(filteredFy) / filteredFy * 8.0) * 0.1;
            }
            
            angleAdjustX = std::max(-maxAngleAdjust, std::min(maxAngleAdjust, angleAdjustX));
            angleAdjustY = std::max(-maxAngleAdjust, std::min(maxAngleAdjust, angleAdjustY));
        }
        
        double baseAngleX = bodyBaseAngleX;
        double baseAngleY = 0.0;
        
        if (forceUprightThisStep) {
            baseAngleX = -178.0;
            baseAngleY = 0.0;
        }
        else if (currentxuewei == ZHIYANG_DETECTED && bFirst) {
            baseAngleX = bodyBaseAngleX;
            baseAngleY = 0.0;
            bFirst = false;
        }
        else if (!bFirst) {
            baseAngleX = bodyBaseAngleX;
            baseAngleY = 0.0;
        }
        else if (isNeckRegion) {
            baseAngleX = neckBaseAngleX;
        }
        
        double targetAngleX, targetAngleY;
        if (forceUprightThisStep) {
            targetAngleX = -178.0;
            targetAngleY = 0.0;
        }
        else {
            targetAngleX = baseAngleX + angleAdjustX - y_comp - a;
            targetAngleY = baseAngleY + angleAdjustY - p_comp - b;
        }
        
        double xAngle = g_angleXFilter.filter(targetAngleX);
        double yAngle = g_angleYFilter.filter(targetAngleY);
        double zAngle = m_vCurrentPos[5];

        if (xAngle < -200.0) xAngle = -200.0;
        if (xAngle > -160.0) xAngle = -160.0;

        if (yAngle < -15.0) yAngle = -15.0;
        if (yAngle > 15.0) yAngle = 15.0;

        if (zAngle > 210.0) zAngle = 210.0;
        if (zAngle < 150.0) zAngle = 150.0;

        qDebug() << "Move to next: " << xPos << " " << yPos << " " << currentZ 
                 << " " << xAngle << " " << yAngle << " " << zAngle
                 << " Neck:" << isNeckRegion << " ForceUpright:" << forceUprightThisStep
                 << " Dist:" << currentDist;
        
        MovL(xPos, yPos, currentZ, xAngle, yAngle, NORMAL_ANGLE);

        Wait_Done();
        
        m_vCurrentPos[0] = xPos;
        m_vCurrentPos[1] = yPos;
        m_vCurrentPos[2] = currentZ;
        m_vCurrentPos[3] = xAngle;
        m_vCurrentPos[4] = yAngle;
        m_vCurrentPos[5] = zAngle;

        m_CurrForce = Point3D(filteredFx, filteredFy, filteredFz);
        
        qDebug() << "Force mag: " << filteredMag << " | Fx:" << filteredFx << " Fy:" << filteredFy;
        
        deltaZ = 0.0;
        
        double safeZoneMin = adaptiveMidForce - tolerance;
        double safeZoneMax = adaptiveMidForce + tolerance;
        
        if (filteredMag > hysteresisHigh) {
            g_highForceCounter++;
            g_lowForceCounter = 0;
            g_stableCounter = 0;
            
            if (g_highForceCounter >= 2) {
                double excess = filteredMag - targetForce;
                if (excess > 5) {
                    deltaZ = 2.0;
                }
                else if (excess > 2) {
                    deltaZ = 1.0;
                }
                else {
                    deltaZ = 0.5;
                }
                g_highForceCounter = 0;
                consecutiveAdjustments++;
            }
        }
        else if (filteredMag < hysteresisLow) {
            g_lowForceCounter++;
            g_highForceCounter = 0;
            g_stableCounter = 0;
            
            if (g_lowForceCounter >= 2) {
                if (g_lastValidZ == 0.0) {
                    g_lastValidZ = currentZ;
                }
                
                double deficit = targetForce - filteredMag;
                if (deficit > 5) {
                    deltaZ = -2.0;
                }
                else if (deficit > 2) {
                    deltaZ = -1.0;
                }
                else {
                    deltaZ = -0.5;
                }
                g_lowForceCounter = 0;
                consecutiveAdjustments++;
            }
        }
        else if (filteredMag >= safeZoneMin && filteredMag <= safeZoneMax) {
            g_stableCounter++;
            g_lowForceCounter = 0;
            g_highForceCounter = 0;
            
            if (g_stableCounter >= 3) {
                g_lastValidZ = currentZ;
                
                if (consecutiveAdjustments > 8) {
                    adaptiveMidForce = adaptiveMidForce * 0.95 + filteredMag * 0.05;
                }
                consecutiveAdjustments = 0;
            }
            
            deltaZ = 0.0;
        }
        else {
            g_lowForceCounter = 0;
            g_highForceCounter = 0;
            g_stableCounter = 0;
            
            if (filteredMag > adaptiveMidForce && filteredMag < maxForce) {
                double excess = filteredMag - adaptiveMidForce;
                if (excess > 3) {
                    deltaZ = 0.8;
                }
                else {
                    deltaZ = 0.4;
                }
                consecutiveAdjustments++;
            }
            else if (filteredMag < adaptiveMidForce && filteredMag > minForce) {
                double deficit = adaptiveMidForce - filteredMag;
                if (deficit > 3) {
                    deltaZ = -0.8;
                }
                else {
                    deltaZ = -0.4;
                }
                consecutiveAdjustments++;
            }
        }
        
        if (needHemisphereFit && std::fabs(filteredFx) > 6.0) {
            double lateralForce = std::fabs(filteredFx);
            double hemisphereDepth = hemisphericalRadius - std::sqrt(
                std::max(0.0, hemisphericalRadius * hemisphericalRadius - 
                        (lateralForce * lateralForce / (angleP * angleP))));
            if (hemisphereDepth > 0 && hemisphereDepth < hemisphericalRadius) {
                deltaZ += hemisphereDepth * 0.03;
            }
        }
        
        if (deltaZ != 0.0) {
            currentZ += deltaZ;
            m_vCurrentPos[2] = currentZ;
        }

        qDebug() << "Delta Z: " << deltaZ << " | Stable: " << g_stableCounter 
                 << " | Low: " << g_lowForceCounter << " | High: " << g_highForceCounter
                 << " | Adaptive Mid: " << adaptiveMidForce;
    }

    next[2] = currentZ;

    qDebug() << "Finish from one acu to other (V5) !";
    qDebug() << "Last valid Z: " << g_lastValidZ;

    return true;
}

bool MyWindow::MoveToNextAcupointNew_ImprovedV6(int group, int row, DETECTED_XUEWEI currentxuewei,
    DETECTED_XUEWEI nextxuewei, std::vector<double>& next,int level)
{
    bool bFirst = true;
    double maxForce = 20.0;
    double midForce = 10.0;
    double touchForce = 5.0;
    
    double deadZoneLow = 3.0;
    double deadZoneHigh = 3.0;
    
    double step = 3.0;
    qDebug() << "speed is " << step;
    
    bool isLeftSideRoute = (currentxuewei == ZUOJIANJING_DETECTED && nextxuewei == ZUOQIHAIYU_DETECTED);
    bool isRightSideRoute = (currentxuewei == YOUJIANJING_DETECTED && nextxuewei == YOUQIHAIYU_DETECTED);
    bool isShoulderToQiHaiRoute = isLeftSideRoute || isRightSideRoute;
    
    double forceUprightDistance = 100.0;
    double totalTraveledDistance = 0.0;
    bool isUpright = false;
    
    g_forceFilterX.reset();
    g_forceFilterY.reset();
    g_forceFilterZ.reset();
    g_forceMagFilter.reset();
    g_lowForceCounter = 0;
    g_highForceCounter = 0;
    g_stableCounter = 0;
    
    double startX = m_vCurrentPos[0];
    double startY = m_vCurrentPos[1];
    
    double dis2D = sqrt((next[0] - m_vCurrentPos[0]) * (next[0] - m_vCurrentPos[0]) + 
                        (next[1] - m_vCurrentPos[1]) * (next[1] - m_vCurrentPos[1]));
    int divid = dis2D / step;
    if (divid < 1) divid = 1;
    
    double deltaZ = 0.0;
    double currentZ = m_vCurrentPos[2];
    double xRaw = m_vCurrentPos[0];
    double yRaw = m_vCurrentPos[1];
    
    double adaptiveMidForce = midForce;
    int consecutiveAdjustments = 0;
    
    for (int i = 1; i < divid; ++i)
    {
        double xPos = xRaw + (next[0] - xRaw) * i / divid;
        double yPos = yRaw + (next[1] - yRaw) * i / divid;
        currentZ += deltaZ;
        
        if (isShoulderToQiHaiRoute && !isUpright) {
            double stepDistance = sqrt((xPos - m_vCurrentPos[0]) * (xPos - m_vCurrentPos[0]) + 
                                      (yPos - m_vCurrentPos[1]) * (yPos - m_vCurrentPos[1]));
            totalTraveledDistance += stepDistance;
            
            if (totalTraveledDistance >= forceUprightDistance) {
                isUpright = true;
                qDebug() << "Distance threshold reached: " << totalTraveledDistance 
                         << "mm, switching to upright posture";
            }
        }
        
        GetPose();
        
        double rawFx = m_vForces[0] - m_vRawForces[0];
        double rawFy = m_vForces[1] - m_vRawForces[1];
        double rawFz = m_vForces[2] - m_vRawForces[2];
        
        double filteredFx = g_forceFilterX.filter(rawFx);
        double filteredFy = g_forceFilterY.filter(rawFy);
        double filteredFz = g_forceFilterZ.filter(rawFz);
        
        Point3D filteredForce(filteredFx, filteredFy, filteredFz);
        double filteredMag = g_forceMagFilter.filter(filteredForce.Magnitude());
        
        double y = 0.0;
        double p = 0.0;
        double r = 0.0;
        
        if (!isUpright) {
            if (filteredForce.x() > 10.0) {
                y = std::fabs(filteredForce.x()) * 0.2;
            }
            else if (filteredForce.x() > 1.0) {
                y = std::fabs(filteredForce.x()) * 0.05;
            }
            else if (filteredForce.x() < -10.0) {
                y = std::fabs(filteredForce.x()) * -0.2;
            }
            else if (filteredForce.x() < -1.0) {
                y = std::fabs(filteredForce.x()) * -0.05;
            }

            if (filteredForce.y() > 10.0) {
                p = std::fabs(filteredForce.y()) * -0.2;
            }
            else if (filteredForce.y() > 1.0) {
                p = std::fabs(filteredForce.y()) * -0.05;
            }
            else if (filteredForce.y() < -10.0) {
                p = std::fabs(filteredForce.y()) * 0.2;
            }
            else if (filteredForce.y() < -1.0) {
                p = std::fabs(filteredForce.y()) * 0.05;
            }

            double a = 0.0;
            double b = 0.0;
            
            if (m_vForces[3] < -0.1) {
                a = -1.0 * std::fabs(m_vForces[3]);
            }
            else if (m_vForces[3] > 0.1) {
                a = 6.0 * std::fabs(m_vForces[3]);
            }

            if (m_vForces[4] < -0.1) {
                b = 6.0 * std::fabs(m_vForces[4]);
            }
            else if (m_vForces[4] > 0.1) {
                b = -1.0 * std::fabs(m_vForces[4]);
            }

            double xAngle = m_vCurrentPos[3] - y - a;
            double yAngle = m_vCurrentPos[4] - p - b;
            double zAngle = m_vCurrentPos[5] - r;

            if (xAngle < -200.0) xAngle = -200.0;
            if (xAngle > -160.0) xAngle = -160.0;

            if (yAngle < -15.0) yAngle = -15.0;
            if (yAngle > 15.0) yAngle = 15.0;

            if (zAngle > 210.0) zAngle = 210.0;
            if (zAngle < 150.0) zAngle = 150.0;

            if (currentxuewei == ZHIYANG_DETECTED && bFirst) {
                xAngle = -178.0;
                yAngle = 0.0;
                bFirst = false;
            }

            if (!bFirst) {
                xAngle = -178.0;
                yAngle = 0.0;
            }

            qDebug() << "Move to next: " << xPos << " " << yPos << " " << currentZ 
                     << " " << xAngle << " " << yAngle << " " << zAngle
                     << " Upright:" << isUpright;
            
            MovL(xPos, yPos, currentZ, xAngle, yAngle, NORMAL_ANGLE);
        }
        else {
            double xAngle = -178.0;
            double yAngle = 0.0;
            double zAngle = m_vCurrentPos[5];

            if (zAngle > 210.0) zAngle = 210.0;
            if (zAngle < 150.0) zAngle = 150.0;

            qDebug() << "Move to next (upright): " << xPos << " " << yPos << " " << currentZ 
                     << " " << xAngle << " " << yAngle << " " << zAngle;
            
            MovL(xPos, yPos, currentZ, xAngle, yAngle, NORMAL_ANGLE);
        }

        Wait_Done();
        
        m_vCurrentPos[0] = xPos;
        m_vCurrentPos[1] = yPos;
        m_vCurrentPos[2] = currentZ;
        
        if (isUpright) {
            m_vCurrentPos[3] = -178.0;
            m_vCurrentPos[4] = 0.0;
        }
        
        m_CurrForce = Point3D(filteredFx, filteredFy, filteredFz);
        
        qDebug() << "Force mag is : " << filteredMag;
        qDebug() << "Detected force is : " << filteredFx << " " << filteredFy << " " << filteredFz 
                 << " " << group << " " << row;
        
        deltaZ = 0.0;
        
        double safeZoneMin = adaptiveMidForce - deadZoneLow;
        double safeZoneMax = adaptiveMidForce + deadZoneHigh;
        
        if (filteredMag > maxForce) {
            g_highForceCounter++;
            g_lowForceCounter = 0;
            g_stableCounter = 0;
            
            if (g_highForceCounter >= 2) {
                double excess = filteredMag - maxForce;
                if (excess > 20) {
                    deltaZ = 4.0;
                }
                else if (excess > 10) {
                    deltaZ = 2.5;
                }
                else {
                    deltaZ = 1.5;
                }
                g_highForceCounter = 0;
                consecutiveAdjustments++;
            }
        }
        else if (filteredMag < touchForce) {
            g_lowForceCounter++;
            g_highForceCounter = 0;
            g_stableCounter = 0;
            
            if (g_lowForceCounter >= 3) {
                if (g_lastValidZ == 0.0) {
                    g_lastValidZ = currentZ;
                }
                
                double deficit = touchForce - filteredMag;
                if (deficit > 15) {
                    deltaZ = -2.5;
                }
                else if (deficit > 8) {
                    deltaZ = -1.5;
                }
                else {
                    deltaZ = -0.8;
                }
                g_lowForceCounter = 0;
                consecutiveAdjustments++;
            }
        }
        else if (filteredMag >= safeZoneMin && filteredMag <= safeZoneMax) {
            g_stableCounter++;
            g_lowForceCounter = 0;
            g_highForceCounter = 0;
            
            if (g_stableCounter >= 3) {
                g_lastValidZ = currentZ;
                
                if (consecutiveAdjustments > 5) {
                    adaptiveMidForce = adaptiveMidForce * 0.95 + filteredMag * 0.05;
                }
                consecutiveAdjustments = 0;
            }
            
            deltaZ = 0.0;
        }
        else {
            g_lowForceCounter = 0;
            g_highForceCounter = 0;
            g_stableCounter = 0;
            
            if (filteredMag > adaptiveMidForce && filteredMag < maxForce) {
                double excess = filteredMag - adaptiveMidForce;
                if (excess > 5) {
                    deltaZ = 1.0;
                }
                else {
                    deltaZ = 0.5;
                }
                consecutiveAdjustments++;
            }
            else if (filteredMag < adaptiveMidForce && filteredMag > touchForce) {
                double deficit = adaptiveMidForce - filteredMag;
                if (deficit > 5) {
                    deltaZ = -1.0;
                }
                else {
                    deltaZ = -0.5;
                }
                consecutiveAdjustments++;
            }
        }

        if (deltaZ != 0.0) {
            currentZ += deltaZ;
            m_vCurrentPos[2] = currentZ;
        }

        qDebug() << "Delta Z: " << deltaZ << " | Stable: " << g_stableCounter 
                 << " | Low: " << g_lowForceCounter << " | High: " << g_highForceCounter
                 << " | Adaptive Mid: " << adaptiveMidForce;
    }

    next[2] = currentZ;

    qDebug() << "Finish from shoulder to QiHaiYu !";
    qDebug() << "Last valid Z: " << g_lastValidZ;
    qDebug() << "Total traveled distance: " << totalTraveledDistance;
    qDebug() << "Upright mode activated: " << isUpright;

    return true;
}
