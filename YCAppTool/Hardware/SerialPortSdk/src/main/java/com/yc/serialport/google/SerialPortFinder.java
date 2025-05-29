package com.yc.serialport.google;

import android.util.Log;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.LineNumberReader;
import java.util.Iterator;
import java.util.Vector;

/**
 * 用于查找设备下所有串口路径
 */
public class SerialPortFinder {

    private static final String TAG = "SerialPort";
    private static final String DRIVERS_PATH = "/proc/tty/drivers";
    private static final String SERIAL_FIELD = "serial";
    private Vector<Driver> mDrivers;

    public SerialPortFinder() {
        File file = new File(DRIVERS_PATH);
        boolean b = file.canRead();
        Log.i(TAG, "SerialPortFinder: file.canRead() = " + b);
    }

    /**
     * 获取 Drivers
     *
     * @return Drivers
     * @throws IOException IOException
     */
    private Vector<Driver> getDrivers() throws IOException {
        if (mDrivers == null) {
            mDrivers = new Vector<>();
            LineNumberReader r = new LineNumberReader(new FileReader(DRIVERS_PATH));
            String line;
            while ((line = r.readLine()) != null) {
                // 设备名称可能存在空格
                String driverName = line.substring(0, 0x15).trim();
                String[] w = line.split(" +");
                if ((w.length >= 5) && ("serial".equals(w[w.length - 1]))) {
                    Log.d(TAG, "Found new driver " + driverName + " on " + w[w.length - 4]);
                    mDrivers.add(new Driver(driverName, w[w.length - 4]));
                }
            }
            r.close();
        }
        return mDrivers;
    }


    /**
     * 获取串口
     *
     * @return 串口
     */
    public String[] getAllDevices() {
        Vector<String> devices = new Vector<>();
        // 解析每个设备
        Iterator<Driver> itdriv;
        try {
            itdriv = getDrivers().iterator();
            while (itdriv.hasNext()) {
                Driver driver = itdriv.next();
                Iterator<File> itdev = driver.getDevices().iterator();
                while (itdev.hasNext()) {
                    String device = itdev.next().getName();
                    String value = String.format("%s (%s)", device, driver.getName());
                    devices.add(value);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return devices.toArray(new String[devices.size()]);
    }


    /**
     * 获取串口路径
     *
     * @return 串口路径
     */
    public String[] getAllDevicesPath() {
        Vector<String> devices = new Vector<>();
        Iterator<Driver> itdriv;
        try {
            itdriv = getDrivers().iterator();
            while (itdriv.hasNext()) {
                Driver driver = itdriv.next();
                Iterator<File> itdev = driver.getDevices().iterator();
                while (itdev.hasNext()) {
                    String device = itdev.next().getAbsolutePath();
                    devices.add(device);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return devices.toArray(new String[devices.size()]);
    }

    public static class Driver {

        public Driver(String name, String root) {
            mDriverName = name;
            mDeviceRoot = root;
        }

        private final String mDriverName;
        private final String mDeviceRoot;

        Vector<File> mDevices = null;

        public Vector<File> getDevices() {
            if (mDevices == null) {
                mDevices = new Vector<>();
                File dev = new File("/dev");
                File[] files = dev.listFiles();
                int i;
                for (i = 0; i < files.length; i++) {
                    if (files[i].getAbsolutePath().startsWith(mDeviceRoot)) {
                        mDevices.add(files[i]);
                    }
                }
            }
            return mDevices;
        }

        public String getName() {
            return mDriverName;
        }

    }
}
