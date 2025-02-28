// 可以使用的传感器型号 R106F D80 D82

#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <string>
#include <cstdint>
#include <Windows.h>
#include <iostream>
#include <cstring>  // For memcpy
#include <iomanip>
#include <algorithm>
#include <unordered_map>

#pragma comment(lib, "Ws2_32.lib")

#ifdef XJC_SDK_LIB_EXPORTS
#define XJC_SDK_LIB_API __declspec(dllexport)
#else
#define XJC_SDK_LIB_API __declspec(dllimport)
#endif

namespace XJC_SDK_Lib {
    class XJC_SDK_LIB_API SerialPort {
    public:
        //SerialPort(const std::wstring& portName, int baudRate);
        SerialPort();
        ~SerialPort();

        int creatPort(const std::wstring& portName, int baudRate, const std::string& newsensorName);
        bool openPort();
        bool closePort();    
               
        int sendAndReceiveCommand();
        bool send1000HzCommand();
        bool sendFilterOpenCommand();
        bool sendFilterCloseCommand();
        bool sendStopCommand();        

        // 新增方法：获取传感器的力和力矩值
        bool getForceAndTorque(float& fx, float& fy, float& fz, float& mx, float& my, float& mz);

    private:
        std::wstring portName;
        int baudRate;
        HANDLE hSerial;  // 串口句柄
        std::string sensorName;

        std::vector<uint8_t> receiveData(size_t expectedBytes);

        std::vector<uint8_t> extractDataAfterHeader(const std::vector<uint8_t>& data, const std::vector<uint8_t>& frameHeader, size_t numBytesToRead);

        bool sendQACommand();

        bool sendHexData(const std::vector<uint8_t>& data);

        void receiveAndPrintData();

        // 判断系统字节序的方法
        bool isSystemLittleEndian() const;

        // 反转字节顺序的方法
        void reverseBytes(uint8_t* data, size_t size) const;

        // 存储传感器的力和力矩值
        float forceValues[6];  // fx, fy, fz, mx, my, mz
    };


    // 下面是C风格的导出函数，C#可以调用
    extern "C" {
        __declspec(dllexport) SerialPort* __stdcall CreateSerialPort() {
            return new SerialPort();
        }

        __declspec(dllexport) void __stdcall DestroySerialPort(SerialPort* instance) {
            delete instance;
        }

        __declspec(dllexport) int __stdcall CreatePort(SerialPort* instance, const wchar_t* portName, int baudRate, const char* newsensorName) {
            return instance->creatPort(portName, baudRate, newsensorName);
        }

        __declspec(dllexport) bool __stdcall OpenPort(SerialPort* instance) {
            return instance->openPort();
        }

        __declspec(dllexport) bool __stdcall ClosePort(SerialPort* instance) {
            return instance->closePort();
        }

        __declspec(dllexport) int __stdcall SendAndReceiveCommand(SerialPort* instance) {
            return instance->sendAndReceiveCommand();
        }

        __declspec(dllexport) bool __stdcall GetForceAndTorque(
            SerialPort* instance, float* fx, float* fy, float* fz, float* mx, float* my, float* mz) {
            return instance->getForceAndTorque(*fx, *fy, *fz, *mx, *my, *mz);
        }

        __declspec(dllexport) bool __stdcall SendFilterOpenCommand(SerialPort* instance) {
            return instance->sendFilterOpenCommand();
        }

        __declspec(dllexport) bool __stdcall SendFilterCloseCommand(SerialPort* instance) {
            return instance->sendFilterCloseCommand();
        }
    }


    class XJC_SDK_LIB_API EthernetTCP
    {
    public:
        EthernetTCP();
        ~EthernetTCP();

        int GetForceSensorData(const char* ipAddress, const int port,
            float* fx, float* fy, float* fz,
            float* mx, float* my, float* mz);

        int sendFilterOpenCommand(const char* ipAddress, const int port);

        int sendFilterCloseCommand(const char* ipAddress, const int port);

        int sendClearZeroCommand(const char* ipAddress, const int port);

    private:
        // 初始化 Winsock
        bool initializeWinsock();

        // 创建并配置套接字
        SOCKET createAndConfigureSocket();

        // 连接到服务器
        bool connectToServer(SOCKET& ConnectSocket, const char* ipAddress, int port);

        // 发送数据
        bool sendData(SOCKET& ConnectSocket, const unsigned char* data, int length);

        // 接收数据
        int receiveData(SOCKET& ConnectSocket, char* buffer, int length);

        // 将接收到的字节数据转换为 double 数组的函数
        std::vector<double> convertToDouble(const char* buffer, int length);

        // 清理套接字和 Winsock
        void cleanup(SOCKET& ConnectSocket);

        // 解析从第10个字节开始每4个字节为浮点数
        void parseForceValues(const std::vector<uint8_t>& data,
            float* fx, float* fy, float* fz,
            float* mx, float* my, float* mz);

        bool isLittleEndian();

        float bytesToFloat(const uint8_t* bytes);

        void printBytes(const std::vector<uint8_t>& data);

    }; 


    // 下面是C风格的导出函数，C#可以调用
    extern "C" {
        __declspec(dllexport) EthernetTCP* __stdcall CreatEthernetTCP() {
            return new EthernetTCP();
        }

        __declspec(dllexport) void __stdcall DestroyEthernetTCP(EthernetTCP* instance) {
            delete instance;
        }

        __declspec(dllexport) int __stdcall GetForceSensorData(EthernetTCP* instance, const char* ipAddress, const int port, float* fx, float* fy, float* fz, float* mx, float* my, float* mz) {
            return instance->GetForceSensorData(ipAddress, port, fx, fy, fz, mx, my, mz);
        }

        /*__declspec(dllexport) void __stdcall ParseForceValues(EthernetTCP* instance, const std::vector<uint8_t>& data, float* fx, float* fy, float* fz, float* mx, float* my, float* mz) {
            return instance->parseForceValues(data, fx, fy, fz, mx, my, mz);
        }*/

        __declspec(dllexport) int __stdcall TCPSendFilterOpenCommand(EthernetTCP* instance, const char* ipAddress, const int port) {
            return instance->sendFilterOpenCommand(ipAddress, port);
        }

        __declspec(dllexport) int  __stdcall TCPSendFilterCloseCommand(EthernetTCP* instance, const char* ipAddress, const int port) {
            return instance->sendFilterCloseCommand(ipAddress, port);
        }
        
        __declspec(dllexport) int  __stdcall TCPClearZeroCommand(EthernetTCP* instance, const char* ipAddress, const int port) {
            return instance->sendClearZeroCommand(ipAddress, port);
        }

    }

}
