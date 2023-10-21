#ifndef TASK_GRAPH_TIMER_H
#define TASK_GRAPH_TIMER_H
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <future>
#include "../Math/MathTools.h"
namespace sablin{

class TaskGraphTimer{
private:
    std::atomic<bool> is_stop_;
    std::mutex lock_;
    std::condition_variable cond_var_;
    std::future<void> future_;
    long origin_interval_;
    long left_interval;
public:
    explicit TaskGraphTimer():
        is_stop_(true),
        origin_interval_(0),
        left_interval(0){}

    ~TaskGraphTimer(){
        Stop();
    }

    void Stop(){
        if(is_stop_.exchange(true)) return;
        cond_var_.notify_one();
        future_.wait();
    }

    template <typename Func>
    void Start(long interval, const Func& task){
        // if(!is_stop_.exchange(false)) return;
        // origin_interval_ = left_interval = interval;
        // future_ = std::async(std::launch::async, [this, task](){
        //     while(!is_stop_){
        //         std::unique_lock<std::mutex> ul(lock_);
        //         auto result = cond_var_.wait_for(ul, std::chrono::milliseconds(left_interval));
        //         if(result == std::cv_status::timeout && !is_stop_){
        //             long start = GetCurrentMs();
        //             task();
        //             long span = GetCurrentMs() - start;
        //             left_interval = (origin_interval_ > span)?
        //                 origin_interval_ - span: origin_interval_;
        //         }
        //     }
        // });
    }

};

}
#endif
