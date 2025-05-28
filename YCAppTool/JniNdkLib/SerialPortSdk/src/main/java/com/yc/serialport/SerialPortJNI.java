package com.yc.serialport;

/**
 * c++版本的串口通信方案。
 */
public class SerialPortJNI {

    static {
        System.loadLibrary("SerialPort");
    }

    /**
     * 打开串口并设置串口数据位，校验位, 速率，停止位
     *
     * @param path     串口地址
     * @param baudRate 波特率
     * @param dataBits 数据位
     * @param stopBits 停止位
     * @param parity   校验类型 取值N ,E, O
     * @return 返回1表示成功
     */
    public static native int openPort(String path, int baudRate, int dataBits,
                                      int stopBits, char parity);

    /**
     * 设置是否使用原始模式(Raw Mode)方式来通讯 取值0,1,2
     *
     * @param mode 0=nothing
     *             1=Raw mode
     *             2=no raw mode
     * @return
     */
    public static native int setMode(int mode);

    /**
     * 读取串口数据
     *
     * @param maxSize 数据最大长度
     * @return 串口数据
     */
    public static native byte[] readPort(int maxSize);

    /**
     * 写入串口数据
     *
     * @param data 串口数据指令
     */
    public static native void writePort(byte[] data);

    /**
     * 关闭串口
     */
    public static native void closePort();

}
