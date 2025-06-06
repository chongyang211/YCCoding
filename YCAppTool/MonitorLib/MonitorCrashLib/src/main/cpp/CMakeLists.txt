
# 最低版本要求
cmake_minimum_required(VERSION 3.10.2)

project("crash_dumper")

# add_library：添加项目so库，就是你需要创建的目标库声明处
add_library(
        # crash_dumper 就是指定库的名称
        crash_dumper
        # SHARED就是指定生成库的类型：动态|静态；
        SHARED
        # 需要参与编译的src文件，如果有多个，则使用空格或者换行包括进来
        # 这个是程序入口
        crash_dumper.cpp
        stack_tracer.cpp
        readelf.cpp
)


# find_library：查找第三方库，这个在某个库需要关联其他库进行连接的时候就需要查找到库
# 表示引入库，这里是指定了ndk里面的log库，并将库的路径赋值给了log-lib变量，相当于java中定义了一个变量log-lib，值是log。
find_library(
        log-lib
        log)

# target_link_libraries：将第三方库链接到目标库中，使用find_library找到第三方库之后，使用target_link_libraries某个库就可以在链接时找到第三方库。
target_link_libraries(
        crash_dumper
        ${log-lib})