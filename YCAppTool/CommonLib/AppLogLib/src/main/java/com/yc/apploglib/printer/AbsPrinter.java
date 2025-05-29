package com.yc.apploglib.printer;


import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

/**
 * <pre>
 *     @author yangchong
 *     GitHub : https://github.com/yangchong211/YCCommonLib
 *     email : yangchong211@163.com
 *     time  : 2018/11/9
 *     desc  : 抽象的Printer
 *     revise:
 * </pre>
 */
public abstract class AbsPrinter {

    /**
     * name
     *
     * @return 字符串
     */
    @NonNull
    public abstract String name();

    /**
     * 打印
     *
     * @param level   level等级
     * @param tag     tag标签
     * @param message message
     * @param tr      异常
     */
    public abstract void println(int level, String tag, String message, Throwable tr);

    /**
     * 清理过期文件，默认是7天
     */
    public void clearOldFile() {
        //子类可以自由实现
    }

    @Override
    public final boolean equals(@Nullable Object obj) {
        if (obj instanceof AbsPrinter) {
            if (obj == this) {
                return true;
            }
            AbsPrinter p = (AbsPrinter) obj;
            return name().equals(p.name());
        }
        return false;
    }
}
