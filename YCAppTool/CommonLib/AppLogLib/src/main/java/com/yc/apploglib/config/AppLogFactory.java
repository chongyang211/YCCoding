package com.yc.apploglib.config;

import android.content.Context;
import android.text.TextUtils;
import android.util.Log;

import androidx.annotation.NonNull;

import com.yc.apploglib.AppLogHelper;
import com.yc.apploglib.log.DefaultLoggerImpl;
import com.yc.apploglib.log.InterLogger;
import com.yc.apploglib.printer.AbsPrinter;
import com.yc.apploglib.printer.FilePrinterImpl;
import com.yc.apploglib.printer.LogcatPrinterImpl;

import java.io.File;

/**
 * <pre>
 *     @author yangchong
 *     GitHub : https://github.com/yangchong211/YCCommonLib
 *     email : yangchong211@163.com
 *     time  : 2018/11/9
 *     desc  : log日志工厂累
 *     revise:
 * </pre>
 */
public final class AppLogFactory {

    public final static LogDispatcher S_LOG_DISPATCHER = new LogDispatcher();
    private static AppLogConfig appLogConfig;

    public static void init(@NonNull AppLogConfig config) {
        appLogConfig = config;
        if (config.isEnableDbgLog()) {
            S_LOG_DISPATCHER.addPrinter(new LogcatPrinterImpl());
        }
        if (config.isWriteFile()){
            File file;
            if (TextUtils.isEmpty(config.getFilePath())){
                Context context = config.getContext();
                String log = getExternalFilePath(context, "logger");
                file = new File(log);
            } else {
                file = new File(config.getFilePath());
            }
            Log.d(AppLogHelper.TAG , "file path" + file.getPath());
            S_LOG_DISPATCHER.addPrinter(new FilePrinterImpl(file));
        }
        S_LOG_DISPATCHER.setMinLogLevel(config.getMinLogLevel());
    }

    public static InterLogger getLogger(String tag) {
        return new DefaultLoggerImpl(tag, S_LOG_DISPATCHER);
    }

    public static boolean addPrinter(@NonNull AbsPrinter printer) {
        return S_LOG_DISPATCHER.addPrinter(printer);
    }

    public static void removePrinter(@NonNull AbsPrinter printer) {
        S_LOG_DISPATCHER.removePrinter(printer);
    }

    public static boolean hasPrinter(@NonNull String printerName) {
        return S_LOG_DISPATCHER.hasPrinter(printerName);
    }

    public static void enableLogcatPrinter() {
        enableLogcatPrinter(Log.WARN);
    }

    public static AppLogConfig getAppLogConfig(){
        if (appLogConfig == null){
            throw new NullPointerException("AppLogFactory please init log at first");
        }
        return appLogConfig;
    }

    public static void enableLogcatPrinter(int logLevel) {
        if (!S_LOG_DISPATCHER.hasPrinter(LogcatPrinterImpl.PRINTER_NAME)) {
            S_LOG_DISPATCHER.addPrinter(new LogcatPrinterImpl());
            S_LOG_DISPATCHER.setMinLogLevel(logLevel);
        }
    }


    /**
     * 外部存储根目录，举个例子
     * files:/storage/emulated/0/Android/data/包名/files
     */
    public static String getExternalFilePath(Context context , String name) {
        String path = getExternalFilePath(context) + File.separator + name;
        File file = new File(path);
        if (!file.exists()) {
            //创建一个File对象所对应的目录，成功返回true，否则false。且File对象必须为路径而不是文件。
            //创建多级目录，创建路径中所有不存在的目录
            file.mkdirs();
        }
        return path;
    }

    /**
     * 获取外部存储根目录的files文件路径
     * files:/storage/emulated/0/Android/data/包名/files
     * @param context   上下文
     * @return          路径
     */
    public static String getExternalFilePath(Context context){
        File filesDir = context.getExternalFilesDir(null);
        if (filesDir!=null && filesDir.exists()){
            return filesDir.getAbsolutePath();
        }
        return null;
    }
}
