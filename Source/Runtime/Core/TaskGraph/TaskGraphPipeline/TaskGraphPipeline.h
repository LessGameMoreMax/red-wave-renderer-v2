#ifndef TASK_GRAPH_PIPELINE_H
#define TASK_GRAPH_PIPELINE_H
#include "../../HAL/Runnable.h"
#include "../TaskGraphInterface/TaskGraphDescInfoInterface.h"
#include "../TaskGraphElement/TaskGraphElementManager.h"
#include "../TaskGraphParam/TaskGraphParamManager.h"
#include "../TaskGraphDaemon/TaskGraphDaemonManager.h"
#include "../TaskGraphEvent/TaskGraphEventManager.h"
#include "../TaskGraphElement/TaskGraphElementManager.h"
#include "TaskGraphSchedule/TaskGraphSchedule.h"
#include "../TaskGraphElement/TaskGraphElementRepository.h"
#include "../../Utility/Async.h"
#include "../../Utility/AsyncFuture.h"
#include "../TaskGraphParam/TaskGraphParamManagerWrapper.h"
#include "../TaskGraphElement/TaskGraphNode/TaskGraphNode.h"
#include "../TaskGraphElement/TaskGraphNode/TaskGraphNodeInfo.h"
#include "../TaskGraphElement/TaskGraphAdapter/TaskGraphAdapter.h"
#include "../TaskGraphElement/TaskGraphAdapter/TaskGraphFunction/TaskGraphFunction.h"
#include "../TaskGraphElement/TaskGraphAdapter/TaskGraphFence/TaskGraphFence.h"
#include "../TaskGraphElement/TaskGraphAdapter/TaskGraphCoordinator/TaskGraphCoordinator.h"
#include "../TaskGraphElement/TaskGraphNode/TaskGraphTemplateNode.h"
#include "../TaskGraphDaemon/TaskGraphTemplateDaemon.h"
namespace sablin{

class TaskGraphPipeline: public Runnable,
                         public TaskGraphDescInfoInterface{
    friend class TaskGraphPerf;
    friend class TaskGraphPipelineFactory;
private:
    bool is_init_;
    TaskGraphElementManager* element_manager_;
    TaskGraphParamManager* param_manager_;
    TaskGraphDaemonManager* daemon_manager_;
    TaskGraphEventManager* event_manager_;

    TaskGraphSchedule schedule_;
    TaskGraphElementRepository repository_;
protected:
    explicit TaskGraphPipeline();
    CLASS_NO_ALLOWED_COPY(TaskGraphPipeline)
    virtual ~TaskGraphPipeline();

    RStatus InitEnv();
public:
    virtual RStatus Init() override;
    virtual RStatus Run() override;
    virtual RStatus Destroy() override;

    RStatus Process(size_t run_times = kTaskGraphDefaultLoopTimes);
    AsyncFuture<RStatus> AsyncRun();
    AsyncFuture<RStatus> AsyncProcess(size_t run_times = kTaskGraphDefaultLoopTimes);

    RStatus Cancel();
    RStatus Yield();
    RStatus Resume();

    RStatus Dump(std::ostream& oss = std::cout);
    RStatus Perf(std::ostream& oss = std::cout);

    TaskGraphPipeline* SetEngineType(TaskGraphEngineType type);
    TaskGraphPipeline* SetUniqueThreadPoolConfig(const GraphTaskThreadPoolConfig& config);
    TaskGraphPipeline* SetSharedThreadPool(GraphTaskThreadPool* ptr);
    TaskGraphPipeline* SetAutoCheck(bool enable);

    RStatus CalcMaxPara(size_t& size);
    RStatus MakeSerial();
    TaskGraphPipelineState GetCurState() const;

    template<typename T, std::enable_if_t<std::is_base_of<TaskGraphNode, T>::value, int> = 0>
    TaskGraphNode* CreateTaskGraphNode(const TaskGraphNodeInfo& info){
        ASSERT_NO_STRING(is_init_ == false)
        TaskGraphNode* node = new T();
        RStatus status = node->SetElementInfo(info.dependence_, info.name_, info.loop_,
                    param_manager_, event_manager_);
        ASSERT_NO_STRING(status.IsOk())
        repository_.Insert(node);
        return node;
    }

    template<typename T, std::enable_if_t<std::is_base_of<TaskGraphGroup, T>::value, int> = 0>
    TaskGraphGroup* CreateTaskGraphGroup(const std::vector<TaskGraphElement*>& elements,
                const std::set<TaskGraphElement*>& depend_elements = 
                std::initializer_list<TaskGraphElement*>(), const std::string& name = STRING_NULL,
                size_t loop = kTaskGraphDefaultLoopTimes){
        ASSERT_NO_STRING(is_init_ == false)
        if(std::any_of(elements.begin(), elements.end(),
                    [](TaskGraphElement* element){return element == nullptr;})){
            std::cout << "CreateTaskGraphGroup Elements Have Nullptr!" << std::endl;
            ASSERT_NO_STRING(false)
        }

        if(std::any_of(depend_elements.begin(), depend_elements.end(),
                    [](TaskGraphElement* element){return element == nullptr;})){
            std::cout << "CreateTaskGraphGroup DependElements Have Nullptr!" << std::endl;
            ASSERT_NO_STRING(false)
        }

        RStatus status;
        TaskGraphGroup* group = new T();
        for(TaskGraphElement* element: elements){
            status += group->AddElement(element);
            element->belong_ = group;
        }
        ASSERT_NO_STRING(status.IsOk())
        status = group->SetElementInfo(depend_elements, name, loop, nullptr, nullptr);
        ASSERT_NO_STRING(status.IsOk())
        repository_.Insert(group);
        return group;
    }

    template<typename T, std::enable_if_t<std::is_base_of<TaskGraphElement, T>::value, int> = 0>
    RStatus RegisterTaskGraphElement(TaskGraphElement** element_ptr, 
            const std::set<TaskGraphElement*>&
            depend_elements = std::initializer_list<TaskGraphElement*>(),
            const std::string& name = STRING_NULL, size_t loop = kTaskGraphDefaultLoopTimes){
        ASSERT_NO_STRING(is_init_ == false)
        if(std::is_base_of<TaskGraphGroup, T>::value){
            if((*element_ptr) != nullptr && ((TaskGraphGroup*)(*element_ptr))->IsRegistered()){
                std::cout << "This Group Register Duplicate!" << std::endl;
                ASSERT_NO_STRING(false)
            }
        }else if(std::is_base_of<TaskGraphNode, T>::value || std::is_base_of<TaskGraphAdapter, T>::value){
            (*element_ptr) = new T();
        }

        ASSERT_NO_STRING((*element_ptr) != nullptr)
        RStatus status = (*element_ptr)->SetElementInfo(depend_elements, name, loop, param_manager_,
                event_manager_);
        ASSERT_NO_STRING(status.IsOk())
        status = element_manager_->Add(dynamic_cast<TaskGraphElement*>(*element_ptr));
        ASSERT_NO_STRING(status.IsOk())
        repository_.Insert(*element_ptr);
        return status;
    }

    RStatus RegisterTaskGraphElement(TaskGraphFunction** function_ptr,
            const std::set<TaskGraphElement*>&
            depend_elements = std::initializer_list<TaskGraphElement*>(),
            const std::string& name = STRING_NULL, size_t loop = kTaskGraphDefaultLoopTimes){
        return RegisterTaskGraphElement<TaskGraphFunction>((TaskGraphElement**)(function_ptr), depend_elements, name, loop);
    }

    RStatus RegisterTaskGraphElement(TaskGraphFence** fence_ptr,
            const std::set<TaskGraphElement*>&
            depend_elements = std::initializer_list<TaskGraphElement*>(),
            const std::string& name = STRING_NULL, size_t loop = kTaskGraphDefaultLoopTimes){
        return RegisterTaskGraphElement<TaskGraphFence>((TaskGraphElement**)(fence_ptr), depend_elements, name, loop);
    }

    template<int32_t SIZE>
    RStatus RegisterTaskGraphElement(TaskGraphCoordinator<SIZE>** coordinator_ptr,
            const std::set<TaskGraphElement*>&
            depend_elements = std::initializer_list<TaskGraphElement*>(),
            const std::string& name = STRING_NULL, size_t loop = kTaskGraphDefaultLoopTimes){
        return RegisterTaskGraphElement<TaskGraphCoordinator, SIZE>((TaskGraphElement**)(coordinator_ptr), depend_elements, name, loop);
    }

    template<typename TNode, typename... Args,
        std::enable_if_t<std::is_base_of<TaskGraphTemplateNode<Args...>, TNode>::value, int> = 0>
    RStatus RegisterTaskGraphElement(TaskGraphTemplateNode<Args...>** element_ptr,
            const std::set<TaskGraphElement*>&
            depend_elements = std::initializer_list<TaskGraphElement*>(),
            Args... args){
        (*element_ptr) = new TNode(std::forward<Args&&>(args)...);
        ASSERT_NO_STRING((*element_ptr) != nullptr)
        RStatus status = (*element_ptr)->SetElementInfo(depend_elements, STRING_NULL,
                kTaskGraphDefaultLoopTimes, param_manager_, event_manager_);
        ASSERT_NO_STRING(status.IsOk())
        status = element_manager_->Add(dynamic_cast<TaskGraphElement*>(*element_ptr));
        ASSERT_NO_STRING(status.IsOk())
        repository_.Insert(*element_ptr);
        return status;
    }

    template<typename TAspect, typename TParam = TaskGraphPassedDefaultParam,
        std::enable_if_t<std::is_base_of<TaskGraphAspect, TAspect>::value, int> = 0,
        std::enable_if_t<std::is_base_of<TaskGraphPassedParam, TParam>::value, int> = 0>
    TaskGraphPipeline* AddTaskGraphAspect(const std::set<TaskGraphElement*>& elements =
            std::initializer_list<TaskGraphElement*>(), TParam* param = nullptr){
        ASSERT_NO_STRING(is_init_ == false)
        const std::set<TaskGraphElement*> cur_elements = 
                elements.empty()? repository_.elements_: elements;
        for(TaskGraphElement* element: cur_elements){
            if(repository_.Find(element)){
                element->AddTaskGraphAspect<TAspect, TParam>(param);
            }
        }
        return this;
    }

    template<typename TDaemon, typename TParam = TaskGraphPassedDefaultParam,
        std::enable_if_t<std::is_base_of<TaskGraphDaemon, TDaemon>::value, int> = 0,
        std::enable_if_t<std::is_base_of<TaskGraphPassedParam, TParam>::value, int> = 0>
    TaskGraphPipeline* AddDaemon(long ms, TParam* param = nullptr){
        ASSERT_NO_STRING(is_init_ == false)
        ASSERT_NO_STRING(param_manager_ != nullptr)
        ASSERT_NO_STRING(daemon_manager_ != nullptr)
        TaskGraphDaemon* daemon = new TDaemon();
        daemon->SetDaemonParam<TParam>(param)->SetInterval(ms);
        daemon->SetTaskGraphParamManager(param_manager_);
        daemon->SetTaskGraphEventManager(event_manager_);
        RStatus status = daemon_manager_->Add(daemon);
        if(!status.IsOk()) return nullptr;
        return this;
    }

    template<typename TDaemon, typename... Args,
        std::enable_if_t<std::is_base_of<TaskGraphTemplateDaemon<Args...>, TDaemon>::value, int> = 0>
    TaskGraphPipeline* AddTaskGraphDaemon(long ms, Args... args){
        ASSERT_NO_STRING(is_init_ == false)
        ASSERT_NO_STRING(param_manager_ != nullptr)
        ASSERT_NO_STRING(daemon_manager_ != nullptr)

        auto daemon = new TDaemon(std::forward<Args>(args)...);
        daemon->SetInterval(ms);
        daemon->SetTaskGraphParamManager(param_manager_);
        daemon->SetTaskGraphEventManager(event_manager_);
        RStatus status = daemon_manager_->Add(daemon);
        if(!status.IsOk()) return nullptr;
        return this;
    }

    template<typename TEvent, typename TParam = TaskGraphPassedDefaultParam,
        std::enable_if_t<std::is_base_of<TaskGraphEvent, TEvent>::value, int> = 0,
        std::enable_if_t<std::is_base_of<TaskGraphPassedParam, TParam>::value, int> = 0>
    TaskGraphPipeline* AddTaskGraphEvent(const std::string& key, TParam* param = nullptr){
        ASSERT_NO_STRING(is_init_ == false)
        ASSERT_NO_STRING(param_manager_ != nullptr)
        ASSERT_NO_STRING(event_manager_ != nullptr)
        
        event_manager_->param_manager_ = param_manager_;
        RStatus status = event_manager_->CreateWithParam<TEvent, TParam>(key, param);
        if(!status.IsOk()) return nullptr;
        return this;
    }

    TASK_GRAPH_DECLARE_PARAM_MANAGER_WRAPPER
};

}
#endif
