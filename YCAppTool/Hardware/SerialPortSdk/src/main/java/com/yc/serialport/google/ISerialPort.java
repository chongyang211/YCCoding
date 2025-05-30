package com.yc.serialport.google;

import java.io.IOException;

public interface ISerialPort {
    /**
     * 打开串口
     *
     * @param port 串口路径，根据你的设备不同，路径也不同
     * @param flag 波特率
     */
    boolean open(String port, int flag);

    /**
     * 发送数据
     * 往串口中写入数据
     *
     * @param data 数据
     */
    void writeByte(byte[] data);

    /**
     * 接受数据
     * 读取串口数据
     *
     * @return 数据
     * @throws IOException
     */
    String readData() throws IOException;

    /**
     * 关闭串口
     */
    void close() throws IOException;
}
