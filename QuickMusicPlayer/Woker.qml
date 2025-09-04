import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


// 高级动画图片组件实现，支持动画播放、帧控制、加载状态管理、页面过渡等功能。
// 适用于需要播放动画（如 GIF、APNG）并需要精细控制的场景，例如页面过渡、加载动画等。
Item {
    id: animatedImageRoot

    // ============= 公开属性 =============
    // 动画源 (接受路径或ImageProvider)
    property alias source: animatedImage.source
    property alias loops: animatedImage.loops
    // 动画控制
    // 是否播放
    property bool playing: false
    // 状态跟踪
    property bool loading: false
    property bool error: false
    // 页面过渡控制
    property bool pageTransitionActive: false
    // 尺寸策略
    // 是否填充容器
    property bool fillContainer: true

    // ============= 信号 =============
    // ∙started()：动画开始播放时触发
    // ∙finished()：动画正常结束时触发
    // ∙loadFailed(errorString)：加载失败时触发
    // ∙transitionFinished()：页面过渡完成时触发
    signal started();
    signal finished();
    signal loadFailed(string errorString);
    // 在过渡完成或超时时触发 transitionFinished 信号。包括异常
    signal transitionFinished();

    // ============= 组件实现 =============
    implicitWidth: fillContainer ? parent.width : animatedImage.implicitWidth
    implicitHeight: fillContainer ? parent.height : animatedImage.implicitHeight

    Component.onDestruction: {
       console.log("animatedImageRoot onDestruction");
    }

    Component.onCompleted: {
        console.log("animatedImageRoot onCompleted");
    }

    // 主动画图片组件
    AnimatedImage {
        id: animatedImage
        // 在后台线程加载
        asynchronous: true
        // 是否缓存解码后的帧
        cache: true
        // 加载完成且无错误时显示
        visible: status === AnimatedImage.Ready && !error
        // 播放速度控制（1.0为正常速度）
        speed: 1.0
        // 循环次数1（默认无限循环）
        // loops: 1
        // 尺寸策略
        width: fillContainer ? parent.width : implicitWidth
        // 支持根据父组件尺寸自动调整大小，或保持原始尺寸。
        height: fillContainer ? parent.height : implicitHeight
        anchors.centerIn: parent
        // 填充模式
        fillMode: fillContainer ? Image.PreserveAspectFit : Image.Stretch

        // 状态变化信号 处理动画状态变化
        onStatusChanged: {
            console.log("animatedImageRoot onStatusChanged");
            // 加载状态变化
            handleStatusChange(status);
        }

        // 帧变化
        onCurrentFrameChanged: {
            console.log("animatedImageRoot onCurrentFrameChanged");
        }

        // 播放状态变化
        onPlayingChanged: {
            console.log("animatedImageRoot onPlayingChanged" + playing);
            if (playing) {
                started();
            }
        }

        // 暂停状态变化
        onPausedChanged: {
            console.log("animatedImageRoot onPausedChanged");
        }

        // 动画结束处理
        onFrameCountChanged: {
            console.log("animatedImageRoot onFrameCountChanged");
            // 循环次数（默认无限循环）
            // currentFrame 当前帧索引（可读写）
            if (animatedImage.loops !== AnimatedImage.Infinite
                && animatedImage.currentFrame === animatedImage.frameCount - 1) {
                console.log("animatedImageRoot finished");
                // 动画结束信号
                finished();
                // 完成进入动画后，清理状态
                enterAnimation.visible = false;
                completeTransition();
            }
        }

        // 资源释放
        Component.onDestruction: {
            console.log("animatedImageRoot animatedImagesource null");
            // 释放资源
            animatedImage.source = "";
            if (transitionTimeout) {
                // 停止过渡超时计时器
                transitionTimeout.stop();
            }
        }
    }

    // 加载指示器
    BusyIndicator {
        id: loadingIndicator
        anchors.centerIn: parent
        running: loading
        visible: running
    }

    // ============= 自定义功能方法 =============
    // 重新加载资源
    function reload() {
        error = false;
        var currentSource = animatedImage.source;
        // 动画源文件路径（支持本地文件、网络URL和qrc资源）
        animatedImage.source = "";
        animatedImage.source = currentSource;
    }

    // 跳转到特定帧
    function seek(frame) {
        if (frame >= 0 && frame < animatedImage.frameCount) {
            animatedImage.currentFrame = frame;
        }
    }

    // 跳转到下一帧
    function nextFrame() {
        seek(animatedImage.currentFrame + 1)
    }

    // 跳转到上一帧
    function prevFrame() {
        seek(animatedImage.currentFrame - 1)
    }

    // 用于页面过渡的专用函数
    function startPageTransition(duration = 1000) {
        pageTransitionActive = true;
        // 设置运行
        playing = true;
        // 开始展示布局
        enterAnimation.visible = true;
        // 设置过渡超时保护
        transitionTimeout.restart();
        console.log("animatedImageRoot startPageTransition");
    }

    // ============= 内部状态处理 =============
    function handleStatusChange(status) {
        console.log("animatedImageRoot handleStatusChange " + status);
        switch (status) {
            // 动画加载中
        case AnimatedImage.Loading:
            loading = true;
            error = false;
            break;
            // 动画加载完成
        case AnimatedImage.Ready:
            loading = false;
            error = false;
            break;
            // 错误处理
        case AnimatedImage.Error:
            loading = false;
            error = true;
            enterAnimation.visible = false;
            // 对外提供回调
            loadFailed(animatedImage.errorString);
            completeTransition();
            break;
        }
    }

    // 完成调用这个js方法
    function completeTransition() {
        if (pageTransitionActive) {
            pageTransitionActive = false
            // 调用结束
            transitionFinished()
        }
    }

    // 过渡动画超时保护
    // 防止页面过渡动画卡住，设置超时保护。
    Timer {
        id: transitionTimeout
        interval: 2000 // 2秒超时
        onTriggered: {
            console.warn("Page transition timed out");
            completeTransition();
        }
    }
}

