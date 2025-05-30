/*
Copyright 2017 yangchong211（github.com/yangchong211）

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

package com.yc.ycthreadpoollib.pool.deliver;

import androidx.annotation.Nullable;

import java.util.concurrent.Executor;


/**
 * <pre>
 *     @author yangchong
 *     blog  : https://github.com/yangchong211
 *     time  :
 *     desc  : 默认情况下，用于Java平台的交付。
 *     revise:
 * </pre>
 */
public final class JavaDeliver implements Executor {

    private static final JavaDeliver instance = new JavaDeliver();

    /**
     * 使用单利模式获取对象
     * @return                      JavaDeliver对象
     */
    public static JavaDeliver getInstance() {
        return instance;
    }

    /**
     * 注意增加非空判断
     * @param runnable              runnable
     */
    @Override
    public void execute(@Nullable Runnable runnable) {
        if (runnable!=null){
            runnable.run();
        }
    }


}
