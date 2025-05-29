package com.yc.zxingserver.tools;

import android.graphics.Bitmap;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.os.Message;
import android.text.TextUtils;
import android.util.Log;

import androidx.annotation.NonNull;

import com.google.zxing.BarcodeFormat;
import com.google.zxing.BinaryBitmap;
import com.google.zxing.DecodeHintType;
import com.google.zxing.MultiFormatReader;
import com.google.zxing.PlanarYUVLuminanceSource;
import com.google.zxing.RGBLuminanceSource;
import com.google.zxing.ReaderException;
import com.google.zxing.Result;
import com.google.zxing.common.GlobalHistogramBinarizer;
import com.google.zxing.common.HybridBinarizer;

import java.util.EnumMap;
import java.util.Map;
import java.util.Vector;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * 解析摄像头二进制数据
 * 扫码场景，yuv类型
 */
public final class ZxingYuvHelper {

    private static volatile ZxingYuvHelper instance;
    private static final String TAG = "DecodeCode:";
    private DecodeListener decodeListener;
    private static final int DECODE_YUV = 1;
    private DecodeThread decodeThread;
    private int width;
    private int height;
    private MultiFormatReader multiFormatReader;
    private long lastFeedTime = 0;
    private final AtomicBoolean isInit = new AtomicBoolean(false);
    private final AtomicBoolean isAvailable = new AtomicBoolean(true);
    private final AtomicInteger countLong = new AtomicInteger(0);
    private long lastResultTime = 0;

    public static ZxingYuvHelper getInstance() {
        if (instance == null) {
            synchronized (ZxingYuvHelper.class) {
                if (instance == null) {
                    instance = new ZxingYuvHelper();
                }
            }
        }
        return instance;
    }

    public void setDecodeListener(DecodeListener decodeListener) {
        this.decodeListener = decodeListener;
    }

    private ZxingYuvHelper() {
        if (isInit.get()) {
            d(TAG + "已经初始化了");
        } else {
            init();
        }
    }

    private void init() {
        HandlerThread thread = new HandlerThread("qr_decode_thread");
        thread.start();
        decodeThread = new DecodeThread(thread.getLooper());
        //扫码初始化
        multiFormatReader = new MultiFormatReader();
        //添加可以解析的编码类型
        Vector<BarcodeFormat> decodeFormats = new Vector<>();
        decodeFormats.addAll(DecodeManager.ONE_D_FORMATS);
        decodeFormats.addAll(DecodeManager.QR_CODE_FORMATS);
        decodeFormats.addAll(DecodeManager.DATA_MATRIX_FORMATS);
        decodeFormats.addAll(DecodeManager.AZTEC_FORMATS);
        decodeFormats.addAll(DecodeManager.PDF417_FORMATS);

        //Collection<BarcodeFormat> decodeFormats = EnumSet.of(BarcodeFormat.QR_CODE, BarcodeFormat.CODE_128);
        Map<DecodeHintType, Object> hints = new EnumMap<>(DecodeHintType.class);
        hints.put(DecodeHintType.POSSIBLE_FORMATS, decodeFormats);
        String characterSet = "utf-8";
        hints.put(DecodeHintType.CHARACTER_SET, characterSet);
        hints.put(DecodeHintType.TRY_HARDER, true);
        multiFormatReader.setHints(hints);
        d(TAG + "初始化完成");
        isInit.set(true);
    }

    public void release() {
        isAvailable.set(false);
        decodeListener = null;
        decodeThread.getLooper().quit();
        multiFormatReader = null;
        isInit.set(false);
        d(TAG + "销毁");
    }

    public void pushYuv(byte[] yuv, int width, int height) {
        if (isAvailable.get()) {
            isAvailable.set(false);
            this.width = width;
            this.height = height;
            decodeThread.obtainMessage(DECODE_YUV, yuv).sendToTarget();
            //PalmSdkUtils.d(TAG + "pushYuv " + yuv.length);
            countLong.getAndIncrement();
        }
    }

    private class DecodeThread extends Handler {
        DecodeThread(Looper looper) {
            super(looper);
        }

        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            if (msg != null && msg.what == DECODE_YUV) {
                decode((byte[]) msg.obj, width, height);
                isAvailable.set(true);
            }
        }
    }

    public PlanarYUVLuminanceSource buildLuminanceSource(byte[] data, int width, int height) {
        //public PlanarYUVLuminanceSource(byte[] yuvData, //传入的帧数据
        //                                  int dataWidth, //数据源图片宽度
        //                                  int dataHeight, //数据源图片高度
        //                                  int left, //识别区域的左边距
        //                                  int top, //识别区域的上边距
        //                                  int width, //识别区域的宽度
        //                                  int height, //识别区域的高度
        //                                  boolean reverseHorizontal //是否需要旋转图片
        //                                  ）
        return new PlanarYUVLuminanceSource(data, width, height, 0, 0,
                width, height, false);
    }


    public RGBLuminanceSource buildRGBLuminanceSource(byte[] data, int width, int height) {
        int[] pixels = new int[width * height];
        // 将byte[]转换为RGB格式的像素数组
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int index = y * width + x;
                int r = data[index] & 0xff;
                int g = data[index] & 0xff;
                int b = data[index] & 0xff;
                pixels[index] = 0xff000000 | (r << 16) | (g << 8) | b;
            }
        }
        return new RGBLuminanceSource(width,height,pixels);
    }

    /**
     * 获取RGBLuminanceSource
     *
     * @param bitmap bitmap
     * @return 资源
     */
    private RGBLuminanceSource getRGBLuminanceSource(@NonNull Bitmap bitmap) {
        int width = bitmap.getWidth();
        int height = bitmap.getHeight();
        int[] pixels = new int[width * height];
        bitmap.getPixels(pixels, 0, bitmap.getWidth(), 0, 0,
                bitmap.getWidth(), bitmap.getHeight());
        return new RGBLuminanceSource(width, height, pixels);
    }

    /**
     * 刷码解析
     *
     * @param data   data数据
     * @param width  宽
     * @param height 高
     */
    public void decode(byte[] data, int width, int height) {
        long startTime = System.currentTimeMillis();
        //Java中的nanoTime是一个用于获取当前系统时间的方法。与其他获取时间的方法相比，nanoTime的精确度更高，可用于需要高精度计时的场景。
        //这个时间戳往往会在计算时间差、比较时间等场景下被使用。
        //long startTime = System.nanoTime();
        if (startTime - lastFeedTime > 3000) {
            //PalmSdkUtils.d(TAG + "decode start 3秒回调 " + countLong.get() + "次");
            lastFeedTime = startTime;
            countLong.set(0);
        }
        PlanarYUVLuminanceSource source = buildLuminanceSource(data, width, height);
        Result rawResult = null;
        boolean isReDecode;
        int count = 0;
        try {
            BinaryBitmap bitmap = new BinaryBitmap(new HybridBinarizer(source));
            count++;
            rawResult = multiFormatReader.decodeWithState(bitmap);
            isReDecode = false;
        } catch (ReaderException re) {
            isReDecode = true;
        }
        if (isReDecode) {
            try {
                BinaryBitmap bitmap = new BinaryBitmap(new HybridBinarizer(source.invert()));
                count++;
                rawResult = multiFormatReader.decodeWithState(bitmap);
                isReDecode = false;
            } catch (Exception e) {
                isReDecode = true;
            }
        }
        if (isReDecode) {
            try {
                BinaryBitmap bitmap = new BinaryBitmap(new GlobalHistogramBinarizer(source));
                count++;
                rawResult = multiFormatReader.decodeWithState(bitmap);
                isReDecode = false;
            } catch (Exception e) {
                isReDecode = true;
            }
        }
        if (isReDecode && source.isRotateSupported()) {
            try {
                BinaryBitmap bitmap = new BinaryBitmap(new HybridBinarizer(source.rotateCounterClockwise()));
                count++;
                rawResult = multiFormatReader.decodeWithState(bitmap);
            } catch (Exception e) {
                d(TAG + "decode exception " + e);
            }
        }
        multiFormatReader.reset();
        if (rawResult != null) {
            BarcodeFormat format = rawResult.getBarcodeFormat();
            //类型
            String type = format.toString();
            String content = rawResult.getText().trim();
            //必须是二维码，避免一些其他格式的
            String qrCode = BarcodeFormat.QR_CODE.toString();
            d(TAG + "decode result type " + type + " , " + qrCode + " ; content " + content);
            if (!TextUtils.isEmpty(content) && qrCode.equals(type)) {
                decodeSuccess(content, count, startTime);
            } else {
                //空数据
                if (decodeListener != null) {
                    decodeListener.onDecodeFailed(-1);
                }
            }
        } else {
            //PalmSdkUtils.d(TAG + "decode result null");
            //失败
            if (decodeListener != null) {
                decodeListener.onDecodeFailed(-2);
            }
        }
    }

    private void decodeSuccess(String content, long count, long startTime) {
        try {
            long time = System.currentTimeMillis() - lastResultTime;
            if (time < 500) {
                d(TAG + "fix 短时间内，出现多次二维码解析回调");
                return;
            }
            if (decodeListener != null) {
                long endTime = System.currentTimeMillis();
                long codeTime = endTime - startTime;
                d(TAG + "识别执行解析次数" + count + "；decode 耗时" + codeTime + " , " + time);
                decodeListener.onDecodeSuccess(content);
            }
        } finally {
            lastResultTime = System.currentTimeMillis();
        }
    }

    private void d(String msg) {
        Log.d("Zxing" , msg);
    }

    public interface DecodeListener {
        /**
         * 解析二维码
         *
         * @param content 二维码数据
         */
        void onDecodeSuccess(final String content);

        /**
         * 解析失败
         *
         * @param error 异常
         */
        void onDecodeFailed(final int error);
    }
}