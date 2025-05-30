package com.yc.mmaplog.formatter;

import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.Locale;

import com.yc.mmaplog.LogLevel;
import com.yc.mmaplog.inter.InterFormatter;


public class DateFileFormatter implements InterFormatter {
    private SimpleDateFormat simpleDateFormat = null;
    private Date mDate;
    private String lastDataFormated = null;
    private StringBuffer mStringBuffer;
    private int mTimeLength = 0;

    public DateFileFormatter() {
        this("yyyy:MM:dd HH:mm:ss");
    }

    public DateFileFormatter(String pattern) {
        simpleDateFormat = new SimpleDateFormat(pattern, Locale.getDefault());
        mStringBuffer = new StringBuffer();
        //重置秒数
        Calendar instance = Calendar.getInstance();
        instance.set(Calendar.SECOND, 0);
        mDate = instance.getTime();
    }

    public synchronized String format(int logLevel, String tag, String msg) {
        if ((System.currentTimeMillis() - mDate.getTime()) > 1000 || lastDataFormated == null) {
            mDate.setTime(System.currentTimeMillis());
            lastDataFormated = simpleDateFormat.format(mDate);
            resetTimePrefix();
            return formatString(logLevel, tag, msg);
        }
        return formatString(logLevel, tag, msg);
    }

    private void resetTimePrefix() {
        if (mStringBuffer.length() > 0) {
            mStringBuffer.delete(0, mStringBuffer.length());
        }
        mTimeLength = mStringBuffer.append(lastDataFormated).append(' ').length();
    }

    private String formatString(int logLevel, String tag, String msg) {
        if (mStringBuffer.length() > mTimeLength) {
            mStringBuffer.delete(mTimeLength, mStringBuffer.length());
        }
        return mStringBuffer.append(LogLevel.getShortLevelName(logLevel)).append(tag).append(msg).append('\n').toString();
    }
}