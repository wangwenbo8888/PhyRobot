#include "MyWindow.h"
#include "PhysicalTherapyRobot.h"

#include <cmath>

bool MyWindow::MoveToNextAcupointNew_ImprovedV10_Simple(int group, int row, 
    DETECTED_XUEWEI currentxuewei, DETECTED_XUEWEI nextxuewei, 
    std::vector<double>& next, int ration)
{
    bool bFirst = true;
    
    double maxForce = 20.0;
    double midForce = 10.0;
    double touchForce = 5.0;
    double deadZoneLow = 3.0;
    double deadZoneHigh = 3.0;
    
    if (ration >= 3) {
        maxForce = 20.0 + (ration - 2) * 1.5;
        midForce = 10.0 + (ration - 2) * 0.8;
        touchForce = 5.0 + (ration - 2) * 0.3;
        deadZoneLow = 3.0 + (ration - 2) * 0.4;
        deadZoneHigh = 3.0 + (ration - 2) * 0.4;
    }

    double step = 3.0 * ration;
    double maxStep = 15.0;
    if (ration >= 3) {
        maxStep = 6.0 + (ration - 3) * 1.0;
    }
    if (step > maxStep) {
        step = maxStep;
    }

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
    if (divid < 1) {
        divid = 1;
    }
    
    if (ration >= 3) {
        divid = (int)(divid * (1.0 + (ration - 2) * 0.3));
        if (divid > 300) {
            divid = 300;
        }
    }

    double deltaZ = 0.0;
    double currentZ = m_vCurrentPos[2];
    double xRaw = m_vCurrentPos[0];
    double yRaw = m_vCurrentPos[1];

    double adaptiveMidForce = midForce;
    int consecutiveAdjustments = 0;

    double zAdjustScale = 1.0;
    if (ration >= 3) {
        zAdjustScale = 0.7 - (ration - 3) * 0.15;
    }

    int highForceThreshold = 2;
    int lowForceThreshold = 3;
    int stableThreshold = 3;
    
    if (ration >= 3) {
        highForceThreshold = 2 + (ration - 2);
        lowForceThreshold = 3 + (ration - 2);
        stableThreshold = 3 + (ration - 2) / 2;
    }

    for (int i = 1; i < divid; ++i) {
        double xPos = xRaw + (next[0] - xRaw) * i / divid;
        double yPos = yRaw + (next[1] - yRaw) * i / divid;
        currentZ += deltaZ;

        if (isShoulderToQiHaiRoute && !isUpright) {
            double stepDistance = sqrt((xPos - m_vCurrentPos[0]) * (xPos - m_vCurrentPos[0]) +
                (yPos - m_vCurrentPos[1]) * (yPos - m_vCurrentPos[1]));
            totalTraveledDistance += stepDistance;

            if (totalTraveledDistance >= forceUprightDistance) {
                isUpright = true;
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

            if (xAngle < -200.0) {
                xAngle = -200.0;
            }
            if (xAngle > -160.0) {
                xAngle = -160.0;
            }

            if (yAngle < -15.0) {
                yAngle = -15.0;
            }
            if (yAngle > 15.0) {
                yAngle = 15.0;
            }

            if (zAngle > 210.0) {
                zAngle = 210.0;
            }
            if (zAngle < 150.0) {
                zAngle = 150.0;
            }

            if (currentxuewei == ZHIYANG_DETECTED && bFirst) {
                xAngle = -178.0;
                yAngle = 0.0;
                bFirst = false;
            }

            if (!bFirst) {
                xAngle = -178.0;
                yAngle = 0.0;
            }

            MovL(xPos, yPos, currentZ, xAngle, yAngle, NORMAL_ANGLE);
        }
        else {
            double xAngle = -178.0;
            double yAngle = 0.0;
            double zAngle = m_vCurrentPos[5];

            if (zAngle > 210.0) {
                zAngle = 210.0;
            }
            if (zAngle < 150.0) {
                zAngle = 150.0;
            }

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
                    deltaZ = 4.0 * zAdjustScale;
                }
                else if (excess > 10) {
                    deltaZ = 2.5 * zAdjustScale;
                }
                else {
                    deltaZ = 1.5 * zAdjustScale;
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
                    deltaZ = -2.5 * zAdjustScale;
                }
                else if (deficit > 8) {
                    deltaZ = -1.5 * zAdjustScale;
                }
                else {
                    deltaZ = -0.8 * zAdjustScale;
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
                    deltaZ = 1.0 * zAdjustScale;
                }
                else {
                    deltaZ = 0.5 * zAdjustScale;
                }
                consecutiveAdjustments++;
            }
            else if (filteredMag < adaptiveMidForce && filteredMag > touchForce) {
                double deficit = adaptiveMidForce - filteredMag;
                if (deficit > 5) {
                    deltaZ = -1.0 * zAdjustScale;
                }
                else {
                    deltaZ = -0.5 * zAdjustScale;
                }
                consecutiveAdjustments++;
            }
        }

        if (deltaZ != 0.0) {
            currentZ += deltaZ;
            m_vCurrentPos[2] = currentZ;
        }
    }

    next[2] = currentZ;

    return true;
}