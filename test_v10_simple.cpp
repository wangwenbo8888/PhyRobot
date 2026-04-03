#include <vector>
#include <cmath>
#include <iostream>

// 简化的V10函数测试
class MyWindow {
public:
    bool MoveToNextAcupointNew_ImprovedV10(int group, int row, int currentxuewei, int nextxuewei, 
                                          std::vector<double>& next, int ration) {
        
        bool bFirst = true;
        
        // === 力阈值自适应优化 ===
        double maxForce = 20.0;
        double midForce = 10.0;
        double touchForce = 5.0;
        double deadZoneLow = 3.0;
        double deadZoneHigh = 3.0;
        
        // === 适应不同ration值的参数调整 ===
        if (ration >= 3) {
            maxForce = 20.0 + (ration - 2) * 1.5;
            midForce = 10.0 + (ration - 2) * 0.8;
            touchForce = 5.0 + (ration - 2) * 0.3;
            deadZoneLow = 3.0 + (ration - 2) * 0.4;
            deadZoneHigh = 3.0 + (ration - 2) * 0.4;
        }

        // === 步长优化 - 限制最大步长 ===
        double step = 3.0 * ration;
        double maxStep = 15.0;
        if (ration >= 3) {
            maxStep = 6.0 + (ration - 3) * 1.0;
        }
        if (step > maxStep) step = maxStep;

        // === 计数器阈值自适应优化 ===
        int highForceThreshold = 2;
        int lowForceThreshold = 3;
        int stableThreshold = 3;
        
        if (ration >= 3) {
            highForceThreshold = 2 + (ration - 2);
            lowForceThreshold = 3 + (ration - 2);
            stableThreshold = 3 + (ration - 2) / 2;
        }

        std::cout << "V10 compilation test passed - ration: " << ration << std::endl;
        std::cout << "maxForce: " << maxForce << ", step: " << step << std::endl;
        std::cout << "highForceThreshold: " << highForceThreshold << std::endl;
        
        return true;
    }
};

int main() {
    MyWindow window;
    std::vector<double> nextPos = {100, 200, 300};
    
    // 测试不同的ration值
    for (int ration = 1; ration <= 5; ration++) {
        bool result = window.MoveToNextAcupointNew_ImprovedV10(0, 0, 0, 0, nextPos, ration);
        std::cout << "Ration " << ration << ": " << (result ? "PASS" : "FAIL") << std::endl;
    }
    
    return 0;
}