package com.yc.netlib.weaknetwork;


import org.jetbrains.annotations.NotNull;

import java.io.IOException;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.nio.charset.Charset;

import okio.Buffer;
import okio.BufferedSink;
import okio.ByteString;
import okio.Okio;
import okio.Sink;
import okio.Source;
import okio.Timeout;

/**
 * <pre>
 *     @author 杨充
 *     blog  : https://github.com/yangchong211
 *     time  : 2017/01/30
 *     desc  : 可以设置每次写入大小的BufferedSink
 *     revise:
 * </pre>
 */
public class ByteCountBufferedSink implements BufferedSink {

    private final long mByteCount;
    private final Sink mOriginalSink;
    private final BufferedSink mDelegate;

    public ByteCountBufferedSink(Sink sink, long byteCount) {
        this.mOriginalSink = sink;
        this.mDelegate = Okio.buffer(mOriginalSink);
        this.mByteCount = byteCount;
    }

    @Override
    public long writeAll(Source source) throws IOException {
        if (source == null){
            throw new IllegalArgumentException("source == null");
        }
        long totalBytesRead = 0;
        for (long readCount; (readCount = source.read(buffer(), mByteCount)) != -1; ) {
            totalBytesRead += readCount;
            emitCompleteSegments();
        }
        return totalBytesRead;
    }

    @NotNull
    @Override
    public BufferedSink write(byte[] source, int offset, int byteCount) throws IOException {
        if (!isOpen()){
            throw new IllegalStateException("closed");
        }
        //计算出要写入的次数
        long count = (long) Math.ceil((double) source.length / mByteCount);
        for (int i = 0; i < count; i++) {
            //让每次写入的字节数精确到mByteCount 分多次写入
            long newOffset = i * mByteCount;
            long writeByteCount = Math.min(mByteCount, source.length - newOffset);
            //写数据
            buffer().write(source, (int) newOffset, (int) writeByteCount);
            emitCompleteSegments();
        }
        return this;
    }

    @NotNull
    @Override
    public BufferedSink emitCompleteSegments() throws IOException {
        final Buffer buffer = buffer();
        mOriginalSink.write(buffer, buffer.size());
        return this;
    }

    @NotNull
    @Override
    public Buffer buffer() {
        return mDelegate.buffer();
    }

    @NotNull
    @Override
    public BufferedSink write(ByteString byteString) throws IOException {
        return mDelegate.write(byteString);
    }

    @NotNull
    @Override
    public BufferedSink write(byte[] source) throws IOException {
        return mDelegate.write(source);
    }

    @NotNull
    @Override
    public BufferedSink write(Source source, long byteCount) throws IOException {
        return mDelegate.write(source, byteCount);
    }

    @NotNull
    @Override
    public BufferedSink writeUtf8(String string) throws IOException {
        return mDelegate.writeUtf8(string);
    }

    @NotNull
    @Override
    public BufferedSink writeUtf8(String string, int beginIndex, int endIndex) throws IOException {
        return mDelegate.writeUtf8(string, beginIndex, endIndex);
    }

    @NotNull
    @Override
    public BufferedSink writeUtf8CodePoint(int codePoint) throws IOException {
        return mDelegate.writeUtf8CodePoint(codePoint);
    }

    @NotNull
    @Override
    public BufferedSink writeString(String string, Charset charset) throws IOException {
        return mDelegate.writeString(string, charset);
    }

    @NotNull
    @Override
    public BufferedSink writeString(String string, int beginIndex, int endIndex, Charset charset) throws IOException {
        return mDelegate.writeString(string, beginIndex, endIndex, charset);
    }

    @NotNull
    @Override
    public BufferedSink writeByte(int b) throws IOException {
        return mDelegate.writeByte(b);
    }

    @NotNull
    @Override
    public BufferedSink writeShort(int s) throws IOException {
        return mDelegate.writeShort(s);
    }

    @NotNull
    @Override
    public BufferedSink writeShortLe(int s) throws IOException {
        return mDelegate.writeShortLe(s);
    }

    @NotNull
    @Override
    public BufferedSink writeInt(int i) throws IOException {
        return mDelegate.writeInt(i);
    }

    @Override
    public BufferedSink writeIntLe(int i) throws IOException {
        return mDelegate.writeIntLe(i);
    }

    @Override
    public BufferedSink writeLong(long v) throws IOException {
        return mDelegate.writeLong(v);
    }

    @Override
    public BufferedSink writeLongLe(long v) throws IOException {
        return mDelegate.writeLongLe(v);
    }

    @Override
    public BufferedSink writeDecimalLong(long v) throws IOException {
        return mDelegate.writeDecimalLong(v);
    }

    @Override
    public BufferedSink writeHexadecimalUnsignedLong(long v) throws IOException {
        return mDelegate.writeHexadecimalUnsignedLong(v);
    }

    @Override
    public void flush() throws IOException {
        mDelegate.flush();
    }

    @NotNull
    @Override
    public BufferedSink emit() throws IOException {
        return mDelegate.emit();
    }

    @NotNull
    @Override
    public OutputStream outputStream() {
        return mDelegate.outputStream();
    }

    @Override
    public int write(ByteBuffer src) throws IOException {
        return mDelegate.write(src);
    }

    @Override
    public boolean isOpen() {
        return mDelegate.isOpen();
    }

    @Override
    public void write(Buffer source, long byteCount) throws IOException {
        mDelegate.write(source, byteCount);
    }

    @NotNull
    @Override
    public Timeout timeout() {
        return mDelegate.timeout();
    }

    @Override
    public void close() throws IOException {
        mDelegate.close();
    }

    @NotNull
    @Override
    public Buffer getBuffer() {
        return mDelegate.getBuffer();
    }

    @NotNull
    @Override
    public BufferedSink write(ByteString byteString, int i, int i1) throws IOException {
        return mDelegate.write(byteString, i, i1);
    }
}
