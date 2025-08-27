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

#如果你的项目使用 CMake，确保源文件被添加到 add_executable 或 add_library 中
add_executable(YCCppBlog main.cpp
        cpp_chapter/chapter10/day_10_1.cpp
        cpp_chapter/chapter10/day_10_1.cpp
        cpp_chapter/chapter10/day_10_2.cpp
        cpp_chapter/chapter10/day_10_2_4.cpp
        cpp_chapter/chapter10/day_10_3_1.cpp
        cpp_chapter/chapter10/day_10_3_2.cpp
        cpp_chapter/chapter10/day_10_3_3.cpp
        cpp_chapter/chapter10/day_10_3_4.cpp
        cpp_chapter/chapter10/day_10_3_4.cpp
        cpp_chapter/chapter8/day_8_2.cpp
        cpp_chapter/chapter8/day_8_3.cpp
        cpp_chapter/chapter8/day_8_4.cpp
        cpp_chapter/chapter8/day_8_7.cpp
        cpp_chapter/chapter8/day_8_5.cpp
        cpp_chapter/chapter8/day_8_9.cpp
        cpp_chapter/chapter8/day_8_8.cpp
        cpp_chapter/chapter8/day_8_6.cpp
        cpp_chapter/chapter6/day_6_7_1.cpp
        cpp_chapter/chapter6/day_6_7_2.cpp
        cpp_chapter/chapter10/day_10_4_1.cpp
        cpp_chapter/chapter10/day_10_4_5.cpp
        cpp_chapter/chapter10/day_10_4_4.cpp
        cpp_chapter/chapter10/day_10_4_3.cpp
        cpp_chapter/chapter10/day_10_4_2.cpp
        cpp_chapter/chapter10/day_10_4_8.cpp
        cpp_chapter/chapter14/day_14_1.cpp
        cpp_chapter/chapter16/day_16_1.cpp
        cpp_chapter/chapter16/day_16_1_3.cpp
        cpp_chapter/chapter16/day_16_1_4.cpp
        cpp_demo/student/StudentManager.cpp
        cpp_demo/student/StudentManager.h
        cpp_demo/worker/WorkerManager.cpp
        cpp_demo/worker/WorkerManager.h
        cpp_demo/worker/Worker.h
        cpp_demo/worker/Employee.cpp
        cpp_demo/worker/Employee.h
        cpp_demo/worker/Manager.cpp
        cpp_demo/worker/Manager.h
        cpp_demo/worker/Boss.cpp
        cpp_demo/worker/Boss.h
        cpp_demo/speaker/SpeechManager.cpp
        cpp_demo/speaker/SpeechManager.h
        cpp_demo/speaker/Speaker.h
        cpp_demo/school/ComputerManager.cpp
        cpp_demo/school/ComputerManager.h
        cpp_demo/school/Identity.h
        cpp_demo/school/Student.cpp
        cpp_demo/school/Student.h
        cpp_demo/school/Teacher.cpp
        cpp_demo/school/Teacher.h
        cpp_demo/school/Manager.cpp
        cpp_demo/school/Manager.h
        cpp_demo/school/GlobalFile.h
        thirdlib/json/json11.h
        thirdlib/json/json11.cpp
        thirdlib/json/jsonTest.cpp
        c_chapter/chapter7/day7_7_8.c
        c_demo/student/student_manager.h
        c_demo/student/student_manager.c
        c_demo/bank/BankManager.h
        c_demo/bank/BankManager.c
        c_demo/worker/WorkManager.h
        c_demo/worker/WorkManager.c
        c_demo/worker/DemoWorker.c
        c_demo/worker/Employee.h
        c_demo/worker/WorkerTools.h
        c_demo/worker/WorkerLogger.h
        c_demo/worker/WorkerError.h
        c_demo/worker/WorkerLogger.c
        c_demo/worker/WorkerValid.h
        c_demo/worker/WorkerValid.c
        c_demo/worker/Employee.c
        c_demo/calculator/calculator.h
        c_demo/calculator/parser.h
        c_demo/calculator/stack.h
        c_demo/calculator/error_handler.h
        c_demo/calculator/history.h
        c_demo/calculator/constants.h
        c_demo/calculator/calculator.c
        c_demo/calculator/parser.c
        c_demo/calculator/stack.c
        c_demo/calculator/error_handler.c
        c_demo/calculator/history.c
        c_demo/calculator/constants.c
        c_demo/calculator/calculator_main.c
        c_demo/speaker/complex/speech_main.c
        c_demo/speaker/easy/speech_worker.c
        c_demo/computer/computer_main.cpp
        c_demo/computer/identity.h
        c_demo/computer/file_operation.h
        c_demo/computer/user.h
        c_demo/sql/sql_manager.c
        cpp_demo/bank/BankUserManager.cpp
        cpp_demo/bank/BankUserManager.h
        cpp_demo/bank/Account.cpp
        cpp_demo/bank/Account.h
        cpp_demo/bank/Bank.cpp
        cpp_demo/bank/Bank.h
        cpp_demo/bank/FileManager.cpp
        cpp_demo/bank/FileManager.h
        c_demo/sql/sql_manager.h
        cpp_demo/sql/SqlManager.cpp
        cpp_demo/sql/SqlManager.h
        cpp_hal_worker/relay_strategy.cpp
        cpp_hal_worker/relay_strategy.h
        cpp_hal_worker/access_strategy.h
        cpp_hal_worker/strategy_utils.cpp
        cpp_hal_worker/strategy_utils.h
        cpp_hal_worker/wiegand_strategy.cpp
        cpp_hal_worker/wiegand_strategy.h
        cpp_demo/worker/Worker.cpp
        cpp_demo/speaker/FileSpeechUtils.cpp
        cpp_demo/speaker/FileSpeechUtils.h
        cpp_demo/school/ToolUtils.cpp
        cpp_demo/school/ToolUtils.h
        cpp_chapter/chapter13/day_13_1.cpp
        cpp_chapter/chapter13/day_13_3.cpp
        cpp_chapter/chapter13/day_13.h
        cpp_chapter/DemoTest.cpp
        cpp_chapter/DemoTest.h
        cpp_chapter/Demo.cpp
        cpp_chapter/yt_process_killer.cpp
        cpp_chapter/yt_process_killer.h
        cpp_demo/ticket/TicketEasyManager.cpp
        cpp_chapter/chapter13/day13_6.cpp
        cpp_demo/thread/ThreadPool.cpp
        cpp_demo/thread/ThreadPool.h
        cpp_demo/thread/ThreadPoolTest.cpp
)
