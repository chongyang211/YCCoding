package com.yc.netlib.speed;

public interface ConnectionStateChangeListener {
    void onBandwidthStateChange(ConnectionQuality bandwidthState);
}
