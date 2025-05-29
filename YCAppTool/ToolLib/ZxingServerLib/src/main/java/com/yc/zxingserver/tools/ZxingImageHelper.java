package com.yc.zxingserver.tools;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
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
 * 扫码场景，image类型
 */
public final class ZxingImageHelper {

    private static volatile ZxingImageHelper instance;
    private static final String TAG = "DecodeCode:";
    private DecodeListener decodeListener;
    private static final int DECODE_IMAGE = 2;
    private DecodeThread decodeThread;
    private MultiFormatReader multiFormatReader;
    private final AtomicBoolean isInit = new AtomicBoolean(false);
    private final AtomicBoolean isAvailable = new AtomicBoolean(true);
    private final AtomicInteger countLong = new AtomicInteger(0);

    public static ZxingImageHelper getInstance() {
        if (instance == null) {
            synchronized (ZxingImageHelper.class) {
                if (instance == null) {
                    instance = new ZxingImageHelper();
                }
            }
        }
        return instance;
    }

    public void setDecodeListener(DecodeListener decodeListener) {
        this.decodeListener = decodeListener;
    }

    private ZxingImageHelper() {
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

    public void pushImage(byte[] data) {
        if (isAvailable.get()) {
            isAvailable.set(false);
            decodeThread.obtainMessage(DECODE_IMAGE, data).sendToTarget();
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
            if (msg != null && msg.what == DECODE_IMAGE) {
                decode((byte[]) msg.obj);
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
        return new RGBLuminanceSource(width, height, pixels);
    }

    /**
     * 获取RGBLuminanceSource
     * RGBLuminanceSource是一个用于从RGB图像中提取亮度信息的类。
     * RGBLuminanceSource类的主要作用是将RGB图像转换为灰度图像，并提供对灰度图像中像素值的访问。
     * 它使用加权平均法将RGB图像的红、绿、蓝三个通道的像素值转换为灰度值，然后将灰度值存储在一个二维数组中。
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
     * @param data data数据
     */
    private void decode(byte[] data) {
        long startTime = System.currentTimeMillis();
        //Java中的nanoTime是一个用于获取当前系统时间的方法。与其他获取时间的方法相比，nanoTime的精确度更高，可用于需要高精度计时的场景。
        //这个时间戳往往会在计算时间差、比较时间等场景下被使用。
        //long startTime = System.nanoTime();
        Bitmap imageQr = BitmapFactory.decodeByteArray(data, 0, data.length);
        RGBLuminanceSource source = getRGBLuminanceSource(imageQr);
        Result rawResult = null;
        boolean isReDecode;
        int count = 0;
        //BinaryBitmap类的主要作用是为解码器提供一个可读取的二进制图像。
        //解码器可以使用BinaryBitmap来解码包含在图像中的条形码或二维码。
        try {
            BinaryBitmap bitmap = new BinaryBitmap(new HybridBinarizer(source));
            count++;
            //MultiFormatReader类是ZXing库中的核心类之一，它可以识别和解码多种不同类型的条码和二维码.
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
                if (decodeListener != null) {
                    long endTime = System.currentTimeMillis();
                    long codeTime = endTime - startTime;
                    d(TAG + "识别执行解析次数" + count + "；decode 耗时" + codeTime);
                    decodeListener.onDecodeSuccess(content);
                }
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


    /**
     * 刷码解析
     *
     * @param data   data数据
     * @param width  宽
     * @param height 高
     */
    private void decode(byte[] data, int width, int height) {
        long startTime = System.currentTimeMillis();
        //Java中的nanoTime是一个用于获取当前系统时间的方法。与其他获取时间的方法相比，nanoTime的精确度更高，可用于需要高精度计时的场景。
        //这个时间戳往往会在计算时间差、比较时间等场景下被使用。
        //long startTime = System.nanoTime();
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
                if (decodeListener != null) {
                    long endTime = System.currentTimeMillis();
                    long codeTime = endTime - startTime;
                    d(TAG + "识别执行解析次数" + count + "；decode 耗时" + codeTime);
                    decodeListener.onDecodeSuccess(content);
                }
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

    private void d(String msg) {
        Log.d("Zxing", msg);
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