package com.yc.netlib.ping;

import android.os.Handler;
import android.os.Message;

import java.util.Hashtable;
import java.util.concurrent.Callable;
import java.util.concurrent.CancellationException;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.FutureTask;
import java.util.concurrent.ThreadPoolExecutor;

/**
 * <pre>
 *     @author yangchong
 *     GitHub : https://github.com/yangchong211/YCAndroidTool
 *     email : yangchong211@163.com
 *     time  : 2018/11/9
 *     desc  : AsyncTask
 *     revise: 之前投资界公共库
 * </pre>
 */
public abstract class NetAsyncTaskEx<Params, Progress, Result> {
    private static final int MESSAGE_POST_RESULT = 0x1;
    private static final int MESSAGE_POST_PROGRESS = 0x2;
    private static final int MESSAGE_POST_CANCEL = 0x3;

    private volatile Status mStatus = Status.PENDING;

    public enum Status {
        PENDING, RUNNING, FINISHED,
    }

    private static final LDNetInternalHandler sHandler = new LDNetInternalHandler();
    private final LDNetWorkerRunnable<Params, Result> mWorker;
    private final FutureTask<Result> mFuture;

     protected Hashtable<Params, NetAsyncTaskEx> mTaskCache = new Hashtable<Params, NetAsyncTaskEx>();

    public NetAsyncTaskEx() {
        mWorker = new LDNetWorkerRunnable<Params, Result>() {
            public Result call() throws Exception {
                // Process.setThreadPriority(Process.THREAD_PRIORITY_BACKGROUND);
                return doInBackground(mParams);
            }
        };

        mFuture = new FutureTask<Result>(mWorker) {
            @SuppressWarnings("unchecked")
            @Override
            protected void done() {
                Message message;
                Result result = null;

                try {
                    result = get();
                } catch (InterruptedException e) {
                    android.util.Log.w(this.getClass().getSimpleName(), e);
                } catch (ExecutionException e) {
                    throw new RuntimeException(
                            "An error occured while executing doInBackground()",
                            e.getCause());
                } catch (CancellationException e) {
                    message = sHandler.obtainMessage(MESSAGE_POST_CANCEL,
                            new LDNetAsyncTaskResult<Result>(NetAsyncTaskEx.this,
                                    (Result[]) null));
                    message.sendToTarget();
                    return;
                } catch (Throwable t) {
                    throw new RuntimeException(
                            "An error occured while executing "
                                    + "doInBackground()", t);
                }

                message = sHandler.obtainMessage(MESSAGE_POST_RESULT,
                        new LDNetAsyncTaskResult<Result>(NetAsyncTaskEx.this, result));
                message.sendToTarget();
            }
        };
    }

    private static class LDNetInternalHandler extends Handler {
        @SuppressWarnings({"rawtypes", "unchecked"})
        @Override
        public void handleMessage(Message msg) {
            LDNetAsyncTaskResult result = (LDNetAsyncTaskResult) msg.obj;
            switch (msg.what) {
                case MESSAGE_POST_RESULT:
                    // There is only one result
                    result.mTask.finish(result.mData[0]);
                    break;
                case MESSAGE_POST_PROGRESS:
                    result.mTask.onProgressUpdate(result.mData);
                    break;
                case MESSAGE_POST_CANCEL:
                    result.mTask.onCancelled();
                    break;
            }
        }
    }

    public final Status getStatus() {
        return mStatus;
    }

    protected abstract Result doInBackground(Params... params);

    /**
     * 后台线程准备运行阶段
     */
    protected void onPreExecute() {
    }

    /**
     * 后台运行阶段，当前运行已经结束
     *
     * @param result
     */
    protected void onPostExecute(Result result) {
    }

    /**
     * 进度更新阶段
     *
     * @param values
     */
    protected void onProgressUpdate(Progress... values) {
    }

    /**
     * 取消运行
     */
    protected void onCancelled() {
    }

    public final boolean isCancelled() {
        return mFuture.isCancelled();
    }

    public final boolean cancel(boolean mayInterruptIfRunning) {
        return mFuture.cancel(mayInterruptIfRunning);
    }

    /**
     * 初始化运行阶段
     *
     * @param params
     * @return
     */
    @SuppressWarnings("incomplete-switch")
    public final NetAsyncTaskEx<Params, Progress, Result> execute(Params... params) {
        if (mStatus != Status.PENDING) {
            switch (mStatus) {
                case RUNNING:
                    throw new IllegalStateException("Cannot execute task:"
                            + " the task is already running.");
                case FINISHED:
                    throw new IllegalStateException("Cannot execute task:"
                            + " the task has already been executed "
                            + "(a task can be executed only once)");
            }
        }

        mStatus = Status.RUNNING;

        onPreExecute();

        mWorker.mParams = params;
        ThreadPoolExecutor sExecutor = getThreadPoolExecutor();
        // ThreadPoolExecutor sExecutor = new ThreadPoolExecutor(CORE_POOL_SIZE,
        // MAXIMUM_POOL_SIZE, KEEP_ALIVE, TimeUnit.SECONDS, sWorkQueue,
        // sThreadFactory);
        if (sExecutor != null) {
            sExecutor.execute(mFuture);
            return this;
        } else {
            return null;
        }
    }

    protected abstract ThreadPoolExecutor getThreadPoolExecutor();

    protected final void publishProgress(Progress... values) {
        sHandler.obtainMessage(MESSAGE_POST_PROGRESS,
                new LDNetAsyncTaskResult<Progress>(this, values)).sendToTarget();
    }

    protected void finish(Result result) {
        if (isCancelled()) {
            result = null;
        }
        onPostExecute(result);
        mStatus = Status.FINISHED;
    }

    private static abstract class LDNetWorkerRunnable<Params, Result> implements Callable<Result> {
        Params[] mParams;
    }

    private static class LDNetAsyncTaskResult<Data> {
        @SuppressWarnings("rawtypes")
        final NetAsyncTaskEx mTask;
        final Data[] mData;

        LDNetAsyncTaskResult(@SuppressWarnings("rawtypes") NetAsyncTaskEx task, Data... data) {
            mTask = task;
            mData = data;
        }
    }
}
