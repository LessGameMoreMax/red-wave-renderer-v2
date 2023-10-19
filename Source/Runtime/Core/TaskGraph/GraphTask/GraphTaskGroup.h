#ifndef GRAPH_TASK_GROUP_H
#define GRAPH_TASK_GROUP_H
#include "../../HAL/Runnable.h"
#include <vector>
#include <functional>
#include "../../Misc/MacroTools.h"
#include "../GraphTaskThreadPoolDefine.h"
#include "../../Misc/MacroTools.h"
namespace sablin{

class GraphTaskGroup: public Runnable{
    friend class GraphTaskThreadPool;
private:
    std::vector<std::function<void()>> task_vector_;
    long ttl_;
    std::function<void(RStatus)> on_finished_;
public:
    explicit GraphTaskGroup(): ttl_(kMaxBlockTTL), on_finished_(nullptr){}
    explicit GraphTaskGroup(const std::function<void()>& task,
            long ttl = kMaxBlockTTL, 
            const std::function<void(RStatus)>& on_finished = nullptr) noexcept{
        AddTask(task);
        SetTTL(ttl);
        SetOnFinished(on_finished);
    }
    CLASS_NO_ALLOWED_COPY(GraphTaskGroup)

    GraphTaskGroup* AddTask(const std::function<void()>& task){
        task_vector_.emplace_back(task);
        return this;
    }

    GraphTaskGroup* SetTTL(long ttl){
        ttl_ = ttl;
        return this;
    }

    GraphTaskGroup* SetOnFinished(const std::function<void(RStatus)>& on_finished){
        on_finished_ = on_finished;
        return this;
    }

    inline constexpr long GetTTL() const{
        return ttl_;
    }

    inline void Clear(){
        task_vector_.clear();
    }

    inline constexpr size_t GetSize() const{
        return task_vector_.size();
    }

    virtual RStatus Run() override{
        return RStatus("No Implement!", STRING_CODE_LOCATION);
    }
};

}
#endif
