#ifndef WORK_THREAD_POOL_H
#define WORK_THREAD_POOL_H
#include <list>
#include <mutex>
#include "Core/Thread/RunnableThread.h"
#include "WorkThreadPoolDefine.h"
namespace sablin{

class WorkThreadPool{
private:
    uint32_t work_thread_min_num_;
    uint32_t work_thread_max_num_;
    std::mutex lock_;
    std::list<RunnableThread*> work_threads_;
private:
    void Setup();
    void Release(const int32_t thread_num);
    uint32_t Add(const int32_t thread_num);
public:
    explicit WorkThreadPool();
    CLASS_NO_ALLOWED_COPY(WorkThreadPool)
    ~WorkThreadPool();

    uint32_t EnlargeWorkThread(const int32_t thread_num);
    bool ReduceWorkThread(const int32_t thread_num);

    inline uint32_t GetWorkThreadMinNum() const{
        return work_thread_min_num_;
    }

    inline uint32_t GetWorkThreadMaxNum() const{
        return work_thread_max_num_;
    }

    inline uint32_t GetWorkThreadNum(){
        std::lock_guard<std::mutex> lk(lock_);
        return work_threads_.size();
    }
};

}
#endif
