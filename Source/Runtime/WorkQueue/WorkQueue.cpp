#include "WorkQueue.h"
namespace sablin{

WorkQueue::~WorkQueue(){
    ASSERT_WITH_STRING(work_queue_ == nullptr, "Not Call Exit Explicitly!")
}

void WorkQueue::Initialize(){
    work_queue_ = new ProducerConsumerQueue<lenin::_WorkRunnable*>();
    ASSERT_WITH_STRING(work_queue_ != nullptr, "Failed To Init work_queue_!")
    work_thread_pool_ = new WorkThreadPool();
    ASSERT_WITH_STRING(work_thread_pool_ != nullptr, "Failed To Init work_thread_pool_!")
}

void WorkQueue::Exit(bool should_wait){
    if(work_queue_ == nullptr) return;
    work_queue_->Stop(should_wait);

    delete work_thread_pool_;
    work_thread_pool_ = nullptr;

    delete work_queue_;
    work_queue_ = nullptr;
}



}
