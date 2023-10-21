#include "TaskGraphEvent.h"
namespace sablin{

RStatus TaskGraphEvent::Process(TaskGraphEventType event_type){
    switch(event_type){
        case TaskGraphEventType::kTaskGraphEventTypeSync:
            Trigger(event_param_ptr_);
            break;
        case TaskGraphEventType::kTaskGraphEventTypeAsync:
            ASSERT_NO_STRING(thread_pool_ != nullptr)
            thread_pool_->Commit([this]{this->Trigger(this->event_param_ptr_); },
                    kPoolGraphTaskStrategy);
            break;
        default:
            return RStatus("Unknown Event Type");
    }
    return RStatus();
}

}
