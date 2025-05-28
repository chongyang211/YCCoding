package com.yc.serialport.google;


import com.yc.serialport.SerialPort;
import com.yc.serialport.jni.ByteTranslateUtils;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class BaseSerialPort implements ISerialPort {

    protected SerialPort mSerialPort;
    protected OutputStream mOutputStream;
    protected InputStream mInputStream;

    /**
     * 打开串口
     *
     * @param port     串口路径，根据你的设备不同，路径也不同
     * @param baudrate 波特率
     */
    @Override
    public boolean open(String port, int baudrate) {
        boolean isOpenSuccess = false;
        try {
            if (mSerialPort == null) {
                //mSerialPort.setSuPath("/system/xbin/su");
                //打开串口
                mSerialPort = new SerialPort(new File(port), baudrate, 0);
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (mSerialPort != null) {
                isOpenSuccess = true;
                //从串口对象中获取输出流
                mOutputStream = mSerialPort.getOutputStream();
                mInputStream = mSerialPort.getInputStream();
            }
        }
        return isOpenSuccess;
    }


    /**
     * 发送数据
     * 往串口中写入数据
     *
     * @param data 数据
     */
    @Override
    public void writeByte(byte[] data) {
        try {
            //写入数据
            mOutputStream.write(data);
            mOutputStream.flush();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    /**
     * 接受数据
     * 读取串口数据
     *
     * @return 数据
     * @throws IOException
     */
    @Override
    public String readData() throws IOException {
        StringBuilder sb = new StringBuilder();
        byte[] buffer = new byte[512];
        //buffer缓冲数据
        while ((mInputStream.available()) != 0) {
            int len = mInputStream.read(buffer);
            byte[] dataByte = new byte[len];
            //数据拷贝一下
            System.arraycopy(buffer, 0, dataByte, 0, len);
            String data = ByteTranslateUtils.bytesToHex2(dataByte);
            sb.append(data);
        }
        return sb.toString();
    }

    /**
     * 关闭串口
     */
    @Override
    public void close() throws IOException {
        if (mSerialPort != null) {
            mSerialPort.close();
            mSerialPort = null;
        }
        if (mInputStream != null) {
            mInputStream.close();
        }
        if (mOutputStream != null) {
            mOutputStream.close();
        }
    }


}
