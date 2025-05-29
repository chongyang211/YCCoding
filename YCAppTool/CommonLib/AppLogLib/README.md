# 日志打印和存储库
#### 目录介绍
- 01.基础概念介绍
- 02.Api调用说明
- 03.如何写日志数据
- 04.日志其他操作
- 05.日志框架优化


## 01.基础概念介绍

日志系统设计需要考虑的问题：

1. 需要保证日志系统收集到的数据的有效性和完整性，在app崩溃时日志不丢失。
2. 保证收集日志的过程不影响app的性能，不能占用过高cpu资源，或者频繁IO造成卡顿现象。

日志保存策略设计大概如下：

由于保存日志的过程是个耗时过程，我们需要开启线程去保存。但是日志产生的频率可能很高，又不能采用一般的线程去处理，太多的线程也会损耗性能。所以应该考虑队列的形式保存日志，然后一条一条的去保存。

## 02.Api调用说明

通用日志库框架，专用`LogCat`工具，主要功能全局配置log输出, 个性化设置Tag，可以设置日志打印级别，支持设置日期自动清理过期日志，支持打印复杂对象，可以实现自定义日志接口，支持简化版本将日志写入到文件中。小巧好用！

第一步：初始化操作：

``` java
String ycLogPath = AppFileUtils.getCacheFilePath(this, "ycLog");
AppLogConfig config = new AppLogConfig.Builder()
        //设置日志tag总的标签
        .setLogTag("yc")
        //是否将log日志写到文件
        .isWriteFile(true)
        //是否是debug
        .enableDbgLog(true)
        //设置日志最小级别
        .minLogLevel(Log.VERBOSE)
        //设置输出日志到file文件的路径。前提是将log日志写入到文件设置成true
        .setFilePath(ycLogPath)
        .build();
//配置
AppLogFactory.init(config);
```

第二步：使用Log日志，十分简单，如下所示

``` java
//自己带有tag标签
AppLogHelper.d("MainActivity: ","debug log");
//使用全局tag标签
AppLogHelper.d("MainActivity log info no tag");
```

其他的一些api设计，如下所示：

```
//当然，如果不满足你的要求，开发者可以自己实现日志输出的形式。
AppLogFactory.addPrinter(new AbsPrinter() {
    @NonNull
    @Override
    public String name() {
        return "yc";
    }

    @Override
    public void println(int level, String tag, String message, Throwable tr) {
        //todo 这块全局回调日志，你可以任意实现自定义操作
    }
});
```

## 03.如何写日志数据

如何写数据的设计：创建Writer对象，调用writer.write(string)进行写入数据。

``` java
try {
    PrintWriter writer = getWriter();
    writer.println(log);
    if (tr != null) {
        tr.printStackTrace(writer);
        writer.println();
    }
    writer.flush();
} catch (Throwable ignored) {
}
```

`flush`写数据时机，粗略分为四种情况：

1. 缓存池满
2. 应用从前台进入后台
3. 应用从后台恢复至前台
4. 手动代码调用。手动代码调用一般发生在进入超高频触发区之前，或者是跨功能模块时，以及用户交互需求日志上传时。


## 04.日志其他操作

### 4.1 组装日志数据

比如，打印日志如下所示：`AppLogHelper.i("Log test info : click text view test")`

日志组装后输出到控制台：`yc_log:: [(LogTestActivity.kt:110)#OnClick] Log test info : click text view test`

携带方法和的类的源文件行号，支持点击跳转到源文件。那么如何组装数据：

- 获取当前堆栈信息，然后获取指定索引处的`className`和`methodName`。也就是：`(LogTestActivity.kt:110)#OnClick`
- 从中获取方法执行的线程相关的信息，以及执行的方法名称等。这些信息能帮助我们更好的查找问题之所在。


## 05.日志框架优化


### 5.1 存储数据队列设计

1. 采集一次日志就保存到file文件中吗

这个采集过程，如果是每产生一次日志数据，就写一次文件，势必造成系统IO调用的频繁操作，可能会造成app运行卡顿现象。因此采用队列保存日志，当达到某数值则开始写入到文件，简单来说是追加写入。

2. 队列缓存大小，单位(条)，也可以动态修改容积。

比如进入日志高频触发区，则将缓存容积扩大到100条，进入低频触发区就缩小到10条。容积缩小时，如果缓存内条目数已经大于目标条数，自动触发flush。

3. 在初始化的时候，创建一个日志队列线程并开始

4. 开启while循环，从日志队列取数据，如果有数据则doFlush()发送到LogService，通过Aidl进行数据发送。


### 5.2 高效日志方案实践

移动日志系统使用了Linux系统中提供的mmap作为日志文件的载体

目前业内流行的XLOG日志组件、MMKV、美团Logan均采用了此方案，其最大的优势就是高效I/O、低损耗、跨进程 等优势。

对于移动端日志采集SDK来说：主要进行的工作就是将用户写入的数据保存到文件中，在这个过程中涉及到在native层调用mmap函数实现在进程虚拟内存地址空间中分配地址空间，创建和物理内存的映射关系。

使用mmap技术优化，mmap: 是一种内存映射文件的方法，它将一个文件映射到进程的地址空间中，实现文件磁盘地址和进程虚拟地址空间中一段虚拟地址的一一对映关系。实现这样的映射关系后，进程就可以采用指针的方式读写操作这一段内存，而系统会自动回写脏页面到对应的文件磁盘上


### 参考
- Qt教程(1) : 用Log4Qt自定义日志系统 原创
    - https://blog.51cto.com/u_13267193/5392834
- 高性能日志框架 Log4a 原理分析
    - https://www.pianshen.com/article/19061482533/


