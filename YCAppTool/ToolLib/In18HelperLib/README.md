# 国际化通用组件库
#### 目录介绍
- 01.基础概念介绍
- 02.常见思路和做法
- 03.切换语言Api
- 04.获取货币符号Api
- 05.国际化其他Api
- 06.原理和遇到的坑
- 07.其他问题说明



### 01.基础概念介绍



### 02.常见思路和做法




### 03.Api调用说明
#### 3.1 项目初始化api
- 在 Application 中初始化框架
    ``` java
    public final class XxxApplication extends Application {
        @Override
        public void onCreate() {
            super.onCreate();
            // 初始化语种切换框架
            LocaleService.getInstance().init(this.getApplication());
        }
    }
    ```
- 重写 Application 的 attachBaseContext 方法
    ``` java
    @Override
    protected void attachBaseContext(Context base) {
        // 绑定语种
        super.attachBaseContext(LocaleService.getInstance().attachBaseContext(base));
    }
    ``` java
- 重写基类 BaseActivity 的 attachBaseContext 方法
    ``` java
    @Override
    protected void attachBaseContext(Context newBase) {
        // 绑定语种
        super.attachBaseContext(LocaleService.getInstance().attachBaseContext(newBase));
    }
    ```



#### 3.2 如何更新设置语言
- 语种设置，api如下所示：
    ``` java
    // 设置当前的语种（返回 true 表示需要重启 App）
    LocaleService.getInstance().setAppLanguage(this, new Locale("ch"));
    // 获取当前的语种
    Locale currentLocale = LocaleService.getInstance().getCurrentLocale();
    // 跟随系统语种（返回 true 表示需要重启 App）
    LocaleService.getInstance().clearAppLanguage(this);
    ```



#### 3.3 如何监听app语言更换
- 监听app语言更换，api如下所示：
    ``` java
    LocaleService.getInstance().addOnLocaleChangedListener(new OnLocaleChangedListener() {
        @Override
        public void onAppLocaleChange(Locale oldLocale, Locale newLocale) {
            Log.d("MultiLanguages", "监听到应用切换了语种，旧语种：" + oldLocale + "，新语种：" + newLocale);
        }
  
        @Override
        public void onSystemLocaleChange(Locale oldLocale, Locale newLocale) {
            boolean systemLanguage = LocaleSpUtils.isSystemLanguage(LocaleActivity.this);
            Log.d("MultiLanguages", "监听到系统切换了语种，旧语种：" + oldLocale + "，新语种：" + newLocale + "，是否跟随系统：" + systemLanguage);
        }
    });
    ```


#### 3.4 一些其他API说明
- 其他api如下所示：
    ``` java
    // 获取当前 app 使用的 lang
    Locale currentLang = LocaleService.getInstance().getCurrentLang();
    // 获取当前 app 使用的 lang tag
    String currentLocaleTag = LocaleService.getInstance().getCurrentLocaleTag();
    // 获取支持的语言列表
    List<Locale> supportLocaleList = LocaleService.getInstance().getSupportLocaleList();
    // 获取当前 系统 使用的 locale
    Locale systemLocale = LocaleService.getInstance().getSystemLocale();
    // 判断是否是系统使用的locale
    boolean isSystemLocale = LocaleService.getInstance().isSystemLocale();
    ```



### 04.获取货币符号Api
- 获取货币符号API如下所示
    ``` java
    //获取当前货币符号【可以跟随不同项目配置自由变化】
    String symbol1 = SymbolHelper.getInstance().getSymbol();
    //获取印尼货币符号
    String symbol2 = SymbolHelper.getInstance().getSymbol(CountryCode.BR);
    ```




### 05.其他问题说明





