#ifndef THREAD_H 
#define THREAD_H
#include <cstdint>
namespace sablin{

class ThisThread{
    friend class LinuxPlatformThread;
    friend class WindowsPlatformThread;
private:
    static thread_local int32_t thread_id_;
public:
    static int32_t GetThreadId(){
        return thread_id_;
    }

};

}
#endif
