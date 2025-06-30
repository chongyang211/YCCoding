cmake_minimum_required(VERSION 3.28)
project(YCCppBlog)

set(CMAKE_CXX_STANDARD 17)

# ld: symbol(s) not found for architecture arm64
# 设置目标架构为 arm64（适用于 Apple Silicon）
#set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE STRING "Target architectures")
# 适用于多架构（如同时支持 x86_64 和 arm64）
#set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "Target architectures")
if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch arm64")
endif()

add_executable(YCCppBlog main.cpp
        chapter10/day_10_1.cpp
        chapter10/day_10_1.cpp
        chapter10/day_10_2.cpp
        chapter10/day_10_2_4.cpp
        chapter10/day_10_3_1.cpp
        chapter10/day_10_3_2.cpp
        chapter10/day_10_3_3.cpp
        chapter10/day_10_3_4.cpp
        chapter10/day_10_3_4.cpp
        chapter8/day_8_2.cpp
        chapter8/day_8_3.cpp
        chapter8/day_8_4.cpp
        chapter8/day_8_7.cpp
        chapter8/day_8_5.cpp
        chapter8/day_8_9.cpp
        chapter8/day_8_8.cpp
        chapter8/day_8_6.cpp
        chapter6/day_6_7_1.cpp
        chapter6/day_6_7_2.cpp
        chapter10/day_10_4_1.cpp
        chapter10/day_10_4_5.cpp
        chapter10/day_10_4_4.cpp
        chapter10/day_10_4_3.cpp
        chapter10/day_10_4_2.cpp
        chapter10/day_10_4_8.cpp
        chapter14/day_14_1.cpp
        chapter16/day_16_1.cpp
        chapter16/day_16_1_3.cpp
        chapter16/day_16_1_4.cpp
        demo/student/StudentManager.cpp
        demo/student/StudentManager.h
        demo/worker/WorkerManager.cpp
        demo/worker/WorkerManager.h
        demo/worker/Worker.h
        demo/worker/Employee.cpp
        demo/worker/Employee.h
        demo/worker/Manager.cpp
        demo/worker/Manager.h
        demo/worker/Boss.cpp
        demo/worker/Boss.h
        demo/worker/WorkerTest.cpp
        demo/speaker/SpeechManager.cpp
        demo/speaker/SpeechManager.h
        demo/speaker/Speaker.h
        demo/school/ComputerManager.cpp
        demo/school/ComputerManager.h
        demo/school/Identity.h
        demo/school/Student.cpp
        demo/school/Student.h
        demo/school/Teacher.cpp
        demo/school/Teacher.h
        demo/school/Manager.cpp
        demo/school/Manager.h
)
