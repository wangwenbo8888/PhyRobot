// 可以使用的传感器型号 R106F D80 D82
#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>  // For memcpy
#include <iomanip>
#include <algorithm>
#include <array>
#include <stdexcept>
#include <cerrno>
#include <fstream>
#include <chrono>
#include <thread>
#include <codecvt> // 用于宽字符转换
#include <vector>
#include <locale>
#include <unordered_map>
#include <sstream>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <utility>
#include <climits>
#include <sys/socket.h> // Linux socket
#include <arpa/inet.h>  // inet_pton, htons, etc.
#include <filesystem>

#ifdef XJC_SDK_Lib_EXPORTS
#define XJC_SDK_Lib_API __attribute__((visibility("default")))
#else
#define XJC_SDK_Lib_API
#endif

namespace XJC_SDK_Lib {
    class XJC_SDK_Lib_API SerialPort {
    public:
        // SerialPort(const std::string& portName, int baudRate);
        SerialPort();
        ~SerialPort();

        int creatPort(const std::wstring& portName, int baudRate, const std::string& newsensorName);
        bool openPort();
        void closePort();        
        
        int sendAndReceiveCommand();
        bool send1000HzCommand();
        bool sendFilterOpenCommand();
        bool sendFilterCloseCommand();
        bool sendStopCommand();
        void receiveAndPrintData();

        bool getForceAndTorque(float& fx, float& fy, float& fz, float& mx, float& my, float& mz);

        void collectDataToCSV(const std::string& fileName, int durationmilliSeconds);        

    private:
        bool sendHexData(const std::vector<uint8_t>& data);
        std::vector<uint8_t> receiveData(size_t expectedBytes);
        std::vector<uint8_t> extractDataAfterHeader(const std::vector<uint8_t>& data, const std::vector<uint8_t>& frameHeader, size_t numBytesToRead);
        bool sendSingleCommand();
        std::string wstringToString(const std::wstring& wstr);

        std::wstring portName;
        int baudRate;   // 波特率
        int fd;  // 串口文件描述符

        bool isSystemLittleEndian() const;
        void reverseBytes(uint8_t* data, size_t size) const;

        float forceValues[6];  // fx, fy, fz, mx, my, mz

        std::string sensorName;              // 传感器名称

    };

    // 下面是C风格的导出函数，C#可以调用
    extern "C" {
        __attribute__((visibility("default"))) SerialPort* CreateSerialPort() {
            return new SerialPort();
        }

        __attribute__((visibility("default"))) void DestroySerialPort(SerialPort* instance) {
            delete instance;
        }

        __attribute__((visibility("default"))) int CreatePort(SerialPort* instance, const wchar_t* portName, int baudRate, const char* newsensorName) {
            return instance->creatPort(portName, baudRate, newsensorName);
        }

        __attribute__((visibility("default"))) bool OpenPort(SerialPort* instance) {
            return instance->openPort();
        }

        __attribute__((visibility("default"))) void ClosePort(SerialPort* instance) {
            return instance->closePort();
        }

        __attribute__((visibility("default"))) int SendAndReceiveCommand(SerialPort* instance) {
            return instance->sendAndReceiveCommand();
        }

        __attribute__((visibility("default"))) bool GetForceAndTorque(
            SerialPort* instance, float* fx, float* fy, float* fz, float* mx, float* my, float* mz) {
            return instance->getForceAndTorque(*fx, *fy, *fz, *mx, *my, *mz);
        }

        __attribute__((visibility("default"))) bool SendFilterOpenCommand(SerialPort* instance) {
            return instance->sendFilterOpenCommand();
        }

        __attribute__((visibility("default"))) bool SendFilterCloseCommand(SerialPort* instance) {
            return instance->sendFilterCloseCommand();
        }
    }

    class XJC_SDK_Lib_API EthernetTCP {
        public:
        EthernetTCP();
        ~EthernetTCP();

        int GetForceSensorData(const char* ipAddress, const int port,
                                float* fx, float* fy, float* fz,
                                float* mx, float* my, float* mz);

        int sendFilterOpenCommand(const char* ipAddress, const int port);

        int sendFilterCloseCommand(const char* ipAddress, const int port);

        private:
        // 创建并配置套接字
        int createAndConfigureSocket();

        // 连接到服务器
        bool connectToServer(int& ConnectSocket, const char* ipAddress, int port);

        // 发送数据
        bool sendData(int& ConnectSocket, const unsigned char* data, int length);

        // 接收数据
        int receiveData(int& ConnectSocket, char* buffer, int length);

        // 将接收到的字节数据转换为 double 数组的函数
        std::vector<double> convertToDouble(const char* buffer, int length);

        // 清理套接字
        void cleanup(int& ConnectSocket);

        // 解析从第10个字节开始每4个字节为浮点数
        // void parseForceValues(const std::vector<double>& data, float* forceValues);
        void parseForceValues(const std::vector<uint8_t>& data,
                                              float* fx, float* fy, float* fz,
                                              float* mx, float* my, float* mz);

        bool isLittleEndian();

        float bytesToFloat(const uint8_t* bytes);

        void printBytes(const std::vector<uint8_t>& data);

    };

    // 下面是C风格的导出函数，C#可以调用
    extern "C" {
        __attribute__((visibility("default"))) EthernetTCP* CreatEthernetTCP() {
            return new EthernetTCP();
        }

        __attribute__((visibility("default"))) void DestroyEthernetTCP(EthernetTCP* instance) {
            delete instance;
        }

        __attribute__((visibility("default"))) int GetForceSensorData(EthernetTCP* instance, const char* ipAddress, const int port, float* fx, float* fy, float* fz, float* mx, float* my, float* mz) {
            return instance->GetForceSensorData(ipAddress, port, fx, fy, fz, mx, my, mz);
        }

        /*__declspec(dllexport) void __stdcall ParseForceValues(EthernetTCP* instance, const std::vector<uint8_t>& data, float* fx, float* fy, float* fz, float* mx, float* my, float* mz) {
            return instance->parseForceValues(data, fx, fy, fz, mx, my, mz);
        }*/

        __attribute__((visibility("default"))) int TCPSendFilterOpenCommand(EthernetTCP* instance, const char* ipAddress, const int port) {
            return instance->sendFilterOpenCommand(ipAddress, port);
        }

        __attribute__((visibility("default"))) int TCPSendFilterCloseCommand(EthernetTCP* instance, const char* ipAddress, const int port) {
            return instance->sendFilterCloseCommand(ipAddress, port);
        }
    }


}
