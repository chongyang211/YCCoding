package com.yc.netlib.ping;


/**
 * <pre>
 *     @author yangchong
 *     GitHub : https://github.com/yangchong211/YCAndroidTool
 *     email : yangchong211@163.com
 *     time  : 2018/11/9
 *     desc  : 监控网络诊断的跟踪信息
 *     revise: 之前投资界公共库
 * </pre>
 */
public interface NetDiagnoListener {

    /**
     * 当结束之后返回日志
     *
     * @param log
     */
    void OnNetDiagnoFinished(String log);


    /**
     * 跟踪过程中更新日志
     *
     * @param log
     */
    void OnNetDiagnoUpdated(String log);

    /**
     * 监听状态
     * @param isDomainParseOk               域名解析是否成功
     * @param isSocketConnected             socket中conected是否成功
     */
    void OnNetStates(boolean isDomainParseOk, boolean isSocketConnected);
}
