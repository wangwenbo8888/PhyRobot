#include <stdio.h>
#include "../include/XJC_SDK_Lib.h"

int main() {
    // c++ style code
    // serialport
    /*
    XJC_SDK_Lib::SerialPort serialPort;
    int result = serialPort.creatPort(L"/dev/ttyACM0", 115200, "D80");
    std::cerr << "creat port result code = " << result << std::endl;
    if (serialPort.openPort()) {
        int result = serialPort.sendAndReceiveCommand();
        if (result == 1)
        {
            std::cerr << "Success send and receive command:" << std::endl;
            float fx, fy, fz, mx, my, mz;
            if (serialPort.getForceAndTorque(fx, fy, fz, mx, my, mz)) {
                std::cerr << "Force and Torque values:" << std::endl;
                std::cerr << "Fx: " << fx << ", Fy: " << fy << ", Fz: " << fz << std::endl;
                std::cerr << "Mx: " << mx << ", My: " << my << ", Mz: " << mz << std::endl;
            }
            else
            {
                std::cerr << "Failed to retrieve valid force and torque data." << std::endl;
            }
            if (serialPort.sendFilterOpenCommand())
            {
                std::cerr << "Success to open filter." << std::endl;
            }
            else
            {
                std::cerr << "Failed to open filter." << std::endl;
            }
            serialPort.closePort();
        }
        else
        {
            std::cerr << "Failed send and receive command , error code : " << result << std::endl;
        }
    }
    else {
        std::cerr << "Failed to open port." << std::endl;
    }
    return 0;
    */

    // c style code
    // serialport
    /*
    // 创建串口对象
    XJC_SDK_Lib::SerialPort* sp = XJC_SDK_Lib::CreateSerialPort();
    if (sp == NULL) {
        printf("无法创建SerialPort实例\n");
        //FreeLibrary(hDll);
        return -1;
    }
    // 初始化串口
    const wchar_t* portName = L"/dev/ttyACM0";
    int baudRate = 115200;
    const char* sensorName = "D80";
    int result = XJC_SDK_Lib::CreatePort(sp, portName, baudRate, sensorName);
    if (result != 1) {
        printf("CreatePort函数调用失败\n");
        XJC_SDK_Lib::DestroySerialPort(sp);
        //FreeLibrary(hDll);
        return -1;
    }

    // 打开串口
    if (XJC_SDK_Lib::OpenPort(sp)) {
        printf("successed open serial port\n");
    }
    else {
        printf("failed open serial port\n");
    }

    result = XJC_SDK_Lib::SendAndReceiveCommand(sp);
    if (result == 1)
    {
        printf("Success send and receive command\n");
        float fx, fy, fz, mx, my, mz;
        if (XJC_SDK_Lib::GetForceAndTorque(sp, &fx, &fy, &fz, &mx, &my, &mz))
        {
            printf("Force and Torque values:\n");
            printf("Fx: %f, Fy: %f, Fz: %f\n", fx, fy, fz);
            printf("Mx: %f, My: %f, Mz: %f\n", mx, my, mz);
        }
        else
        {
            printf("Failed to retrieve valid force and torque data.\n");
        }
    }

    if (XJC_SDK_Lib::SendFilterOpenCommand(sp))
    {
        printf("Successed to open filter\n");
    }
    else
    {
        printf("Filed to open filter\n");
    }

    if (XJC_SDK_Lib::SendFilterCloseCommand(sp))
    {
        printf("Successed to close filter\n");
    }
    else
    {
        printf("Filed to close filter\n");
    }

    // 销毁串口对象
    XJC_SDK_Lib::DestroySerialPort(sp);

    return 0;
    */

    // c++ style code
    // EthernetTCP

    // 创建一个 EthernetTCPDLL 类的对象
    // creat a obj for EthernetTCPDLL-Class 
    XJC_SDK_Lib::EthernetTCP ethernettcpdllObj;
    // 准备参数
    // preparation parameter
    const char* ipAddress = "192.168.1.28"; // your force sensor ip
    const int port = 502;
    float fx, fy, fz, mx, my, mz; // save 6 force values

    // 调用 GetForceSensorData 函数并获取结果
    // use GetForceSensorData and get force values
    int result = ethernettcpdllObj.GetForceSensorData(ipAddress, port, &fx, &fy, &fz,
        &mx, &my, &mz);
    // 检查返回值
    // check result
    if (result == 1) {
        std::cerr << "Successfully received force sensor data:" << std::endl;
        std::cerr << "fx = " << fx << std::endl;
        std::cerr << "fy = " << fy << std::endl;
        std::cerr << "fz = " << fz << std::endl;
        std::cerr << "mx = " << mx << std::endl;
        std::cerr << "my = " << my << std::endl;
        std::cerr << "mz = " << mz << std::endl;
    }
    else {
        std::cerr << "Failed to get force sensor data. Error code: " << result << std::endl;
    }

    // if you need colse filter ---> use 'sendFilterCloseCommand'
    int res = ethernettcpdllObj.sendFilterOpenCommand(ipAddress, port);
    if (res == 1) {
        std::cerr << "Successed to open filter" << std::endl;
    }
    else {
        std::cerr << "Failed to open filter , error code : " << res << std::endl;
    }

    res = ethernettcpdllObj.sendFilterCloseCommand(ipAddress, port);
    if (res == 1) {
        std::cerr << "Successed to close filter" << std::endl;
    }
    else {
        std::cerr << "Failed to close filter , error code : " << res << std::endl;
    }

    return 0;





}