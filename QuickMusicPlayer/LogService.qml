import QtQuick

// 定义为全局单例对象
// 使用 pragma Singleton 声明为全局单例对象，确保整个应用程序中只有一个实例。
pragma Singleton


// 通过 QtObject 实现日志服务的核心功能。
QtObject {
    id: logService

    // 日志来源常量
    readonly property int sourceApp: 0
    readonly property int sourceModule: 1

    // 日志级别常量
    readonly property int levelError: 0
    readonly property int levelWarn: 1
    readonly property int levelInfo: 2
    readonly property int levelDebug: 3
    readonly property int levelTrace: 4
    readonly property int levelSystem: 99

    // 日志过滤器状态
    property var filters: {
        // 显示此级别及以下的日志
        "level": 4,
        // 应用日志、模块日志
        "sources": [true, true] ,
        // 根据关键字过滤日志内容。
        "search": ""
    }

    // 日志缓冲区
    // logBuffer 用于存储日志条目，maxBufferSize 限制缓冲区的大小。
    // 当缓冲区超过最大容量时，移除最早的日志条目。
    property var logBuffer: []
    property int maxBufferSize: 1000

    // 日志发送信号
    signal log(var logEntry)

    // 清除日志信号
    signal clearLogs

    // 直接 API 方法 - 新增
    // 提供了便捷的日志记录方法（error、warn、info 等），用于记录不同级别的日志。
    function error(message) {
        logMessage(message, levelError);
    }

    function warn(message) {
        logMessage(message, levelWarn);
    }

    function info(message) {
        logMessage(message, levelInfo);
    }

    function debug(message) {
        logMessage(message, levelDebug);
    }

    function trace(message) {
        logMessage(message, levelTrace);
    }

    function system(message) {
        logMessage(message, levelSystem);
    }

    // 处理接收到的日志
    function receiveLog(source, level, message) {
        // 创建日志条目
        var entry = {
            timestamp: new Date().toLocaleTimeString(Qt.locale(), "hh:mm:ss.zzz"),
            source: source,
            level: level,
            message: message,
            isSystemInfo: level === levelSystem
        };

        // 添加到缓冲区
        addToBuffer(entry);

        // 应用过滤器
        if (shouldShowLog(entry)) {
            // 发送到订阅者
            log(entry);
        }
    }

    // 添加到缓冲区
    function addToBuffer(entry) {
        logBuffer.push(entry);
        if (logBuffer.length > maxBufferSize) {
            logBuffer.shift();
        }
    }

    // 检查日志是否应该显示
    // shouldShowLog 方法根据过滤规则判断日志是否应该显示。
    function shouldShowLog(entry) {
        // 级别过滤
        if (entry.level > filters.level && entry.level !== levelSystem) {
            return false;
        }

        // 来源过滤
        if (entry.source >= 0 && entry.source < filters.sources.length && !filters.sources[entry.source]) {
            return false;
        }

        // 搜索文本过滤
        if (filters.search !== "" && entry.message.toLowerCase().indexOf(filters.search.toLowerCase()) === -1) {
            return false;
        }

        return true;
    }

    // 设置日志级别过滤器
    function setLevelFilter(level) {
        try {
            // 使用 console.log 是安全的，不会产生循环引用
            console.log("LogService: 设置日志级别为 " + level);
            filters.level = level;
            return true;
        } catch (e) {
            console.error("LogService: 设置日志级别失败: " + e);
            return false;
        }
    }

    // 设置日志来源过滤器
    function setSourceFilter(source, enabled) {
        try {
            if (source >= 0 && source < filters.sources.length) {
                filters.sources[source] = enabled;
                return true;
            }
            return false;
        } catch (e) {
            console.error("LogService: 设置来源过滤器失败: " + e);
            return false;
        }
    }

    // 设置搜索过滤器
    function setSearchFilter(text) {
        try {
            filters.search = text;
            return true;
        } catch (e) {
            console.error("LogService: 设置搜索过滤器失败: " + e);
            return false;
        }
    }

    // 直接记录日志的便捷方法
    function logMessage(message, level) {
        try {
            fallbackToConsole(message, level);
            return true;
        } catch (e) {
            console.error("LogService: 记录日志失败: " + e);
            // 如果发生异常，尝试退回到 console
            fallbackToConsole(message, level);
            return false;
        }
    }

    // 退回到控制台打印的辅助方法
    function fallbackToConsole(message, level) {
        var prefix = "";
        switch (level) {
        case levelError:
            prefix = "[错误] ";
            console.error(prefix + message);
            break;
        case levelWarn:
            prefix = "[警告] ";
            console.warn(prefix + message);
            break;
        case levelInfo:
            prefix = "[信息] ";
            console.log(prefix + message);
            break;
        case levelDebug:
            prefix = "[调试] ";
            console.log(prefix + message);
            break;
        case levelTrace:
            prefix = "[跟踪] ";
            console.log(prefix + message);
            break;
        default:
            console.log(message);
        }
    }

    // 清除所有日志
    function clear() {
        try {
            logBuffer = [];
            clearLogs();
            return true;
        } catch (e) {
            console.error("LogService: 清除日志失败: " + e);
            return false;
        }
    }

    // 检查服务是否可用的公共方法
    function isAvailable() {
        return true;
    }

    // 初始化
    Component.onCompleted: {
        // 设置默认过滤级别（显示所有级别）
        // setLevelFilter(4);
        console.log("LogService singleton initialized");
    }
}
