#include "RunnableThread.h"
namespace sablin{

RunnableThread::RunnableThread(Runnable* runnable, std::string thread_name, ThreadPriority thread_priority, uint32_t stack_size):
        thread_name_(thread_name),
        runnable_(runnable),
        thread_priority_(thread_priority),
        stack_size_(stack_size),
        thread_id_(-1),
        joinable_(false){
}


}
