#include "TaskGraphElement.h"
#include "../../HAL/ThisThread.h"
namespace sablin{

TaskGraphElement::TaskGraphElement():
    done_(false),
    linkable_(false),
    visible_(true),
    is_init_(false),
    timeout_strategy_(TaskGraphElementTimeoutStrategy::kAsError),
    element_type_(TaskGraphElementType::kElement),
    cur_state_(TaskGraphElementState::kCreate),
    loop_(kTaskGraphDefaultLoopTimes),
    level_(kTaskGraphDefaultElementLevel),
    binding_index_(kTaskGraphDefaultBindingIndex),
    timeout_(kTaskGraphDefaultElementTimeout),
    aspect_manager_(nullptr),
    thread_pool_(nullptr),
    perf_info_(nullptr),
    trigger_times_(0),
    left_depend_(0),
    belong_(nullptr){}

TaskGraphElement::~TaskGraphElement(){
    if(perf_info_ != nullptr)
        delete perf_info_;

    if(aspect_manager_ != nullptr)
        delete aspect_manager_;

    for(auto& param: local_params_)
        if(param.second != nullptr)
            delete param.second;
    local_params_.clear();
}


RStatus TaskGraphElement::BeforeRun(){
    done_ = false;
    left_depend_ = dependence_.size();
    return RStatus();
}

RStatus TaskGraphElement::AfterRun(){
    for(TaskGraphElement* element: run_before_)
        --element->left_depend_;
    done_ = true;
    return RStatus();
}

RStatus TaskGraphElement::DoAspect(const TaskGraphAspectType& aspect_type, const RStatus& cur_status){
    RStatus status;
    if(aspect_manager_ != nullptr && aspect_manager_->GetSize() != 0)
        status = aspect_manager_->Reflect(aspect_type, cur_status);
    return status;
}

RStatus TaskGraphElement::SetElementInfo(const std::set<TaskGraphElement*>& depend_elements,
        const std::string& name, size_t loop, TaskGraphParamManager* param_manager,
        TaskGraphEventManager* event_manager){
    ASSERT_NO_STRING(is_init_ == false)
    RStatus status;
    SetName(name)->SetLoop(loop);
    param_manager_ = param_manager;
    event_manager_ = event_manager;
    status = AddDependTaskGraphElements(depend_elements);
    return status;
}

RStatus TaskGraphElement::FatProcessor(const TaskGraphFunctionType& function_type){
    if(!visible_)[[unlikely]] return RStatus();
    RStatus status;
    try{
        switch (function_type){
            case TaskGraphFunctionType::kRun:{
                if(trigger_times_ == 0){
                    status = PrepareRun();
                    ASSERT_NO_STRING(status.IsOk())
                }
                ++trigger_times_;
                for(size_t i=0;i<loop_&&cur_state_.load()==TaskGraphElementState::kNormal;++i){
                    status = DoAspect(TaskGraphAspectType::kBeginRun);
                    ASSERT_NO_STRING(status.IsOk())
                    do{
                        status = IsAsync()? AsyncRun(): Run();
                    }while(CheckYield(), status.IsOk() && IsHold());
                    DoAspect(TaskGraphAspectType::kFinishRun, status);
                }
                break;
            }
            case TaskGraphFunctionType::kInit:{
                status = DoAspect(TaskGraphAspectType::kBeginInit);
                ASSERT_NO_STRING(status.IsOk())
                status = Init();
                DoAspect(TaskGraphAspectType::kFinishInit, status);
                break;
            }
            case TaskGraphFunctionType::kDestroy:{
                status = DoAspect(TaskGraphAspectType::kBeginDestroy);
                ASSERT_NO_STRING(status.IsOk())
                status = Destroy();
                DoAspect(TaskGraphAspectType::kFinishDestroy, status);
                break;
            }
            default:
                return RStatus("Get Function Type Error!", STRING_CODE_LOCATION);
        }
    }catch (std::exception&){
        DoAspect(TaskGraphAspectType::kEnterCrashed);
        status = Crashed("FatProcessor Exception!");
    }
    return status;
}

TaskGraphElement* TaskGraphElement::SetThreadPool(GraphTaskThreadPool* thread_pool){
    ASSERT_NO_STRING(thread_pool != nullptr)
    ASSERT_NO_STRING(is_init_ == false)
    thread_pool_ = thread_pool;
    return this;
}

void TaskGraphElement::Dump(std::ostream& oss){
    DumpElement(oss);
    for(const auto& node: run_before_)
        DumpEdge(oss, this, node);
}

void TaskGraphElement::DumpEdge(std::ostream& oss, TaskGraphElement* src, TaskGraphElement* dst,
         const std::string& label){
    if(src->IsGroup() && dst->IsGroup()){
        oss << 'p' << src << " -> p" << dst << label << "[ltail=cluster_p" << src << "lhead=cluster_p" << dst << "];\n";
    }else if(src->IsGroup() && !dst->IsGroup()){
        oss << 'p' << src << " -> p" << dst << label << "[ltail=cluster_p" << src << "];\n";
    }else if(!src->IsGroup() && dst->IsGroup()){
        oss << 'p' << src << " -> p" << dst << label << "[lhead=cluster_p" << dst << "];\n";
    }else{
        oss << 'p' << src << " -> p" << dst << label << ";\n";
    }
}

void TaskGraphElement::DumpElement(std::ostream& oss){
    DumpElementHeader(oss);
    DumpPerfInfo(oss);

    oss << "\"];\n";
    if(loop_ > 1 && !IsGroup()){
        oss << 'p' << this << " -> p" << this << "[label=\"" << loop_ << "\"]" << ";\n";
    }
}

void TaskGraphElement::DumpElementHeader(std::ostream& oss){
    oss << 'p' << this << "[label=\"";
    if(name_.empty()){
        oss << 'p' << this;
    }else{
        oss << name_;
    }
}

void TaskGraphElement::DumpPerfInfo(std::ostream& oss){
    if(perf_info_ && perf_info_->loop_ > 0){
        oss << "\n";
        oss << "[start " << perf_info_->first_start_ts_;
        oss << "ms, finish " << perf_info_->last_finish_ts_ << "ms,\n";
        oss << "per_cost " << (perf_info_->accu_cost_ts_ / perf_info_->loop_);
        if(perf_info_->loop_ > 1){
            oss << "ms, total_cost " << perf_info_->accu_cost_ts_;
        }
        oss << "ms]";
    }
}

bool TaskGraphElement::IsSerializable(){
    return true;
}

RStatus TaskGraphElement::PopLastAspect(){
    ASSERT_NO_STRING(is_init_ == false)
    ASSERT_NO_STRING(aspect_manager_ != nullptr)
    RStatus status = aspect_manager_->PopLast();
    ASSERT_NO_STRING(status.IsOk())
    if(aspect_manager_->GetSize() == 0){
        delete aspect_manager_;
        aspect_manager_ = nullptr;
    }
    return status;
}

RStatus TaskGraphElement::AsyncRun(){
    if(!IsAsync()) 
        return RStatus("[" + name_ + "] Cannot Async Run!", STRING_CODE_LOCATION);
    async_result_ = thread_pool_->Commit([this]{
        return Run();
    }, kPoolGraphTaskStrategy);

    auto fut_status = async_result_.wait_for(std::chrono::milliseconds(timeout_));
    RStatus status;
    if(fut_status == std::future_status::ready){
        status = GetAsyncResult();
    }else{
        if(timeout_strategy_ == TaskGraphElementTimeoutStrategy::kAsError){
            return RStatus("["+name_+"] Running Time More Than ["+std::to_string(timeout_)+"]ms",
                    STRING_CODE_LOCATION);
        }
        cur_state_.store(TaskGraphElementState::kTimeout);
    }
    return status;
}

RStatus TaskGraphElement::GetAsyncResult(){
    RStatus status;
    if(async_result_.valid()){
        status = async_result_.get();
    }
    return status;
}

RStatus TaskGraphElement::CheckSuitable(){
    ASSERT_NO_STRING(thread_pool_ != nullptr)
    if(loop_ > kTaskGraphDefaultLoopTimes && IsAsync()){
        return RStatus("["+name_+"] Can Set Loop <= 1 Only For The Reason Of Async Run!",
                STRING_CODE_LOCATION);
    }
    return RStatus();
}

RStatus TaskGraphElement::PrepareRun(){
    done_ = false;
    left_depend_ = dependence_.size();
    return RStatus();
}

bool TaskGraphElement::IsHold(){
    return false;
}

bool TaskGraphElement::IsMatch(){
    return false;
}

bool TaskGraphElement::IsTimeout() const{
    bool result = (cur_state_.load() == TaskGraphElementState::kTimeout);
    TaskGraphElement* belong = belong_;
    while(!result && belong){
        result = (belong->cur_state_.load() == TaskGraphElementState::kTimeout);
        belong = belong->belong_;
    }
    return result;
}

RStatus TaskGraphElement::Crashed(const std::string& str){
    return RStatus(RSTATUS_CRASH, str, STRING_CODE_LOCATION);
}

int32_t TaskGraphElement::GetThreadIndex(){
    ASSERT_NO_STRING(thread_pool_ != nullptr)
    return thread_pool_->GetThreadPoolId(ThisThread::GetThreadId());
}

int32_t TaskGraphElement::GetBindingIndex(){
    return binding_index_;
}

RStatus TaskGraphElement::BuildRelation(TaskGraphElementRelation& relation){
    relation.predecessors_ = dependence_;
    relation.successors_ = run_before_;
    relation.belong_ = belong_;
    return RStatus();
}

RStatus TaskGraphElement::AddDependTaskGraphElements(const std::set<TaskGraphElement*>& elements){
    ASSERT_NO_STRING(is_init_ == false)
    for(TaskGraphElement* cur: elements){
        ASSERT_NO_STRING(cur != nullptr)
        if(cur->belong_ != belong_)
            return RStatus(cur->GetName() + "Can Not Depend Because Not Same Belong Info!",
                    STRING_CODE_LOCATION);
        if(this == cur) continue;
        cur->run_before_.insert(this);
        dependence_.insert(cur);
    }
    left_depend_ = dependence_.size();
    return RStatus();
}

TaskGraphElement* TaskGraphElement::SetName(const std::string& name){
    ASSERT_NO_STRING(is_init_ == false)
    name_ = name.empty() ? session_: name;
    if(aspect_manager_ != nullptr){
        aspect_manager_->SetName(name_);
    }
    return this;
}

TaskGraphElement* TaskGraphElement::SetLoop(size_t loop){
    ASSERT_NO_STRING(is_init_ == false)
    if(timeout_ > kTaskGraphDefaultElementTimeout && loop != kTaskGraphDefaultLoopTimes){
        ASSERT_WITH_STRING(false, "Can Not Set Loop Value When Timeout Is Bigger Than 0!")
    }
    loop_ = loop;
    return this;
}

TaskGraphElement* TaskGraphElement::SetLevel(int32_t level){
    ASSERT_NO_STRING(is_init_ == false)
    level_ = level;
    return this;
}

TaskGraphElement* TaskGraphElement::SetVisible(bool visible){
    ASSERT_NO_STRING(is_init_ == false)
    visible_ = visible;
    return this;
}

TaskGraphElement* TaskGraphElement::SetBindingIndex(int32_t index){
    ASSERT_NO_STRING(is_init_ == false)
    binding_index_ = index;
    return this;
}

TaskGraphElement* TaskGraphElement::SetTimeout(long timeout, 
        TaskGraphElementTimeoutStrategy strategy){
    ASSERT_NO_STRING(is_init_ == false)
    if(timeout < kTaskGraphDefaultElementTimeout){
        ASSERT_WITH_STRING(false, "Timeout Value Can Not Smaller Than Zero!")
    }
    if(loop_ > kTaskGraphDefaultLoopTimes && timeout != kTaskGraphDefaultElementTimeout){
        ASSERT_WITH_STRING(false, "Can Not Set Timeout Value When Loop Bigger Than 1!")
    }
    timeout_ = timeout;
    timeout_strategy_ = strategy;
    return this;
}

bool TaskGraphElement::IsGroup() const{
    return (long(element_type_) & long(TaskGraphElementType::kGroup)) > 0;
}

}
