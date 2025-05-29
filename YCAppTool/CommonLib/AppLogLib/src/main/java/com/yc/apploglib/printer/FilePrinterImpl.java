package com.yc.apploglib.printer;

import static com.yc.apploglib.AppLogHelper.TAG;

import android.content.Context;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Process;
import android.util.Log;

import com.yc.apploglib.config.AppLogFactory;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Locale;

/**
 * <pre>
 *     @author yangchong
 *     GitHub : https://github.com/yangchong211/YCCommonLib
 *     email : yangchong211@163.com
 *     time  : 2018/11/9
 *     desc  : Printer写入文件
 *     revise:
 * </pre>
 */
public class FilePrinterImpl extends AbsPrinter {
    /**
     * 10MB
     */
    private static final long MAX_LOG_FILE = 1024 * 1024 * 10;
    /**
     * 名字
     */
    private static final String PRINTER_NAME = "FilePrinter";
    /**
     * file文件，父目录
     */
    private final File mLogDir;
    /**
     * header格式
     */
    private SimpleDateFormat sInfoHeaderFormat = null;
    /**
     * file文件格式
     */
    private SimpleDateFormat simpleDateFormat = null;
    /**
     * 时间
     */
    private long tomorrow;
    /**
     * 工作日志
     */
    private File workLogFile;
    private static final long DAY = 24 * 3600 * 1000;

    /**
     * File Thread handler.
     */
    private final DelayInitializer<Handler> mHandler = new DelayInitializer<>(() -> {
        HandlerThread thread = new HandlerThread("CommonFileLogThread");
        thread.start();
        return new Handler(thread.getLooper());
    });

    private PrintWriter mWriter;

    public FilePrinterImpl(File logDir) {
        mLogDir = logDir;
        //初始化文件
        tomorrow = System.currentTimeMillis() / DAY + 1;
        //初始化文件夹
        SimpleDateFormat simpleDateFormat = getSimpleDateFormat();
        workLogFile = new File(mLogDir, simpleDateFormat.format(System.currentTimeMillis()) + ".log");
        try {
            workLogFile.createNewFile();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private SimpleDateFormat getInfoHeaderFormat() {
        if (sInfoHeaderFormat == null) {
            sInfoHeaderFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS", Locale.getDefault());
        }
        return sInfoHeaderFormat;
    }

    private SimpleDateFormat getSimpleDateFormat() {
        if (simpleDateFormat == null) {
            simpleDateFormat = new SimpleDateFormat("yyyy-MM-dd", Locale.getDefault());
        }
        return simpleDateFormat;
    }

    private PrintWriter getWriter() {
        if (mWriter == null) {
            do {
                File logFile = getLogFile();
                if (logFile == null) {
                    break;
                }
                Log.d("AppLog", "logFile path" + logFile.getPath());
                if (logFile.length() > MAX_LOG_FILE) {
                    if (!logFile.delete()) {
                        break;
                    }
                }
                try {
                    FileOutputStream fileOutputStream = new FileOutputStream(logFile, true);
                    OutputStreamWriter writer = new OutputStreamWriter(fileOutputStream, "UTF-8");
                    //Writer writer = new StringWriter();
                    mWriter = new PrintWriter(writer);
                } catch (Throwable ignored) {
                }
            } while (false);
        }
        return mWriter;
    }

    @Override
    public String name() {
        return PRINTER_NAME;
    }

    @Override
    public void println(final int level, final String tag, final String format, final Throwable tr) {
        final int threadId = Process.myTid();
        postRunnable(() -> logToFileInner(level, threadId, tag, format, tr));
    }

    protected void postRunnable(Runnable runnable) {
        Handler handler = mHandler.get();
        if (handler != null) {
            handler.post(runnable);
        }
    }

    private void logToFileInner(int level, int threadId, String tag, String msg, Throwable tr) {
        try {
            PrintWriter writer = getWriter();
            if (writer == null) {
                return;
            }
            StringBuilder sb = new StringBuilder();
            sb.append("时间：").append(getInfoHeaderFormat().format(new Date())).append("-");
            sb.append("线程id：").append(threadId).append("-");
            sb.append("Level：").append(levelToStr(level)).append("-");
            sb.append("tag：").append(tag).append("-");
            sb.append("打印消息：").append(msg).append("；");

            String log = sb.toString();
            //Log.d("logWrite: ", log);
            writer.println(log);
            if (tr != null) {
                tr.printStackTrace(writer);
                writer.println();
            }
            writer.flush();
        } catch (Throwable ignored) {
            // 6.0 在没有申请权限之前是无法访问外部存储的。
            //ignored.printStackTrace();
        }
    }

    private static String levelToStr(int level) {
        switch (level) {
            case Log.VERBOSE:
                return "V";
            case Log.DEBUG:
                return "D";
            case Log.INFO:
                return "I";
            case Log.WARN:
                return "W";
            case Log.ERROR:
                return "E";
            case Log.ASSERT:
                return "A";
            default:
                return "UNKNOWN";
        }
    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();

        internalClose();
    }

    void internalClose() {
        if (mWriter != null) {
            mWriter.close();
            mWriter = null;
        }
    }

    File getLogFile() {
        long today = System.currentTimeMillis() / DAY;
        if (today >= tomorrow || mLogDir == null) {
            tomorrow = today + 1;
            SimpleDateFormat simpleDateFormat = getSimpleDateFormat();
            workLogFile = new File(mLogDir, simpleDateFormat.format(System.currentTimeMillis()) + ".log");
            try {
                workLogFile.createNewFile();
                Log.d("LogFile", "workLogFile " + workLogFile.getPath());
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        Log.d(TAG, "workLogFile " + workLogFile.getPath());
        return workLogFile;
    }

    @Override
    public void clearOldFile() {
        File logPalmFile = getAppLogger();
        if (logPalmFile == null) {
            return;
        }
        long clearLogFileTime = AppLogFactory.getAppLogConfig().getClearLogFileTime();
        if (clearLogFileTime > 7) {
            //如果过期清理日志的时间（天数）大于7，则使用默认天数7，一周的日志
            clearLogFileTime = 7;
        }
        List<File> fileList = getFileList(logPalmFile);
        for (int i = 0; i < fileList.size(); i++) {
            File file = fileList.get(i);
            long fileTime = getFileTime(file);
            long time = System.currentTimeMillis() - fileTime;
            if (time > clearLogFileTime * 24 * 60 * 1000) {
                //删除超过一周以前的数据
                boolean deleteFile = deleteFile(file);
                Log.d(TAG, "删除log文件状态：" + file.getName() + " , 状态" + deleteFile);
            }

            if (file.getName().endsWith("zip")) {
                boolean deleteFile = deleteFile(file);
                Log.d(TAG, "删除log日志zip文件" + file.getName() + " , 状态：" + deleteFile);
            }
        }
    }

    public File getAppLogger() {
        Context context = AppLogFactory.getAppLogConfig().getContext();
        //File filesDir = context.getFilesDir();
        File filesDir = context.getExternalFilesDir(null);
        if (filesDir == null) {
            return null;
        }
        String path = filesDir.getAbsolutePath() + File.separator + "logger";
        File file = new File(path);
        if (!file.exists()) {
            Log.d(TAG, "init palmLog create");
            //创建一个File对象所对应的目录，成功返回true，否则false。且File对象必须为路径而不是文件。
            //创建多级目录，创建路径中所有不存在的目录
            file.mkdirs();
        }
        Log.d(TAG, "init palmLog path : " + file.getPath());
        return file;
    }


    /**
     * 获取某个file对应的子file列表
     *
     * @param dir file文件
     * @return
     */
    private List<File> getFileList(File dir) {
        List<File> fileList = new ArrayList<>();
        if (dir.listFiles() != null) {
            File[] files = dir.listFiles();
            if (files == null || files.length <= 0) {
                return fileList;
            }
            int length = files.length;
            for (int i = 0; i < length; ++i) {
                File file = files[i];
                fileList.add(file);
            }
        }
        return fileList;
    }


    /**
     * 文件创建时间，方便测试查看缓存文件的最后修改时间
     *
     * @param file 文件
     */
    private long getFileTime(File file) {
        if (file != null && file.exists()) {
            return file.lastModified();
        }
        return 0L;
    }


    /**
     * 删除文件
     *
     * @param file file文件
     * @return true表示删除成功
     */
    private boolean deleteFile(final File file) {
        return file != null && (!file.exists() || file.isFile() && file.delete());
    }

}

