package com.yc.toolutils.hardware;

import android.app.Activity;
import android.content.Context;
import android.graphics.Rect;
import android.os.IBinder;
import android.util.Log;
import android.view.View;
import android.view.ViewTreeObserver.OnGlobalLayoutListener;
import android.view.inputmethod.InputMethodManager;

import com.yc.toolutils.context.AppToolUtils;

import java.lang.reflect.Field;


/**
 * <pre>
 *     @author yangchong
 *     blog  : https://github.com/yangchong211
 *     time  : 2017/11/21
 *     desc  : 键盘相关工具类
 *     revise: v1.4 17年6月8日
 * </pre>
 */
public final class KeyboardUtils {

    private static int sContentViewInvisibleHeightPre;

    private KeyboardUtils() {

    }

    /*
      避免输入法面板遮挡
      <p>在 manifest.xml 中 activity 中设置</p>
      <p>android:windowSoftInputMode="adjustPan"</p>
     */

    /**
     * 动态显示软键盘
     *
     * @param activity activity
     */
    public static void showSoftInput(final Activity activity) {
        InputMethodManager imm =
                (InputMethodManager) activity.getSystemService(Activity.INPUT_METHOD_SERVICE);
        if (imm == null) {
            return;
        }
        View view = activity.getCurrentFocus();
        if (view == null) {
            view = new View(activity);
        }
        imm.showSoftInput(view, InputMethodManager.SHOW_FORCED);
    }

    /**
     * 动态显示软键盘
     *
     * @param view 视图
     */
    public static void showSoftInput(final View view) {
        InputMethodManager imm =
                (InputMethodManager) AppToolUtils.getApp().getSystemService(Context.INPUT_METHOD_SERVICE);
        if (imm == null) {
            return;
        }
        //给view设置焦点
        view.setFocusable(true);
        view.setFocusableInTouchMode(true);
        view.requestFocus();
        // SHOW_FORCED 强制显示，用于指示用户已强制打开输入法（例如通过长按菜单）的标志
        // showSoftInput(View, int)因此在他们明确这样做之前不应将其关闭。此常量在 API 级别 33 中已弃用。
        imm.showSoftInput(view, InputMethodManager.SHOW_FORCED);
        // SHOW_IMPLICIT，用于指示这是显示输入窗口的隐式请求的标志，而不是用户直接请求的结果。在这种情况下，窗口可能不会显示。
        //imm.showSoftInput(view, InputMethodManager.SHOW_IMPLICIT);
    }

    /**
     * 动态隐藏软键盘
     *
     * @param activity activity
     */
    public static void hideSoftInput(final Activity activity) {
        InputMethodManager imm =
                (InputMethodManager) activity.getSystemService(Activity.INPUT_METHOD_SERVICE);
        if (imm == null) {
            return;
        }
        View view = activity.getCurrentFocus();
        if (view == null) {
            view = new View(activity);
        }
        IBinder windowToken = view.getWindowToken();
        //指示软输入窗口仅在用户未明确显示时才应隐藏。
        imm.hideSoftInputFromWindow(windowToken, 0);
    }

    /**
     * 动态隐藏软键盘
     *
     * @param view 视图
     */
    public static void hideSoftInput(final View view) {
        InputMethodManager imm =
                (InputMethodManager) AppToolUtils.getApp().getSystemService(Context.INPUT_METHOD_SERVICE);
        if (imm == null) {
            return;
        }
        IBinder windowToken = view.getWindowToken();
        imm.hideSoftInputFromWindow(windowToken, 0);
    }

    /**
     * 切换软键盘显示与否状态
     */
    public static void toggleSoftInput() {
        InputMethodManager imm =
                (InputMethodManager) AppToolUtils.getApp().getSystemService(Context.INPUT_METHOD_SERVICE);
        if (imm == null) {
            return;
        }
        imm.toggleSoftInput(InputMethodManager.SHOW_FORCED, 0);
    }

    /**
     * 判断软键盘是否可见
     * <p>默认软键盘最小高度为 200</p>
     *
     * @param activity activity
     * @return {@code true}: 可见<br>{@code false}: 不可见
     */
    public static boolean isSoftInputVisible(final Activity activity) {
        return getContentViewInvisibleHeight(activity) >= 200;
    }

    /**
     * 判断软键盘是否可见
     *
     * @param activity             activity
     * @param minHeightOfSoftInput 软键盘最小高度
     * @return {@code true}: 可见<br>{@code false}: 不可见
     */
    public static boolean isSoftInputVisible(final Activity activity,
                                             final int minHeightOfSoftInput) {
        return getContentViewInvisibleHeight(activity) >= minHeightOfSoftInput;
    }

    private static int getContentViewInvisibleHeight(final Activity activity) {
        final View contentView = activity.findViewById(android.R.id.content);
        Rect r = new Rect();
        contentView.getWindowVisibleDisplayFrame(r);
        return contentView.getBottom() - r.bottom;
    }

    /**
     * 注册软键盘改变监听器
     *
     * @param activity activity
     * @param listener listener
     */
    public static void registerSoftInputChangedListener(final Activity activity,
                                                        final OnSoftInputChangedListener listener) {
        final View contentView = activity.findViewById(android.R.id.content);
        sContentViewInvisibleHeightPre = getContentViewInvisibleHeight(activity);
        contentView.getViewTreeObserver().addOnGlobalLayoutListener(new OnGlobalLayoutListener() {
            @Override
            public void onGlobalLayout() {
                if (listener != null) {
                    int height = getContentViewInvisibleHeight(activity);
                    if (sContentViewInvisibleHeightPre != height) {
                        listener.onSoftInputChanged(height);
                        sContentViewInvisibleHeightPre = height;
                    }
                }
            }
        });
    }

    /**
     * 修复软键盘内存泄漏
     * <p>在{@link Activity #onDestroy()}中使用</p>
     *
     * @param context context
     */
    public static void fixSoftInputLeaks(final Context context) {
        if (context == null) {
            return;
        }
        InputMethodManager imm =
                (InputMethodManager) AppToolUtils.getApp().getSystemService(Context.INPUT_METHOD_SERVICE);
        if (imm == null) {
            return;
        }
        String[] strArr = new String[]{"mCurRootView", "mServedView", "mNextServedView"};
        for (int i = 0; i < 3; i++) {
            try {
                Field declaredField = imm.getClass().getDeclaredField(strArr[i]);
                if (declaredField == null) {
                    continue;
                }
                if (!declaredField.isAccessible()) {
                    declaredField.setAccessible(true);
                }
                Object obj = declaredField.get(imm);
                if (obj == null || !(obj instanceof View)) {
                    continue;
                }
                View view = (View) obj;
                if (view.getContext() == context) {
                    declaredField.set(imm, null);
                } else {
                    return;
                }
            } catch (Throwable th) {
                th.printStackTrace();
            }
        }
    }

    /**
     * 点击屏幕空白区域隐藏软键盘
     * <p>根据 EditText 所在坐标和用户点击的坐标相对比，来判断是否隐藏键盘</p>
     * <p>需重写 dispatchTouchEvent</p>
     * <p>参照以下注释代码</p>
     */
    public static void clickBlankArea2HideSoftInput() {
        Log.i("KeyboardUtils", "Please refer to the following code.");
        /*
        @Override
        public boolean dispatchTouchEvent(MotionEvent ev) {
            if (ev.getAction() == MotionEvent.ACTION_DOWN) {
                View v = getCurrentFocus();
                if (isShouldHideKeyboard(v, ev)) {
                    InputMethodManager imm =
                            (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
                    imm.hideSoftInputFromWindow(v.getWindowToken(),
                            InputMethodManager.HIDE_NOT_ALWAYS
                    );
                }
            }
            return super.dispatchTouchEvent(ev);
        }

        // 根据 EditText 所在坐标和用户点击的坐标相对比，来判断是否隐藏键盘
        private boolean isShouldHideKeyboard(View v, MotionEvent event) {
            if (v != null && (v instanceof EditText)) {
                int[] l = {0, 0};
                v.getLocationInWindow(l);
                int left = l[0],
                        top = l[1],
                        bottom = top + v.getHeight(),
                        right = left + v.getWidth();
                return !(event.getX() > left && event.getX() < right
                        && event.getY() > top && event.getY() < bottom);
            }
            return false;
        }
        */
    }

    public interface OnSoftInputChangedListener {
        void onSoftInputChanged(int height);
    }
}
