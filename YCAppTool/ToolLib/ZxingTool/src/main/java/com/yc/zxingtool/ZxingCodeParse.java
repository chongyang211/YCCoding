package com.yc.zxingtool;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import androidx.annotation.NonNull;

import com.google.zxing.BarcodeFormat;
import com.google.zxing.BinaryBitmap;
import com.google.zxing.DecodeHintType;
import com.google.zxing.MultiFormatReader;
import com.google.zxing.RGBLuminanceSource;
import com.google.zxing.Result;
import com.google.zxing.common.GlobalHistogramBinarizer;
import com.google.zxing.common.HybridBinarizer;
import com.google.zxing.qrcode.QRCodeReader;

import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

/**
 * 解析二维码工具类
 */
public final class ZxingCodeParse {


    public static final int DEFAULT_REQ_WIDTH = 450;
    public static final int DEFAULT_REQ_HEIGHT = 800;


    /**
     * 解析一维码/二维码图片
     *
     * @param bitmapPath 路径
     * @return 内容
     */
    public static String parseCode(String bitmapPath) {
        Map<DecodeHintType, Object> hints = new HashMap<>();
        //添加可以解析的编码类型
        Vector<BarcodeFormat> decodeFormats = new Vector<>();
        decodeFormats.addAll(DecodeManager.ONE_D_FORMATS);
        decodeFormats.addAll(DecodeManager.QR_CODE_FORMATS);
        decodeFormats.addAll(DecodeManager.DATA_MATRIX_FORMATS);
        decodeFormats.addAll(DecodeManager.AZTEC_FORMATS);
        decodeFormats.addAll(DecodeManager.PDF417_FORMATS);

        hints.put(DecodeHintType.CHARACTER_SET, "utf-8");
        hints.put(DecodeHintType.TRY_HARDER, Boolean.TRUE);
        hints.put(DecodeHintType.POSSIBLE_FORMATS, decodeFormats);
        return parseCode(bitmapPath, hints);
    }

    /**
     * 解析一维码/二维码图片
     *
     * @param bitmapPath 路径
     * @param hints      解析编码类型
     * @return 码内容
     */
    public static String parseCode(String bitmapPath, Map<DecodeHintType, Object> hints) {
        Result result = parseCodeResult(bitmapPath, hints);
        if (result != null) {
            return result.getText();
        }
        return null;
    }

    /**
     * 解析一维码/二维码图片
     *
     * @param bitmapPath 路径
     * @param hints      解析编码类型
     * @return 码内容
     */
    public static Result parseCodeResult(String bitmapPath, Map<DecodeHintType, Object> hints) {
        return parseCodeResult(bitmapPath, DEFAULT_REQ_WIDTH, DEFAULT_REQ_HEIGHT, hints);
    }


    /**
     * 解析一维码/二维码图片
     *
     * @param bitmapPath 路径
     * @param reqWidth   宽
     * @param reqHeight  高
     * @param hints      解析编码类型
     * @return 码内容
     */
    public static Result parseCodeResult(String bitmapPath, int reqWidth,
                                         int reqHeight, Map<DecodeHintType, Object> hints) {
        Result result = null;
        try {
            MultiFormatReader reader = new MultiFormatReader();
            reader.setHints(hints);
            RGBLuminanceSource source = getRGBLuminanceSource(
                    compressBitmap(bitmapPath, reqWidth, reqHeight));
            if (source != null) {

                boolean isReDecode;
                try {
                    BinaryBitmap bitmap = new BinaryBitmap(new HybridBinarizer(source));
                    result = reader.decodeWithState(bitmap);
                    isReDecode = false;
                } catch (Exception e) {
                    isReDecode = true;
                }

                if (isReDecode) {
                    try {
                        BinaryBitmap bitmap = new BinaryBitmap(new HybridBinarizer(source.invert()));
                        result = reader.decodeWithState(bitmap);
                        isReDecode = false;
                    } catch (Exception e) {
                        isReDecode = true;
                    }
                }

                if (isReDecode) {
                    try {
                        BinaryBitmap bitmap = new BinaryBitmap(
                                new GlobalHistogramBinarizer(source));
                        result = reader.decodeWithState(bitmap);
                        isReDecode = false;
                    } catch (Exception e) {
                        isReDecode = true;
                    }
                }

                if (isReDecode && source.isRotateSupported()) {
                    try {
                        BinaryBitmap bitmap = new BinaryBitmap(
                                new HybridBinarizer(source.rotateCounterClockwise()));
                        result = reader.decodeWithState(bitmap);
                    } catch (Exception e) {

                    }
                }
                reader.reset();
            }
        } catch (Exception e) {
        }
        return result;
    }


    /**
     * 解析二维码图片
     *
     * @param bitmapPath 路径
     * @return 码内容
     */
    public static String parseQRCode(String bitmapPath) {
        Map<DecodeHintType, Object> hints = new HashMap<>();
        hints.put(DecodeHintType.CHARACTER_SET, "utf-8");
        hints.put(DecodeHintType.TRY_HARDER, Boolean.TRUE);
        return parseQRCode(bitmapPath, hints);
    }

    /**
     * 解析二维码图片
     *
     * @param bitmapPath 路径
     * @param hints      配置参数
     * @return 码内容
     */
    public static String parseQRCode(String bitmapPath, Map<DecodeHintType, ?> hints) {
        Result result = parseQRCodeResult(bitmapPath, hints);
        if (result != null) {
            return result.getText();
        }
        return null;
    }

    /**
     * 解析二维码图片
     *
     * @param bitmapPath 路径
     * @param hints      配置参数
     * @return 码内容
     */
    public static Result parseQRCodeResult(String bitmapPath, Map<DecodeHintType, ?> hints) {
        return parseQRCodeResult(bitmapPath, DEFAULT_REQ_WIDTH, DEFAULT_REQ_HEIGHT, hints);
    }

    /**
     * 解析二维码图片
     *
     * @param bitmapPath 路径
     * @param reqWidth   宽
     * @param reqHeight  高
     * @param hints      配置参数
     * @return 码内容
     */
    public static Result parseQRCodeResult(String bitmapPath, int reqWidth,
                                           int reqHeight, Map<DecodeHintType, ?> hints) {
        Result result = null;
        try {
            QRCodeReader reader = new QRCodeReader();
            Bitmap compressBitmap = compressBitmap(bitmapPath, reqWidth, reqHeight);
            RGBLuminanceSource source = getRGBLuminanceSource(compressBitmap);
            if (source != null) {
                boolean isReDecode;
                try {
                    BinaryBitmap bitmap = new BinaryBitmap(new HybridBinarizer(source));
                    result = reader.decode(bitmap, hints);
                    isReDecode = false;
                } catch (Exception e) {
                    isReDecode = true;
                }
                if (isReDecode) {
                    try {
                        BinaryBitmap bitmap = new BinaryBitmap(new HybridBinarizer(source.invert()));
                        result = reader.decode(bitmap, hints);
                        isReDecode = false;
                    } catch (Exception e) {
                        isReDecode = true;
                    }
                }
                if (isReDecode) {
                    try {
                        BinaryBitmap bitmap = new BinaryBitmap(new GlobalHistogramBinarizer(source));
                        result = reader.decode(bitmap, hints);
                        isReDecode = false;
                    } catch (Exception e) {
                        isReDecode = true;
                    }
                }
                if (isReDecode && source.isRotateSupported()) {
                    try {
                        BinaryBitmap bitmap = new BinaryBitmap(
                                new HybridBinarizer(source.rotateCounterClockwise()));
                        result = reader.decode(bitmap, hints);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
                reader.reset();
            }
        } catch (Exception e) {
        }
        return result;
    }

    /**
     * 获取RGBLuminanceSource
     *
     * @param bitmap bitmap
     * @return 资源
     */
    private static RGBLuminanceSource getRGBLuminanceSource(@NonNull Bitmap bitmap) {
        int width = bitmap.getWidth();
        int height = bitmap.getHeight();

        int[] pixels = new int[width * height];
        bitmap.getPixels(pixels, 0, bitmap.getWidth(), 0, 0,
                bitmap.getWidth(), bitmap.getHeight());
        return new RGBLuminanceSource(width, height, pixels);
    }

    /**
     * 压缩图片
     */
    public static Bitmap compressBitmap(String path, int reqWidth, int reqHeight) {
        BitmapFactory.Options newOpts = new BitmapFactory.Options();
        // 开始读入图片，此时把options.inJustDecodeBounds 设回true了
        //获取原始图片大小
        newOpts.inJustDecodeBounds = true;
        // 此时返回bm为空
        BitmapFactory.decodeFile(path, newOpts);
        float width = newOpts.outWidth;
        float height = newOpts.outHeight;
        // 缩放比，由于是固定比例缩放，只用高或者宽其中一个数据进行计算即可
        // wSize=1表示不缩放
        int wSize = 1;
        if (width > reqWidth) {
            // 如果宽度大的话根据宽度固定大小缩放
            wSize = (int) (width / reqWidth);
        }
        // wSize=1表示不缩放
        int hSize = 1;
        // 如果高度高的话根据宽度固定大小缩放
        if (height > reqHeight) {
            hSize = (int) (height / reqHeight);
        }
        int size = Math.max(wSize, hSize);
        if (size <= 0) {
            size = 1;
        }
        // 设置缩放比例
        newOpts.inSampleSize = size;
        // 重新读入图片，注意此时已经把options.inJustDecodeBounds 设回false了
        newOpts.inJustDecodeBounds = false;
        Bitmap bitmap = BitmapFactory.decodeFile(path, newOpts);
        return bitmap;
    }


}
