#### 目录介绍
- 01.整体概述
  - 1.1 项目背景
  - 1.2 遇到问题记录
  - 1.3 基础概念介绍
  - 1.4 设计目标
  - 1.5 产生收益分析
- 02.通知栏基础介绍
  - 2.1 为何适配通知栏
  - 2.2 通知栏前期困境
  - 2.3 系统8.0的适配
  - 2.4 一定适配8.0吗
  - 2.5 悬挂式通知失效
- 03.通知栏简单实践
  - 3.1 创建通知的渠道
  - 3.2 发送通知栏操作
  - 3.3 取消通知栏操作
  - 3.4 管理通知栏渠道
  - 3.5 删除通知栏渠道
  - 3.5 显示未读的图标
  - 3.7 前台服务通知栏
  - 3.8 设置锁屏通知栏
- 04.通知栏遇到的坑
  - 4.1 缺少属性不显示通知栏
- 05.通用方案思路
  - 5.1 适合多种业务场景
- 06.方案基础设计
  - 6.1 整体架构图
  - 6.2 UML设计图
  - 6.3 关键流程图
  - 6.4 接口设计图
  - 6.5 模块间依赖关系
- 07.其他设计说明
  - 7.1 性能设计
  - 7.2 稳定性设计
  - 7.3 灰度设计
  - 7.4 降级设计
  - 7.5 异常设计
- 08.其他说明介绍
  - 8.1 参考链接


### 01.整体概述
#### 1.1 项目背景
- **Notification通知用途**
  - 显示接收到短消息、及时消息等信息；显示客户端的推送消息，如广告、优惠、版本更新、推荐新闻等，与用户交互
  - 显示正在进行的事物，例如：后台运行的程序，如音乐播放进度、下载进度等，实时的任务提醒，也会与用户交互
- 通知栏使用场景众多，针对通知栏8.0适配，通知栏channel管理等，打造一款通用通知栏适配方案比较重要。



#### 1.2 遇到问题记录
- Android O(8.0)通知的改变
  - NotificationChannel是android8.0新增的特性，如果App的targetSDKVersion>=26，没有设置channel通知渠道的话，就会导致通知无法展示。
  - 如果不适配，则报错内容：Failed to post notification on channel “null” Target Api is 26
  - 报错解决方案：第一种：临时方案，google也考虑到适配问题，临时兼容方案是targetSDKVersion低于26；第二种：创建channel
- setFullScreenIntent无效
  - 在某些平台上，当用户正在使用设备时，系统UI可能会选择显示一个提醒通知，而不是启动这个意图。


#### 1.3 基础概念介绍
- 普通Notification：发出通知后，通知会先以图标的形式显示在状态栏中。
- 折叠式Notification：折叠式Notification是一种自定义视图的Notification，用来显示长文本和一些自定义的布局的场景。它有两种状态，一种是普通状态下的视图，一种是展开状态下的视图。
- 悬挂式Notification：Head up通知，android5.0新增，悬挂式Notification不需要下拉通知栏就直接显示出来悬挂在屏幕上方并且焦点不变仍在用户操作的界面因此不会打断用户的操作，过几秒就会自动消失。这种行为通常适用于用户应立即知晓的重要通知，而且仅在设备未锁定时才会显示。


#### 1.4 设计目标


#### 1.5 产生收益分析



### 02.通知栏基础介绍
#### 2.1 为何适配通知栏
- 通知栏的设计确实非常巧妙
  - 它默认情况下不占用任何空间，只有当用户需要的时候用手指在状态栏上向下滑动，通知栏的内容才会显示出来，这在智能手机发展的初期极大地解决了手机屏幕过小，内容展示区域不足的问题。
- 通知栏信息过大难容忍
  - 现在每当安装一个新的App时，都会先到设置里面去找一找有没有推送开关，如果有的话我会第一时间把它关掉。
  - 而如果一个App经常推送垃圾信息却又无法关闭时，会直接将它的通知总开关给关掉，如果还不是什么重要的App的话，那么我可能就直接将它卸载掉了。
  - 为什么一个很好的通知栏功能现在却变得这么遭用户讨厌？很大一部分原因都是因为开发者没有节制地使用导致的。



#### 2.2 通知栏前期困境
- 通知栏又还是有用处的，比如我们收到微信、短信等消息的时候，确实需要通知栏给我们提醒。
  - 因此分析下来，通知栏目前最大的问题就是，无法让用户对感兴趣和不感兴趣的消息进行区分。
- 举一个小案例
  - 就比如说，我希望淘宝向我推送卖家发货和物流的相关消息，但是我不想收到那些打折促销或者是让我去买衣服的这类消息。那么就目前来说，是没有办法对这些消息做区分的，我要么同意接受所有消息，要么就屏蔽所有消息，这是当前通知栏的痛点。



#### 2.3 系统8.0的适配
- 从Android 8.0系统开始，Google引入了通知渠道这个概念。
  - 什么是通知渠道呢？顾名思义，就是每条通知都要属于一个对应的渠道。每个App都可以自由地创建当前App拥有哪些通知渠道，但是这些通知渠道的控制权都是掌握在用户手上的。用户可以自由地选择这些通知渠道的重要程度，是否响铃、是否振动、或者是否要关闭这个渠道的通知。
- 设置通知栏channel好处
  - 拥有了这些控制权之后，用户就再也不用害怕那些垃圾推送消息的打扰了，因为用户可以自主地选择自己关心哪些通知、不关心哪些通知。
- 举一个例子，谷歌地图
  - 比如我希望收到商家，用户发来的通知消息，但是我不想看到附近活动通知，我去过地点的通知，那么用户可以有选择性开启和关闭某些通知的。
- 通知栏渠道划分怎么理解
  - 对于每个App来说，通知渠道的划分是非常需要仔细考究的，因为通知渠道一旦创建之后就不能再修改了，因此开发者需要仔细分析自己的App一共有哪些类型的通知，然后再去创建相应的通知渠道。


#### 2.4 一定适配8.0吗
- 如果你将项目中的targetSdkVersion指定到了26或者更高，那么Android系统就会认为你的App已经做好了8.0系统的适配工作，当然包括了通知栏的适配。
- 这个时候如果还不使用通知渠道的话，那么你的App的通知将完全无法弹出。因此这里给大家的建议就是，一定要适配。




#### 2.6 悬挂式通知失效
- 如何实现悬挂式通知
  - 对于实现效果众说纷纭，大多倾向于调用NotificationCompat.Builder的方法setFullScreenIntent。但在不同机器上，似乎实现效果也不尽相同。
- 使用Heads Up Notification也有必要前提条件。
  - Android API >=21，即5.0以上；需要在设置中开启横幅通知权限（在设置通知管理中，不同机型可能存在差异）
  - 需要设置震动或声音。（必须！）；需要设置Notification为最高权限。（必须！）



### 03.通知栏简单实践
- 大概就是下面这四步操作
  - 创建NotificationManager来对通知进行管理，通过调用getSystemService(NOTIFICATION_SERVICE)来获得NotificationManager实例；
  - 在Android8.0以后要对版本进行适配；
  - 使用NotificationCompat设置标题 内容 图片等属性；
  - 调用通知管理类的notify()方法弹出通知栏；



#### 3.1 创建通知的渠道
##### 3.3.1 通知栏渠道创建
- 简单代码如下所示：
    ``` java
    @TargetApi(Build.VERSION_CODES.O)
    private void createNotificationChannel(String channelId, String channelName, int importance) {
        NotificationChannel channel = new NotificationChannel(channelId, channelName, importance);
        mNotificationManager.createNotificationChannel(channel);
    }
    ```
- 创建一个通知渠道的方式非常简单
  - 创建一个通知渠道至少需要渠道ID、渠道名称以及重要等级这三个参数。
  - 其中渠道ID可以随便定义，只要保证全局唯一性就可以。
  - 渠道名称是给用户看的，需要能够表达清楚这个渠道的用途。
  - 重要等级的不同则会决定通知的不同行为，当然这里只是初始状态下的重要等级，用户可以随时手动更改某个渠道的重要等级，App是无法干预的。


##### 3.3.2 通知栏优先级
- 为何要设置优先级
  - 模拟了这样一个场景。想象一下我们正在开发一个类似于微信的App，其中App通知主要可以分为两类：
  - 一类是我和别人的聊天消息，这类消息非常重要，因此重要等级我设为了`IMPORTANCE_HIGH`。
  - 另一类是公众号的订阅消息，这类消息不是那么重要，因此重要等级我设为了`IMPORTANCE_DEFAULT`。
  - 除此之外，重要等级还可以设置为`IMPORTANCE_LOW`、`IMPORTANCE_MIN`，分别对应了更低的通知重要程度。
- 然后查看设置中心app通知
  - 运行成功之后我们关闭App，进入到设置 -> 应用 -> 通知当中，查看这个App的通知界面，如下图所示：
  - 可以看到，由于这两个通知渠道的重要等级不同，通知的行为也是不同的，聊天消息可以发出提示音并在屏幕上弹出通知，而订阅消息只能发出提示音。


##### 3.3.3 Channel创建多次怎么办
- 创建通知渠道的代码只在第一次执行的时候才会创建
  - 以后每次执行创建代码系统会检测到该通知渠道已经存在了，因此不会重复创建，也并不会影响任何效率。


#### 3.2 发送通知栏操作
##### 3.2.1 如何发送通知栏
- 触发通知的代码和之前版本基本是没有任何区别的，只是在构建通知对象的时候，需要多传入一个通知渠道ID，表示这条通知是属于哪个渠道的。
  - 注意点1：在NotificationCompat.Builder中需要多传入一个通知渠道ID，那么这里我们分别传入了chat和subscribe渠道ID。
    ``` java
    public void sendChatMsg() {
        Notification notification = new NotificationCompat.Builder(this, "chat")
                //省略部分代码
                .build();
        manager.notify(1, notification);
    }
    ```
- 展示一下效果图，如下截图所示：
  - 图3
- 为什么通知栏显示提醒通知和通知静音呢？
  - 提醒通知：创建通知栏channel时，由于消息，这是一条重要等级高的通知，因此会使用这种屏幕弹窗的方式来通知用户有消息到来。
  - 静音通知：接下来点击发送邮件通知按钮，你会发现现在屏幕上不会弹出一条通知提醒了，只会在状态栏上显示一个小小的通知图标
  - 图4，图5


##### 3.2.1 Android8.0通知的变化
- 图7
- 快速向左或者向右滑动可以关闭一条通知，但如果你缓慢地向左或者向右滑动，就会看到这样两个按钮：
  - 一个是时钟延迟，另一个是设置按钮
- 时钟延迟按钮
  - 左边那个时钟图标的按钮可以让通知延迟显示。比方说这是一条比较重要的通知，但是我暂时没时间看，也不想让它一直显示在状态栏里打扰我，我就可以让它延迟一段后时间再显示，这样我就暂时能够先将精力放在专注的事情上，等过会有时间了这条通知会再次显示出来，我不会错过任何信息。如下所示：
  - 图7
- 设置按钮
  - 右边那个设置图标的按钮就可以用来对通知渠道进行屏蔽和配置了，用户对每一个App的每一个通知渠道都有绝对的控制权，可以根据自身的喜好来进行配置和修改。如下所示：
  - 图8
  - 比如说我觉得订阅消息老是向我推荐广告，实在是太烦了，我就可以将订阅消息的通知渠道关闭掉。这样我以后就不会再收到这个通知渠道下的任何消息，而聊天消息却不会受到影响，这就是8.0系统通知渠道最大的特色。


#### 3.3 管理通知栏渠道
- 通知渠道一旦创建之后就不能再通过代码修改了。既然不能修改的话那还怎么管理呢？
  - 为此，Android赋予了开发者读取通知渠道配置的权限，如果我们的某个功能是必须按照指定要求来配置通知渠道才能使用的，那么就可以提示用户去手动更改通知渠道配置。
- 举一个例子说明
  - 想一想我们开发的是一个类似于微信的App，聊天消息是至关重要的，如果用户不小心将聊天消息的通知渠道给关闭了，那岂不是所有重要的信息全部都丢了？为此我们一定要保证用户打开了聊天消息的通知渠道才行。
- 注意获取channel的方式
  - 通过getNotificationChannel()方法获取到了NotificationChannel对象，然后就可以读取该通知渠道下的所有配置了。
    ```
    //生效
    NotificationChannel channel = manager.getNotificationChannel("chat");
    ```
- 具体判断的逻辑，代码如下所示：
  - 判断如果通知渠道的importance等于IMPORTANCE_NONE，就说明用户将该渠道的通知给关闭了，这时会跳转到通知的设置界面提醒用户手动打开。
    ```
    public void openChannelSetting(NotificationChannel channel){
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            if (channel.getImportance() == NotificationManager.IMPORTANCE_NONE) {
                Intent intent = new Intent(Settings.ACTION_CHANNEL_NOTIFICATION_SETTINGS);
                intent.putExtra(Settings.EXTRA_APP_PACKAGE, getPackageName());
                intent.putExtra(Settings.EXTRA_CHANNEL_ID, channel.getId());
                startActivity(intent);
            }
        }
    }
    ```
- 如下所示
  - 图9
  - 当我们将指定消息的通知渠道关闭后，下次再次发送消息将会直接跳转到通知设置界面，提醒用户手动将通知打开。


#### 3.4 删除通知栏渠道
- Android 8.0还赋予了我们删除通知渠道的功能，只需使用如下代码即可删除：
    ```
    NotificationManager manager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
    manager.deleteNotificationChannel(channelId);
    ```
- 这个功能非常不建议使用。
  - 因为Google为了防止应用程序随意地创建垃圾通知渠道，会在通知设置界面显示所有被删除的通知渠道数量，如下图所示：
  - 图10
- 如何删除所有通知栏
  - 创建通知栏，8.0之后会有不同的channelId。那么我是怎么才能获取用app中其他地方的这个值呢？
  - 举一个简单的例子，比如说谷歌sdk，在创建通知栏之前，会根据包名删除该应用其他通知栏。这个是怎么做到的？



#### 3.5 显示未读的图标
- 未读角标功能
  - 长期以来都没有一个标准的API来实现角标功能，很多都是要通过向系统发送广播来实现的，而各个手机厂商的广播标准又不一致，经常导致代码变得极其混杂。
  - 从8.0系统开始，Google制定了Android系统上的角标规范，也提供了标准的API，长期让开发者头疼的这个问题现在终于可以得到解决了。
- 主要修改两个地方
  - 第一是在创建通知渠道的时候，调用了NotificationChannel的setShowBadge(true)方法，表示允许这个渠道下的通知显示角标。
  - 第二是在创建通知的时候，调用了setNumber()方法，并传入未读消息的数量。
- Android系统默认也是会显示角标
  - 图11
  - 需要注意的是，即使不调用setShowBadge(true)方法，Android系统默认也是会显示角标的，但是如果你想禁用角标功能，那么记得一定要调用setShowBadge(false)方法。



#### 3.7 前台服务通知栏
- 为了保活操作，开启前台Service打开通知栏思考？
  - Service几乎都是在后台运行的，所以Service系统优先级还是比较低的，当系统出现内存不足情况时，就有可能回收掉正在后台运行的Service。
  - 如果希望Service可以一直保持运行状态，而不会由于系统内存不足的原因导致被回收，那么就要提高Service的优先级，而提高优先级的方法有多种，其中一种就是考虑使用前台Service。
- 如何把Service设置成前台Service？
  - 很简单，使用startForeground即可；要取消前台，使用stopForeground即可。
- Android 8.0 行为变更说明
  - 系统不允许后台应用创建后台服务。 因此，Android 8.0 引入了一种全新的方法，即 Context.startForegroundService()，以在前台启动新服务。
  - 在系统创建服务后，应用有五秒的时间来调用该服务的 startForeground() 方法以显示新服务的用户可见通知。
- startForeground流程梳理一下
  - startForeground -----> startForeground---> [setServiceForeground]
  - ActivityManagerService ----> setServiceForeground ---> [setServiceForegroundLocked]
  - ActiveServices ---- > setServiceForegroundLocked --->
  - ActiveServices ---- > setServiceForegroundInnerLocked ---> [r.postNotification()]
  - ServiceRecord ---- > postNotification -----> enqueueNotification 展示通知
- startForeground大概原理梳理下
  - 待完善
- 不调用startForeground会怎样？
  - 发现通过startForegroundService开启Service，不调用startForeground会出现崩溃。
  - 这个是为什么呢？然后开始定位分析，找到抛出：Context.startForegroundService() did not then call Service.startForeground()
  - 通过崩溃日志，定位到 ActiveServices 类 中的 serviceForegroundTimeout方法，还有 serviceForegroundCrash



#### 3.8 设置锁屏通知栏



### 05.创建通知的渠道


#### 4.1 缺少属性不显示通知栏
- 创建notification必须添加的属性
  - 小图标，通过 setSmallIcon() 方法设置；标题，通过 setContentTitle() 方法设置；内容，通过 setContentText() 方法设置。发现如果缺少这几个元素，通知栏无法正常展示出来。
- 为什么？
  - 待完善





### 06.方案基础设计
#### 6.1 整体架构图
- 通知栏相关类
  - Notification，通知对应类，保存通知相关的数据。
  - NotificationChannel
  - NotificationChannelGroup
  - NotificationManager
    - NotificationManager 是通知管理类，它是一个系统服务。调用 NotificationManager 的 notify() 方法可以向系统发送通知。
  - INotificationManager
- Androidx相关
  - NotificationCompat
  - NotificationCompatBuilder
- 其他相关类
  - NotificationManagerService


#### 6.2 UML设计图


#### 6.3 关键流程图


#### 6.4 接口设计图


#### 6.5 模块间依赖关系


### 07.其他设计说明
#### 7.1 性能设计


#### 7.2 稳定性设计


#### 7.3 灰度设计


#### 7.4 降级设计


#### 7.5 异常设计


### 08.其他说明介绍
#### 8.1 参考链接










