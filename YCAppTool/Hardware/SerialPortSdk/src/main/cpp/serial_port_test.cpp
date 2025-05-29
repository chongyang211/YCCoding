//
// Created by 杨充 on 2024/4/10.
//

#include "serial_port.h"
#include "iostream"
#include "serial_port_c.h"

int main() {
//    // 创建串口对象
//    SerialPort serial("/dev/ttyUSB0");
//
//    // 打开串口
//    if (serial.open() != SerialPort::SUCCESS) {
//        std::cout << "Failed to open serial port." << std::endl;
//        return 1;
//    }
//
//    // 设置串口参数
//    serial.setBaudRate(SerialPort::BaudRate::BAUD_9600);
//    serial.setParity(SerialPort::Parity::PARITY_NONE);
//    serial.setDataBits(SerialPort::DataBits::DATA_8);
//    serial.setStopBits(SerialPort::StopBits::STOP_1);
//
//    // 发送数据
//    std::string message = "Hello, SerialPort!";
//    serial.write(message);
//
//    // 接收数据
//    std::string receivedData = serial.read();
//
//    // 打印接收到的数据
//    std::cout << "Received data: " << receivedData << std::endl;
//
//    // 关闭串口
//    serial.close();

    return 0;
}

