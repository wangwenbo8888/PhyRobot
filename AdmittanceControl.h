#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include <Eigen/Dense>

// 假设这是你的机械臂接口库
#include "MyWindow.h"

// 定义导纳控制参数
struct AdmittanceParams {
    Eigen::MatrixXd M; // 质量矩阵
    Eigen::MatrixXd D; // 阻尼矩阵
    Eigen::MatrixXd K; // 刚度矩阵
};

class AdmittanceController
{
public:
    AdmittanceController(const AdmittanceParams& params, double dt,MyWindow* window)
        : params_(params)
        , dt_(dt)
        , m_pWindow(window)
    {
        // 初始化状态变量
        x_.setZero(6); // 当前位置（线性+角）
        dx_.setZero(6); // 当前速度（线性+角）
        ddx_.setZero(6); // 当前加速度（线性+角）
    }

    void setPos(Eigen::VectorXd pos)
    {
        x_ = pos;
    }

    void update(const Eigen::VectorXd& force_feedback)
    {
        // 计算加速度
        ddx_ = params_.M.inverse() * (force_feedback - params_.D * dx_ - params_.K * x_);

        // 更新速度和位置
        dx_ += ddx_ * dt_;
        x_ += dx_ * dt_;
        qDebug() << "admittance new pos is x " << x_[0] << "y " << x_[1] << "z " << x_[2] << "Rx " << x_[3] << "Ry " << x_[4] << "Rz " << x_[5];

        // 发送新的位置命令给机械臂
        sendPositionCommand(x_);
    }

private:
    AdmittanceParams params_;
    double dt_;
    Eigen::VectorXd x_; // 当前位置
    Eigen::VectorXd dx_; // 当前速度
    Eigen::VectorXd ddx_; // 当前加速度

    MyWindow* m_pWindow;

    void sendPositionCommand(const Eigen::VectorXd& pos) {
        // 这里调用实际的机械臂接口发送位置命令
        m_pWindow->MovJInterface(pos[0],pos[1],pos[2],pos[3],pos[4],pos[5]);
    }
};

#if 0

int main() {
    // 设置导纳控制参数
    AdmittanceParams params;
    params.M = Eigen::MatrixXd::Identity(6, 6) * 10; // 质量矩阵，这里设置为对角矩阵并乘以10
    params.D = Eigen::MatrixXd::Identity(6, 6) * 0.5; // 阻尼矩阵，这里设置为对角矩阵并乘以0.5
    params.K.setZero(6, 6); // 刚度矩阵设置为零（纯导纳控制）

    // 控制周期
    double dt = 0.01; // 10ms

    // 创建导纳控制器实例
    AdmittanceController admittance(params, dt);

    // 模拟主循环
    while (true) {
        // 获取力反馈（这里假设从传感器获取）
        Eigen::VectorXd force_feedback = getForceFeedback();

        // 更新导纳控制器
        admittance.update(force_feedback);

        // 等待下一个控制周期
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(dt * 1000)));
    }

    return 0;
}
#endif
