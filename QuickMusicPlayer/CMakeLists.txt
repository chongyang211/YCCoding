# 指定 CMake 的最低版本为 3.16。
cmake_minimum_required(VERSION 3.16)

# 定义项目名称为 QuickMusicPlayer，版本为 0.1，并指定使用 C++ 语言。
project(QuickMusicPlayer VERSION 0.1 LANGUAGES CXX)

# 强制要求使用指定的 C++ 标准（默认是 C++17）。
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt 6 库，并确保 Quick 模块可用。
find_package(Qt6 REQUIRED COMPONENTS Quick)

# 设置 Qt 项目的标准配置，要求 Qt 6.8 版本。
qt_standard_project_setup(REQUIRES 6.8)

# 添加一个名为 appQuickMusicPlayer 的可执行文件，源文件为 App.cpp。
qt_add_executable(appQuickMusicPlayer
    App.cpp
)

# 将 App.qml 和 images.qrc 添加到 QML 模块中，模块的 URI 为 QuickMusicPlayer，版本为 1.0。
qt_add_qml_module(appQuickMusicPlayer
    URI QuickMusicPlayer
    VERSION 1.0
    QML_FILES
    App.qml
    RESOURCES
        images.qrc
    RESOURCES
        fonts.qrc
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appQuickMusicPlayer PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appQuickMusicPlayer
    # MACOSX_BUNDLE_BUNDLE_VERSION：设置应用程序的版本号。
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    # MACOSX_BUNDLE_SHORT_VERSION_STRING：设置应用程序的短版本号。
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    # MACOSX_BUNDLE：将应用程序打包为 macOS 的 .app 包。
    MACOSX_BUNDLE TRUE
    # WIN32_EXECUTABLE：将应用程序标记为 Windows 可执行文件。
    WIN32_EXECUTABLE TRUE
)

# 将 Qt6::Quick 模块链接到 appQuickMusicPlayer 可执行文件。
target_link_libraries(appQuickMusicPlayer
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appQuickMusicPlayer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
