#include "WorkQueue.h"
namespace sablin{

void WorkQueue::Initialize(){
    work_thread_pool_ = new WorkThreadPool();
    ASSERT_WITH_STRING(work_thread_pool_ != nullptr, "Failed To Init WorkThreadPool!")
}

void WorkQueue::Exit(){
    delete work_thread_pool_;
    work_thread_pool_ = nullptr;
}



}
