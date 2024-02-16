#include "RunnableThread.h"
namespace sablin{

RunnableThread::RunnableThread(Runnable* runnable, std::string thread_name, ThreadPriority thread_priority, ThreadType thread_type, uint32_t stack_size):
        thread_name_(thread_name),
        runnable_(runnable),
        thread_id_(-1),
        thread_priority_(thread_priority),
        thread_type_(thread_type),
        stack_size_(stack_size),
        joinable_(false){}

void RunnableThread::StopRunnable(){
    if(runnable_ != nullptr) runnable_->Stop();
}


}
