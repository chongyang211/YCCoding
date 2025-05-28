package com.yc.serialport.jni;

import android.text.TextUtils;

import com.yc.serialport.SerialPortJNI;
import com.yc.serialport.google.ISerialPort;

import java.io.File;
import java.io.IOException;

public class BaseJniSerialPort implements ISerialPort {

    /**
     * 有些设备su路径是/system/xbin/su
     */
    private static String mSuPath = "/system/bin/su";
    /**
     * 用于设置 su 路径 默认的路径 /system/bin/su，(一般不会使用)
     *
     * @param suPath 设置su路径
     */
    public static void setSuPath(String suPath) {
        if (TextUtils.isEmpty(suPath)) {
            return;
        }
        mSuPath = suPath;
    }

    /**
     * 打开串口
     *
     * @param port     串口路径，根据你的设备不同，路径也不同
     * @param baudrate 波特率
     */
    @Override
    public boolean open(String port, int baudrate) {
        File device = new File(port);
        /* 检查访问权限 */
        if (!device.canRead() || !device.canWrite()) {
            try {
                /* 没有读/写权限，尝试对文件进行提权 */
                Process su = Runtime.getRuntime().exec(mSuPath);
                // 修改文件属性为 [可读 可写 可执行]
                String cmd = "chmod 666 " + device.getAbsolutePath() + "\n" + "exit\n";
                su.getOutputStream().write(cmd.getBytes());
                if ((su.waitFor() != 0) || !device.canRead() || !device.canWrite()) {
                    throw new SecurityException();
                }
            } catch (Exception e) {
                e.printStackTrace();
                throw new SecurityException();
            }
        }
        //device 串口设备文件
        //baudrate 波特率
        //数据位；默认8,可选值为5~8
        //parity 奇偶校验；0:无校验位(NONE，默认)；1:奇校验位(ODD);2:偶校验位(EVEN)
        //stopBits 停止位；默认1；1:1位停止位；2:2位停止位
        //flags 默认0
        int openPort = SerialPortJNI.openPort(device.getAbsolutePath(), baudrate, 8, 1, 'N');
        SerialPortJNI.setMode(0);
        return openPort == 1;
    }

    @Override
    public void writeByte(byte[] data) {
        SerialPortJNI.writePort(data);
    }

    @Override
    public String readData() throws IOException {
        byte[] dataByte = SerialPortJNI.readPort(1024 * 2);
        return ByteTranslateUtils.bytesToHex2(dataByte);
    }

    @Override
    public void close() throws IOException {
        SerialPortJNI.closePort();
    }
}
