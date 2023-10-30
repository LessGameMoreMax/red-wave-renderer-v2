#ifndef THREAD_SAFE_IOSTREAM_H
#define THREAD_SAFE_IOSTREAM_H
#include <mutex>
#include <iostream>
#include <iomanip>
#include <stdarg.h>
namespace sablin{

static std::mutex kPrintLock;
inline void ThreadSafePrintf(const char* cmd, ...){
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;
    std::lock_guard<std::mutex> lock{kPrintLock};
    std::cout << "[" << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S.") << std::setfill('0') << std::setw(3) << ms << "] ";
    va_list args;
    va_start(args, cmd);
    vprintf(cmd, args);
    va_end(args);
    std::cout << "\n";
}

}
#endif
