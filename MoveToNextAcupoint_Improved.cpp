#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <cmath>
#include <algorithm>
#include <cstdlib>

using namespace std;

#include "MyWindow.h"
#include "PhysicalTherapyRobot.h"
#include "Communicate.h"
#include "AdmittanceControl.h"

#include <QTime>
#include <QStringLiteral>
#include <QDebug>

class LowPassFilter {
private:
    deque<double> m_buffer;
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

bool MyWindow::MoveToNextAcupointNew_ImprovedV6(int group, int row, DETECTED_XUEWEI currentxuewei,
    DETECTED_XUEWEI nextxuewei, vector<double>& next, int ration)
{
    bool bFirst = true;
    double maxForce = 20.0;
    double midForce = 10.0;
    double touchForce = 5.0;

    double deadZoneLow = 3.0;
    double deadZoneHigh = 3.0;

    double step = 3.0 * ration;
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
                y = fabs(filteredForce.x()) * 0.2;
            }
            else if (filteredForce.x() > 1.0) {
                y = fabs(filteredForce.x()) * 0.05;
            }
            else if (filteredForce.x() < -10.0) {
                y = fabs(filteredForce.x()) * -0.2;
            }
            else if (filteredForce.x() < -1.0) {
                y = fabs(filteredForce.x()) * -0.05;
            }

            if (filteredForce.y() > 10.0) {
                p = fabs(filteredForce.y()) * -0.2;
            }
            else if (filteredForce.y() > 1.0) {
                p = fabs(filteredForce.y()) * -0.05;
            }
            else if (filteredForce.y() < -10.0) {
                p = fabs(filteredForce.y()) * 0.2;
            }
            else if (filteredForce.y() < -1.0) {
                p = fabs(filteredForce.y()) * 0.05;
            }

            double a = 0.0;
            double b = 0.0;

            if (m_vForces[3] < -0.1) {
                a = -1.0 * fabs(m_vForces[3]);
            }
            else if (m_vForces[3] > 0.1) {
                a = 6.0 * fabs(m_vForces[3]);
            }

            if (m_vForces[4] < -0.1) {
                b = 6.0 * fabs(m_vForces[4]);
            }
            else if (m_vForces[4] > 0.1) {
                b = -1.0 * fabs(m_vForces[4]);
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

    double minZHeight = 50.0;
    if (currentZ < minZHeight) {
        currentZ = minZHeight;
        m_vCurrentPos[2] = currentZ;
        qDebug() << "Adjusting initial Z to safe height: " << currentZ;
    }

    double adaptiveMidForce = midForce;
    int consecutiveAdjustments = 0;

    // Initialize variables for shoulder-to-abdomen routes
    bool isLeftSideRoute = (currentxuewei == ZUOJIANJING_DETECTED && nextxuewei == ZUOQIHAIYU_DETECTED);
    bool isRightSideRoute = (currentxuewei == YOUJIANJING_DETECTED && nextxuewei == YOUQIHAIYU_DETECTED);
    bool isShoulderToQiHaiRoute = isLeftSideRoute || isRightSideRoute;
    
    double forceUprightDistance = 100.0;
    double totalTraveledDistance = 0.0;
    bool isUpright = false;
    
    int highForceCounter = 0;
    int lowForceCounter = 0;
    int stableCounter = 0;
    double lastValidZ = m_vCurrentPos[2];

    for (int i = 1; i < divid; ++i)
    {
        double xPos = xRaw + (next[0] - xRaw) * i / divid;
        double yPos = yRaw + (next[1] - yRaw) * i / divid;
        currentZ += deltaZ;

        if (currentZ < minZHeight) {
            currentZ = minZHeight;
        }

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
                y = fabs(filteredForce.x()) * 0.2;
            }
            else if (filteredForce.x() > 1.0) {
                y = fabs(filteredForce.x()) * 0.05;
            }
            else if (filteredForce.x() < -10.0) {
                y = fabs(filteredForce.x()) * -0.2;
            }
            else if (filteredForce.x() < -1.0) {
                y = fabs(filteredForce.x()) * -0.05;
            }

            if (filteredForce.y() > 10.0) {
                p = fabs(filteredForce.y()) * -0.2;
            }
            else if (filteredForce.y() > 1.0) {
                p = fabs(filteredForce.y()) * -0.05;
            }
            else if (filteredForce.y() < -10.0) {
                p = fabs(filteredForce.y()) * 0.2;
            }
            else if (filteredForce.y() < -1.0) {
                p = fabs(filteredForce.y()) * 0.05;
            }

            double a = 0.0;
            double b = 0.0;

            if (m_vForces[3] < -0.1) {
                a = -1.0 * fabs(m_vForces[3]);
            }
            else if (m_vForces[3] > 0.1) {
                a = 6.0 * fabs(m_vForces[3]);
            }

            if (m_vForces[4] < -0.1) {
                b = 6.0 * fabs(m_vForces[4]);
            }
            else if (m_vForces[4] > 0.1) {
                b = -1.0 * fabs(m_vForces[4]);
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
            highForceCounter++;
            lowForceCounter = 0;
            stableCounter = 0;

            if (highForceCounter >= 2) {
                double excess = filteredMag - maxForce;
                double baseDelta = 1.5;
                if (excess > 20) {
                    baseDelta = 4.0;
                }
                else if (excess > 10) {
                    baseDelta = 2.5;
                }
                else {
                    baseDelta = 1.5;
                }
                deltaZ = baseDelta;
                highForceCounter = 0;
                consecutiveAdjustments++;
            }
        }
        else if (filteredMag < touchForce) {
            lowForceCounter++;
            highForceCounter = 0;
            stableCounter = 0;

            if (lowForceCounter >= 3) {
                if (lastValidZ == 0.0) {
                    lastValidZ = currentZ;
                }

                double deficit = touchForce - filteredMag;
                double baseDelta = -0.8;
                if (deficit > 15) {
                    baseDelta = -2.5;
                }
                else if (deficit > 8) {
                    baseDelta = -1.5;
                }
                else {
                    baseDelta = -0.8;
                }
                deltaZ = baseDelta;
                lowForceCounter = 0;
                consecutiveAdjustments++;
            }
        }
        else if (filteredMag >= safeZoneMin && filteredMag <= safeZoneMax) {
            stableCounter++;
            lowForceCounter = 0;
            highForceCounter = 0;

            if (stableCounter >= 3) {
                lastValidZ = currentZ;

                if (consecutiveAdjustments > 5) {
                    adaptiveMidForce = adaptiveMidForce * 0.95 + filteredMag * 0.05;
                }
                consecutiveAdjustments = 0;
            }

            deltaZ = 0.0;
        }
        else {
            lowForceCounter = 0;
            highForceCounter = 0;
            stableCounter = 0;

            if (filteredMag > adaptiveMidForce && filteredMag < maxForce) {
                double excess = filteredMag - adaptiveMidForce;
                double baseDelta = 0.5;
                if (excess > 8) {
                    baseDelta = 1.5;
                }
                else if (excess > 3) {
                    baseDelta = 1.0;
                }
                deltaZ = baseDelta;
                consecutiveAdjustments++;
            }
            else if (filteredMag < adaptiveMidForce && filteredMag > touchForce) {
                double deficit = adaptiveMidForce - filteredMag;
                double baseDelta = -0.5;
                if (deficit > 8) {
                    baseDelta = -1.5;
                }
                else if (deficit > 3) {
                    baseDelta = -1.0;
                }
                deltaZ = baseDelta;
                consecutiveAdjustments++;
            }
        }

        if (deltaZ != 0.0) {
            currentZ += deltaZ;
            m_vCurrentPos[2] = currentZ;
        }

        qDebug() << "Delta Z: " << deltaZ << " | Stable: " << stableCounter 
            << " | Low: " << lowForceCounter << " | High: " << highForceCounter
            << " | Adaptive Mid: " << adaptiveMidForce;
    }

    next[2] = currentZ;

    qDebug() << "Finish from shoulder to QiHaiYu (V6 Optimized) !";
    qDebug() << "Last valid Z: " << g_lastValidZ;
    qDebug() << "Total traveled distance: " << totalTraveledDistance;
    qDebug() << "Upright mode activated: " << isUpright;

    return true;
}

bool MyWindow::MoveToNextAcupointNew_ImprovedV9(int group, int row, DETECTED_XUEWEI currentxuewei,
    DETECTED_XUEWEI nextxuewei, vector<double>& next, int ration)
{
    bool bFirst = true;
    
    // Adaptive force thresholds based on ration
    double maxForce = 20.0 + (ration - 1) * 2.0;  // Increase for higher ration
    double midForce = 10.0 + (ration - 1) * 1.0;  // Increase for higher ration
    double touchForce = 5.0 + (ration - 1) * 0.5;  // Increase for higher ration

    // Adaptive dead zone based on ration
    double deadZoneLow = 3.0 + (ration - 1) * 0.5;
    double deadZoneHigh = 3.0 + (ration - 1) * 0.5;

    // Adaptive step size - limit maximum step size for higher ration
    double step = 3.0 * ration;
    if (step > 10.0) step = 10.0;  // Cap maximum step size
    qDebug() << "V9 speed is " << step;

    g_forceFilterX.reset();
    g_forceFilterY.reset();
    g_forceFilterZ.reset();
    g_forceMagFilter.reset();
    g_lowForceCounter = 0;
    g_highForceCounter = 0;
    g_stableCounter = 0;

    double dis2D = sqrt((next[0] - m_vCurrentPos[0]) * (next[0] - m_vCurrentPos[0]) +
        (next[1] - m_vCurrentPos[1]) * (next[1] - m_vCurrentPos[1]));

    int baseDivid = dis2D / step;
    if (baseDivid < 1) baseDivid = 1;

    int divid = baseDivid;
    if (ration >= 2) {
        divid = baseDivid * ration;
        if (divid > 200) divid = 200;
    }
    qDebug() << "V9 divid: " << divid << " base: " << baseDivid;

    double deltaZ = 0.0;
    double currentZ = m_vCurrentPos[2];
    double xRaw = m_vCurrentPos[0];
    double yRaw = m_vCurrentPos[1];

    double adaptiveMidForce = midForce;
    int consecutiveAdjustments = 0;

    double minZHeight = 50.0;

    // Adaptive parameters based on ration
    int highForceThreshold = 2 + (ration - 1);  // More readings for higher ration
    int lowForceThreshold = 3 + (ration - 1);   // More readings for higher ration
    int stableThreshold = 3 + (ration - 1) / 2; // Slightly more readings for higher ration
    double zScaleFactor = 1.0 / (1.0 + (ration - 1) * 0.3);  // Smaller adjustments for higher ration

    for (int i = 1; i < divid; ++i)
    {
        double xPos = xRaw + (next[0] - xRaw) * i / divid;
        double yPos = yRaw + (next[1] - yRaw) * i / divid;
        currentZ += deltaZ;

        if (currentZ < minZHeight) {
            currentZ = minZHeight;
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

        if (filteredForce.x() > 10.0) {
            y = fabs(filteredForce.x()) * 0.2;
        }
        else if (filteredForce.x() > 1.0) {
            y = fabs(filteredForce.x()) * 0.05;
        }
        else if (filteredForce.x() < -10.0) {
            y = fabs(filteredForce.x()) * -0.2;
        }
        else if (filteredForce.x() < -1.0) {
            y = fabs(filteredForce.x()) * -0.05;
        }

        if (filteredForce.y() > 10.0) {
            p = fabs(filteredForce.y()) * -0.2;
        }
        else if (filteredForce.y() > 1.0) {
            p = fabs(filteredForce.y()) * -0.05;
        }
        else if (filteredForce.y() < -10.0) {
            p = fabs(filteredForce.y()) * 0.2;
        }
        else if (filteredForce.y() < -1.0) {
            p = fabs(filteredForce.y()) * 0.05;
        }

        double a = 0.0;
        double b = 0.0;

        if (m_vForces[3] < -0.1) {
            a = -1.0 * fabs(m_vForces[3]);
        }
        else if (m_vForces[3] > 0.1) {
            a = 6.0 * fabs(m_vForces[3]);
        }

        if (m_vForces[4] < -0.1) {
            b = 6.0 * fabs(m_vForces[4]);
        }
        else if (m_vForces[4] > 0.1) {
            b = -1.0 * fabs(m_vForces[4]);
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

        qDebug() << "V9 Move: " << xPos << " " << yPos << " " << currentZ;

        MovL(xPos, yPos, currentZ, xAngle, yAngle, NORMAL_ANGLE);

        Wait_Done();

        m_vCurrentPos[0] = xPos;
        m_vCurrentPos[1] = yPos;
        m_vCurrentPos[2] = currentZ;

        m_CurrForce = Point3D(filteredFx, filteredFy, filteredFz);

        qDebug() << "V9 Force: " << filteredMag;

        deltaZ = 0.0;

        double safeZoneMin = adaptiveMidForce - deadZoneLow;
        double safeZoneMax = adaptiveMidForce + deadZoneHigh;

        if (filteredMag > maxForce) {
            g_highForceCounter++;
            g_lowForceCounter = 0;
            g_stableCounter = 0;

            if (g_highForceCounter >= highForceThreshold) {
                double excess = filteredMag - maxForce;
                if (excess > 20) {
                    deltaZ = 4.0 * zScaleFactor;
                }
                else if (excess > 10) {
                    deltaZ = 2.5 * zScaleFactor;
                }
                else {
                    deltaZ = 1.5 * zScaleFactor;
                }
                g_highForceCounter = 0;
                consecutiveAdjustments++;
            }
        }
        else if (filteredMag < touchForce) {
            g_lowForceCounter++;
            g_highForceCounter = 0;
            g_stableCounter = 0;

            if (g_lowForceCounter >= lowForceThreshold) {
                if (g_lastValidZ == 0.0) {
                    g_lastValidZ = currentZ;
                }

                double deficit = touchForce - filteredMag;
                if (deficit > 15) {
                    deltaZ = -2.5 * zScaleFactor;
                }
                else if (deficit > 8) {
                    deltaZ = -1.5 * zScaleFactor;
                }
                else {
                    deltaZ = -0.8 * zScaleFactor;
                }
                g_lowForceCounter = 0;
                consecutiveAdjustments++;
            }
        }
        else if (filteredMag >= safeZoneMin && filteredMag <= safeZoneMax) {
            g_stableCounter++;
            g_lowForceCounter = 0;
            g_highForceCounter = 0;

            if (g_stableCounter >= stableThreshold) {
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
                    deltaZ = 1.0 * zScaleFactor;
                }
                else {
                    deltaZ = 0.5 * zScaleFactor;
                }
                consecutiveAdjustments++;
            }
            else if (filteredMag < adaptiveMidForce && filteredMag > touchForce) {
                double deficit = adaptiveMidForce - filteredMag;
                if (deficit > 5) {
                    deltaZ = -1.0 * zScaleFactor;
                }
                else {
                    deltaZ = -0.5 * zScaleFactor;
                }
                consecutiveAdjustments++;
            }
        }

        if (deltaZ != 0.0) {
            currentZ += deltaZ;
            m_vCurrentPos[2] = currentZ;
        }

        qDebug() << "V9 Delta Z: " << deltaZ;
    }

    next[2] = currentZ;

    qDebug() << "V9 Finish!";

    return true;
}

bool MyWindow::MoveToNextAcupointNew_ImprovedV10(int group, int row, DETECTED_XUEWEI currentxuewei,
    DETECTED_XUEWEI nextxuewei, vector<double>& next, int ration)
{
    // V10 function variables declaration
    bool bFirst = true;
    int consecutiveAdjustments = 0;
    
    // Force control parameters
    double maxForce = 15.0;
    double warningForce = 12.0;
    double midForce = 8.0;
    double touchForce = 3.0;
    double deadZoneLow = 4.0;
    double deadZoneHigh = 4.0;
    
    if (ration >= 3) {
        maxForce = 15.0 + (ration - 2) * 1.0;
        warningForce = 12.0 + (ration - 2) * 0.8;
        midForce = 8.0 + (ration - 2) * 0.5;
        touchForce = 3.0 + (ration - 2) * 0.2;
        deadZoneLow = 4.0 + (ration - 2) * 0.5;
        deadZoneHigh = 4.0 + (ration - 2) * 0.5;
    }

    // Threshold parameters
    int highForceThreshold = 3;
    int lowForceThreshold = 4;
    int stableThreshold = 4;
    
    if (ration >= 3) {
        highForceThreshold = 3 + (ration - 2);
        lowForceThreshold = 4 + (ration - 2);
        stableThreshold = 4 + (ration - 2);
    }

    double step = 3.0 * ration;
    double maxStep = 15.0;
    if (ration >= 3) {
        maxStep = 6.0 + (ration - 3) * 1.0;
    }
    if (step > maxStep) {
        step = maxStep;
    }
    qDebug() << "V10 speed is " << step;
    // V10_FUNCTION_MARKER

    bool isLeftSideRoute = false;
    bool isRightSideRoute = false;
    if (currentxuewei == ZUOJIANJING_DETECTED && nextxuewei == ZUOQIHAIYU_DETECTED) {
        isLeftSideRoute = true;
    }
    if (currentxuewei == YOUJIANJING_DETECTED && nextxuewei == YOUQIHAIYU_DETECTED) {
        isRightSideRoute = true;
    }
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
    
    int divid = (int)(dis2D / step);
    if (divid < 1) divid = 1;
    
    if (ration >= 3) {
        divid = (int)(divid * (1.0 + (ration - 2) * 0.3));
        if (divid > 300) divid = 300;
    }

    double deltaZ = 0.0;
    double currentZ = m_vCurrentPos[2];
    double xRaw = m_vCurrentPos[0];
    double yRaw = m_vCurrentPos[1];

    double adaptiveMidForce = midForce;

    // Z-axis adjustment optimization
    double zAdjustScale = 0.6;
    if (ration >= 3) {
        zAdjustScale = 0.4 - (ration - 3) * 0.08;
    }

    for (int i = 1; i < divid; ++i) {
        double xPos = xRaw + (next[0] - xRaw) * i / divid;
        double yPos = yRaw + (next[1] - yRaw) * i / divid;
        currentZ += deltaZ;
        
        // Get filtered force magnitude
        double rawFx = m_vForces[0] - m_vRawForces[0];
        double rawFy = m_vForces[1] - m_vRawForces[1];
        double rawFz = m_vForces[2] - m_vRawForces[2];
        
        double filteredFx = g_forceFilterX.filter(rawFx);
        double filteredFy = g_forceFilterY.filter(rawFy);
        double filteredFz = g_forceFilterZ.filter(rawFz);
        
        Point3D filteredForce(filteredFx, filteredFy, filteredFz);
        double filteredMag = g_forceMagFilter.filter(filteredForce.Magnitude());

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

        //double rawFx = m_vForces[0] - m_vRawForces[0];
        //double rawFy = m_vForces[1] - m_vRawForces[1];
        //double rawFz = m_vForces[2] - m_vRawForces[2];

        //double filteredFx = g_forceFilterX.filter(rawFx);
        //double filteredFy = g_forceFilterY.filter(rawFy);
        //double filteredFz = g_forceFilterZ.filter(rawFz);

        //Point3D filteredForce(filteredFx, filteredFy, filteredFz);
        //double filteredMag = g_forceMagFilter.filter(filteredForce.Magnitude());

        double y = 0.0;
        double p = 0.0;
        double r = 0.0;

        if (!isUpright) {
            if (filteredForce.x() > 10.0) {
                y = fabs(filteredForce.x()) * 0.2;
            }
            else if (filteredForce.x() > 1.0) {
                y = fabs(filteredForce.x()) * 0.05;
            }
            else if (filteredForce.x() < -10.0) {
                y = fabs(filteredForce.x()) * -0.2;
            }
            else if (filteredForce.x() < -1.0) {
                y = fabs(filteredForce.x()) * -0.05;
            }

            if (filteredForce.y() > 10.0) {
                p = fabs(filteredForce.y()) * -0.2;
            }
            else if (filteredForce.y() > 1.0) {
                p = fabs(filteredForce.y()) * -0.05;
            }
            else if (filteredForce.y() < -10.0) {
                p = fabs(filteredForce.y()) * 0.2;
            }
            else if (filteredForce.y() < -1.0) {
                p = fabs(filteredForce.y()) * 0.05;
            }

            double a = 0.0;
            double b = 0.0;

            if (m_vForces[3] < -0.1) {
                a = -1.0 * fabs(m_vForces[3]);
            }
            else if (m_vForces[3] > 0.1) {
                a = 6.0 * fabs(m_vForces[3]);
            }

            if (m_vForces[4] < -0.1) {
                b = 6.0 * fabs(m_vForces[4]);
            }
            else if (m_vForces[4] > 0.1) {
                b = -1.0 * fabs(m_vForces[4]);
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

        // === 优化的力控制逻辑 - 增加警告力检查 ===
        if (filteredMag > maxForce) {
            // 超过最大力，紧急调整
            g_highForceCounter++;
            g_lowForceCounter = 0;
            g_stableCounter = 0;

            if (g_highForceCounter >= highForceThreshold) {
                double excess = filteredMag - maxForce;
                if (excess > 15) {
                    deltaZ = 2.0 * zAdjustScale;
                }
                else if (excess > 8) {
                    deltaZ = 1.2 * zAdjustScale;
                }
                else if (excess > 3) {
                    deltaZ = 0.6 * zAdjustScale;
                }
                else {
                    deltaZ = 0.3 * zAdjustScale;
                }
                g_highForceCounter = 0;
                consecutiveAdjustments++;
            }
        }
        else if (filteredMag > warningForce) {
            // 接近警告力，提前调整
            g_highForceCounter++;
            g_lowForceCounter = 0;
            g_stableCounter = 0;

            if (g_highForceCounter >= highForceThreshold) {
                double excess = filteredMag - warningForce;
                // 预防性调整，更小的步进值
                if (excess > 3) {
                    deltaZ = 0.8 * zAdjustScale;
                }
                else {
                    deltaZ = 0.4 * zAdjustScale;
                }
                g_highForceCounter = 0;
                consecutiveAdjustments++;
            }
        }
        else if (filteredMag < touchForce) {
            g_lowForceCounter++;
            g_highForceCounter = 0;
            g_stableCounter = 0;

            if (g_lowForceCounter >= lowForceThreshold) {
                if (g_lastValidZ == 0.0) {
                    g_lastValidZ = currentZ;
                }

                double deficit = touchForce - filteredMag;
                if (deficit > 12) {
                    deltaZ = -1.5 * zAdjustScale;
                }
                else if (deficit > 6) {
                    deltaZ = -0.9 * zAdjustScale;
                }
                else if (deficit > 2) {
                    deltaZ = -0.5 * zAdjustScale;
                }
                else {
                    deltaZ = -0.2 * zAdjustScale;
                }
                g_lowForceCounter = 0;
                consecutiveAdjustments++;
            }
        }
        else if (filteredMag >= safeZoneMin && filteredMag <= safeZoneMax) {
            g_stableCounter++;
            g_lowForceCounter = 0;
            g_highForceCounter = 0;

            if (g_stableCounter >= stableThreshold) {
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
                if (excess > 4) {
                    deltaZ = 0.6 * zAdjustScale;
                }
                else if (excess > 1.5) {
                    deltaZ = 0.3 * zAdjustScale;
                }
                else {
                    deltaZ = 0.1 * zAdjustScale;
                }
                consecutiveAdjustments++;
            }
            else if (filteredMag < adaptiveMidForce && filteredMag > touchForce) {
                double deficit = adaptiveMidForce - filteredMag;
                if (deficit > 4) {
                    deltaZ = -0.6 * zAdjustScale;
                }
                else if (deficit > 1.5) {
                    deltaZ = -0.3 * zAdjustScale;
                }
                else {                        // 微小不足
                    deltaZ = -0.1 * zAdjustScale;
                }
                consecutiveAdjustments++;
            }
        }

        if (deltaZ != 0.0) {
            currentZ += deltaZ;
            m_vCurrentPos[2] = currentZ;
        }

        qDebug() << "V10 Delta Z: " << deltaZ << " | Scale: " << zAdjustScale
            << " | Stable: " << g_stableCounter << " | Low: " << g_lowForceCounter 
            << " | High: " << g_highForceCounter << " | Adaptive Mid: " << adaptiveMidForce;
    }

    next[2] = currentZ;

    qDebug() << "V10 Finish! Ration: " << ration;
    qDebug() << "Last valid Z: " << g_lastValidZ;
    qDebug() << "Total traveled distance: " << totalTraveledDistance;
    qDebug() << "Upright mode activated: " << isUpright;

    return true;
}