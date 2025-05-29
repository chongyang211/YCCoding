package com.yc.serialport.jni;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * hex字符处理工具类
 */
public final class ByteTranslateUtils {


    /**
     * bit = 0/1       就是一个二进制数，这是计算机运算的最小单位，也是最基本的单位，什么都会转为01来计算
     * byte  = 8bit    byte也就是字节。是文件存储的最小单位。也就是说一个字节 = 8个二进制数
     * <p>
     * AscII编码：unicode 存储英文和标点就是一个字节 1 byte 存中文就要2byte.
     * UTF-8 ：存英文要1个byte,存中文要3个byte。
     * Unicode编码：一个英文等于两个字节，一个中文（含繁体）等于两个字节。
     * <p>
     * java采用unicode
     * <p>
     * Byte 字节， 8位；
     * Boolean ，1个字节
     * Short ， 2个字节，16位；
     * char ,2个字节，16位；
     * Int ， 4个字节，32位；
     * float， 4个字节，32位；
     * Long ，8个字节，64位；
     * double，8个字节，64位；
     */
    private static final char[] HEX_CHAR = {'0', '1', '2', '3', '4', '5',
            '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};


    /**
     * 判断奇数或偶数，位运算，最后一位是1则为奇数，为0是偶数
     *
     * @param num 数据
     * @return 是否是奇数
     */
    private static int isOdd(int num) {
        return num & 0x1;
    }

    /**
     * 将int转成hex字符串
     *
     * @param number 数据
     * @return 字符串
     */
    public static String intToHex(int number) {
        String st = Integer.toHexString(number).toUpperCase();
        return String.format("%2s", st).replaceAll(" ", "0");
    }

    /**
     * 1字节转2个Hex字符
     *
     * @param inByte byte
     * @return
     */
    public static String byte2Hex(Byte inByte) {
        return String.format("%02x", inByte).toUpperCase();
    }

    /**
     * 方法功能：判断一个字符串是否有16进制
     *
     * @param s 内容
     * @return boolean
     */
    public static boolean isHexString(String s) {
        Pattern p = Pattern.compile("^[A-Fa-f0-9]+$");
        Matcher m = p.matcher(s);
        return m.matches();
    }


    /**
     * 建议使用第一种
     * 将字节数组转化为十六位字符串，第一种
     *
     * @param bytes 字节数组
     * @return
     */
    public static String bytesToHex1(byte[] bytes) {
        // 一个byte为8位，可用两个十六进制位标识
        char[] buf = new char[bytes.length * 2];
        int a = 0;
        int index = 0;
        for (byte b : bytes) {
            // 使用除与取余进行转换
            if (b < 0) {
                a = 256 + b;
            } else {
                a = b;
            }
            buf[index++] = HEX_CHAR[a / 16];
            buf[index++] = HEX_CHAR[a % 16];
        }
        return new String(buf);
    }


    /**
     * 将字节数组转化为十六位字符串，第二种
     *
     * @param data 字节数组
     * @return
     */
    public static String bytesToHex2(byte[] data) {
        StringBuilder sb = new StringBuilder();
        for (byte b : data) {
            String format = String.format("%02x", b);
            sb.append(format);
        }
        return sb.toString();
    }

    /**
     * 字节数组转转hex字符串
     *
     * @param bytes 字节数组
     * @return hex字符串
     */
    public static String bytesToHex2_2(byte[] bytes) {
        StringBuilder strBuilder = new StringBuilder();
        int j = bytes.length;
        for (int i = 0; i < j; i++) {
            strBuilder.append(byte2Hex(bytes[i]));
            strBuilder.append(" ");
        }
        return strBuilder.toString();
    }

    /**
     * 字节数组转转hex字符串，可选长度
     *
     * @param inBytArr  字节数组
     * @param offset    起始点
     * @param byteCount 长度
     * @return
     */
    public static String bytesToHex2_3(byte[] inBytArr, int offset, int byteCount) {
        StringBuilder strBuilder = new StringBuilder();
        int j = byteCount;
        for (int i = offset; i < j; i++) {
            strBuilder.append(byte2Hex(inBytArr[i]));
        }
        return strBuilder.toString();
    }


    /**
     * 将字节数组转化为十六位字符串，第三种
     * 手动实现字节数组转换为十六进制字符串的算法，需要考虑多种特殊情况。
     * 例如，需要注意字节顺序、字符串长度等问题。建议在实际开发中使用成熟的开源库，而不是手动实现算法。
     *
     * @param data 字节数组
     * @return
     */
    public static String bytesToHex3(byte[] data) {
        //使用 StringBuilder 类来拼接最终的十六进制字符串
        StringBuilder sb = new StringBuilder();
        for (byte b : data) {
            //在循环处理每个字节时，我们使用 Integer.toHexString() 方法将其转换为一个长度为 2 的十六进制字符串
            String hexString = Integer.toHexString(b & 0xFF);
            //注意补充前导零（如果需要）
            if (hexString.length() == 1) {
                hexString = "0" + hexString;
            }
            //将每个字符串拼接在一起，得到最终的十六进制字符串 hexString
            sb.append(hexString);
        }
        return sb.toString();
    }


    /**
     * 将16进制字符串转换为byte[]
     *
     * @param str 字符串
     * @return byte
     */
    public static byte[] hexToBytes1(String str) {
        if (str == null || str.trim().equals("") || !isHexString(str)) {
            return new byte[0];
        }
        byte[] bytes = new byte[str.length() / 2];
        try {
            for (int i = 0; i < str.length() / 2; i++) {
                String subStr = str.substring(i * 2, i * 2 + 2);
                bytes[i] = (byte) Integer.parseInt(subStr, 16);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return bytes;
    }


    /**
     * 将16进制字符串转换为byte[]
     *
     * @param inHex 字符串
     * @return byte
     */
    public static byte[] hexToBytes2(String inHex) {
        int hexlen = inHex.length();
        byte[] result;
        if (isOdd(hexlen) == 1) {
            //奇数
            ++hexlen;
            result = new byte[hexlen / 2];
            inHex = "0" + inHex;
        } else {
            //偶数
            result = new byte[hexlen / 2];
        }
        int j = 0;
        for (int i = 0; i < hexlen; i += 2) {
            result[j] = hexToByte(inHex.substring(i, i + 2));
            ++j;
        }
        return result;
    }

    private static byte hexToByte(String inHex) {
        return (byte) Integer.parseInt(inHex, 16);
    }
}
