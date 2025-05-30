
package com.yc.anrtoollib.caton.canary.monitor;

import android.content.Context;

import com.yc.anrtoollib.caton.canary.internal.BlockInfo;

public interface BlockInterceptor {
    void onBlock(Context context, BlockInfo blockInfo);
}
