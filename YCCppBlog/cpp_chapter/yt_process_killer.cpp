// //
// // Created by 杨充 on 2025/8/7.
// //
//
// #include "yt_process_killer.h"
// #include <iostream>
// #include <fstream>
// #include <cstdlib>
// #include <iostream>
// #include <cstdlib>  // 包含 system 函数
// #include <unistd.h> // 包含 fork 和 exec 系列函数
// #include <sstream>
// #include <cstdio>   // 包含 popen 和 pclose 函数
//
// // 使用 pkill 杀死进程
// bool ProcessKiller::killByPkill(const std::string& processName) {
//     std::string command = "pkill " + processName;
//     int result = system(command.c_str());
//     return (result == 0);
// }
//
// // 使用 killall 杀死进程
// bool ProcessKiller::killByKillall(const std::string& processName) {
//     std::string command = "killall " + processName;
//     int result = system(command.c_str());
//     return (result == 0);
// }
//
// // 使用 pidof + kill 杀死进程
// bool ProcessKiller::killByPidof(const std::string& processName) {
//     // 获取进程 ID
//     std::string pidCommand = "pidof " + processName;
//     FILE* pipe = popen(pidCommand.c_str(), "r");
//     if (!pipe) {
//         std::cerr << "无法执行 pidof 命令" << std::endl;
//         return false;
//     }
//
//     char buffer[128];
//     std::string result;
//     while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
//         result += buffer;
//     }
//     pclose(pipe);
//
//     // 如果找到进程 ID，杀死进程
//     if (!result.empty()) {
//         std::istringstream iss(result);
//         int pid;
//         iss >> pid;
//         std::string killCommand = "kill " + std::to_string(pid);
//         int status = system(killCommand.c_str());
//         return (status == 0);
//     } else {
//         std::cerr << "未找到进程: " << processName << std::endl;
//         return false;
//     }
// }
//
// // 使用 fork + exec 直接调用 pkill
// bool ProcessKiller::killByForkExec(const std::string& processName) {
//     pid_t pid = fork();
//     if (pid == 0) {  // 子进程
//         execlp("pkill", "pkill", processName.c_str(), nullptr);
//         // 如果 execlp 失败
//         std::cerr << "无法执行 pkill 命令" << std::endl;
//         _exit(1);
//     } else if (pid > 0) {  // 父进程
//         int status;
//         waitpid(pid, &status, 0);
//         return (status == 0);
//     } else {  // fork 失败
//         std::cerr << "fork 失败" << std::endl;
//         return false;
//     }
// }
