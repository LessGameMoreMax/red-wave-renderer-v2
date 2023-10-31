#include "TaskGraphPipeline.h"
#include "TaskGraphPerf/TaskGraphPerf.h"
#include <iomanip>
namespace sablin{

TaskGraphPipeline::TaskGraphPipeline():
    is_init_(false){
    session_ = TaskGraphRandom<>::GenerateSession("pipeline");
    element_manager_ = new TaskGraphElementManager();
    param_manager_ = new TaskGraphParamManager();
    daemon_manager_ = new TaskGraphDaemonManager();
    event_manager_ = new TaskGraphEventManager();
}

TaskGraphPipeline::~TaskGraphPipeline(){
    if(element_manager_ != nullptr) delete element_manager_;
    if(param_manager_ != nullptr) delete param_manager_;
    if(daemon_manager_ != nullptr) delete daemon_manager_;
    if(event_manager_ != nullptr) delete event_manager_;
}

RStatus TaskGraphPipeline::InitEnv(){
    ASSERT_NO_STRING(event_manager_ != nullptr)
    ASSERT_NO_STRING(element_manager_ != nullptr)
    RStatus status = schedule_.Setup();
    if(!status.IsOk()) return RStatus("TaskGraphPipeline::InitEnv", STRING_CODE_LOCATION);
    GraphTaskThreadPool* thread_pool = schedule_.GetThreadPool();
    event_manager_->SetThreadPool(thread_pool);
    element_manager_->SetThreadPool(thread_pool);
    repository_.SetThreadPool(thread_pool);
    status += repository_.PreSetup();
    return status;
}

RStatus TaskGraphPipeline::Init(){
    ASSERT_NO_STRING(is_init_ == false)
    ASSERT_NO_STRING(element_manager_ != nullptr)
    ASSERT_NO_STRING(param_manager_ != nullptr)
    ASSERT_NO_STRING(daemon_manager_ != nullptr)
    ASSERT_NO_STRING(event_manager_ != nullptr)
    RStatus status = InitEnv();
    if(!status.IsOk()) return RStatus("TaskGraphPipeline::Init", STRING_CODE_LOCATION);

    status += param_manager_->PreSetup();
    status += event_manager_->Setup();
    status += element_manager_->Init();
    status += daemon_manager_->Setup();
    if(!status.IsOk()) return RStatus("TaskGraphPipeline::Init", STRING_CODE_LOCATION);
    
    is_init_ = true;
    return status;
}

RStatus TaskGraphPipeline::Run(){
    ASSERT_NO_STRING(is_init_ == true)
    ASSERT_NO_STRING(element_manager_ != nullptr)
    ASSERT_NO_STRING(param_manager_ != nullptr)
    RStatus status;
    status += repository_.Setup();
    status += param_manager_->Setup();
    if(!status.IsOk()) return RStatus("TaskGraphPipeline::Run", STRING_CODE_LOCATION);

    status = element_manager_->Run();
    status = repository_.Reset();
    param_manager_->ResetWithStatus(status);
    return status;
}

RStatus TaskGraphPipeline::Destroy(){
    ASSERT_NO_STRING(is_init_ == true)
    ASSERT_NO_STRING(element_manager_ != nullptr)
    ASSERT_NO_STRING(param_manager_ != nullptr)
    ASSERT_NO_STRING(daemon_manager_ != nullptr)
    ASSERT_NO_STRING(event_manager_ != nullptr)
    RStatus status;
    status += event_manager_->Exit();
    status += daemon_manager_->Exit();
    status += element_manager_->Destroy();
    status += param_manager_->Exit();
    if(!status.IsOk()) return RStatus("TaskGraphPipeline::Destroy", STRING_CODE_LOCATION);
    status += repository_.Exit();
    status += schedule_.Exit();
    if(!status.IsOk()) return RStatus("TaskGraphPipeline::Destroy", STRING_CODE_LOCATION);
    is_init_ = false;
    return status;
}

RStatus TaskGraphPipeline::Process(size_t run_times){
    RStatus status = Init();
    if(!status.IsOk()) return RStatus("TaskGraphPipeline::Process", STRING_CODE_LOCATION);
    while(run_times-- > 0 && !status.IsError() && !repository_.IsCancelState())
        status += Run();
    status += Destroy();
    return status;
}

AsyncFuture<RStatus> TaskGraphPipeline::AsyncRun(){
    ASSERT_NO_STRING(is_init_ == true)
    return AsyncLanuchAsync([this]{
            return Run();
    });
}

AsyncFuture<RStatus> TaskGraphPipeline::AsyncProcess(size_t run_times){
    ASSERT_NO_STRING(is_init_ == false)
    return AsyncLanuchAsync([run_times, this]{
        return Process(run_times);
    });
}

RStatus TaskGraphPipeline::Cancel(){
    ASSERT_NO_STRING(is_init_ == true)
    return repository_.PullAllState(TaskGraphElementState::kCancel);
}

RStatus TaskGraphPipeline::Yield(){
    ASSERT_NO_STRING(is_init_ == true)
    return repository_.PullAllState(TaskGraphElementState::kYield);
}

RStatus TaskGraphPipeline::Resume(){
    ASSERT_NO_STRING(is_init_ == true)
    return repository_.PullAllState(TaskGraphElementState::kNormal);
}

RStatus TaskGraphPipeline::Dump(std::ostream& oss){
    ASSERT_NO_STRING(element_manager_ != nullptr)
    oss << std::fixed << std::setprecision(2);
    oss << "\ndigraph TaskGraph {\n";
    oss << "compound=true;\n";

    for(const auto& element: element_manager_->manager_elements_){
        ASSERT_NO_STRING(element != nullptr)
        element->Dump(oss);
    }
    oss << "}\n\n";
    return RStatus();
}

RStatus TaskGraphPipeline::Perf(std::ostream& oss){
    ASSERT_NO_STRING(is_init_ == false)
    return TaskGraphPerf::Perf(this);
}

TaskGraphPipeline* TaskGraphPipeline::SetEngineType(TaskGraphEngineType type){
    ASSERT_NO_STRING(is_init_ == false)
    ASSERT_NO_STRING(element_manager_ != nullptr)
    element_manager_->SetEngineType(type);
    return this;
}

TaskGraphPipeline* TaskGraphPipeline::SetUniqueThreadPoolConfig(const GraphTaskThreadPoolConfig& config){
    ASSERT_NO_STRING(is_init_ == false)
    ASSERT_NO_STRING(config.GraphTaskThreadPoolConfigCheck().IsOk())
    schedule_.pool_config_ = config;
    return this;
}

TaskGraphPipeline* TaskGraphPipeline::SetSharedThreadPool(GraphTaskThreadPool* ptr){
    ASSERT_NO_STRING(is_init_ == false)
    ASSERT_NO_STRING(schedule_.MakeType(ptr).IsOk())
    return this;
}

TaskGraphPipeline* TaskGraphPipeline::SetAutoCheck(bool enable){
    ASSERT_NO_STRING(is_init_ == false)
    ASSERT_NO_STRING(element_manager_ != nullptr)
    element_manager_->auto_check_enable_ = enable;
    return this;
}

RStatus TaskGraphPipeline::CalcMaxPara(size_t& size){
    ASSERT_NO_STRING(element_manager_ != nullptr)
    return element_manager_->CalcMaxParaSize(size);
}

RStatus TaskGraphPipeline::MakeSerial(){
    ASSERT_NO_STRING(is_init_ == false)
    ASSERT_NO_STRING(element_manager_ != nullptr)
    if(schedule_.schedule_type_ != TaskGraphScheduleType::kTaskGraphScheduleTypeUnique){
        return RStatus("Can Not Set Serial Config Without Unique Schedule!", STRING_CODE_LOCATION);
    }
    if(!element_manager_->CheckSerializable()){
        return RStatus("Can Not Set Serial Config For This Pipeline!", STRING_CODE_LOCATION);
    }
    GraphTaskThreadPoolConfig config;
    config.primary_thread_size_ = 0;
    config.secondary_thread_size_ = 0;
    config.max_thread_size_ = 0;
    config.monitor_enable_ = false;
    config.batch_task_enable_ = false;
    schedule_.pool_config_ = config;
    return RStatus();
}

TaskGraphPipelineState TaskGraphPipeline::GetCurState() const{
    return repository_.cur_state_;
}



}
