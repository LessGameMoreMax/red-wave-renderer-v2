#include "TaskGraphStaticEngine.h"
namespace sablin{

TaskGraphStaticEngine::TaskGraphStaticEngine():
    TaskGraphEngine(),
    run_element_size_(0){}

RStatus TaskGraphStaticEngine::Setup(const std::set<TaskGraphElement*,
        TaskGraphElementSorter>& elements){
    RStatus status;
    status = Mark(elements);
    if(!status.IsOk()) return RStatus("TaskGraphStaticEngin::Setup", STRING_CODE_LOCATION);
    status = Analyse(elements);
    return status;
}

RStatus TaskGraphStaticEngine::Mark(const std::set<TaskGraphElement*,
            TaskGraphElementSorter>& elements){
    total_element_size_ = elements.size();
    for(TaskGraphElement* element: elements){
        if(element->dependence_.size() == 1 &&
            (*element->dependence_.begin())->run_before_.size() == 1 &&
            (*(element->dependence_.begin()))->run_before_.find(element) != (*(element->dependence_.begin()))->run_before_.end() &&
            element->GetBindingIndex() == (*(element->dependence_.begin()))->GetBindingIndex()){
            element->linkable_ = true;
        }
    }
    return RStatus();
}

RStatus TaskGraphStaticEngine::Run(){
    run_element_size_ = 0;
    std::vector<std::future<RStatus>> futures;

    RStatus status;
    for(std::vector<TaskGraphCluster>& cluster_arr: para_cluster_arrs_){
        futures.clear();
        for(TaskGraphCluster& cluster: cluster_arr){
            futures.emplace_back(thread_pool_->Commit([&cluster] {
                return cluster.Process(false);
            }, CalcIndex(&cluster)));
            run_element_size_ += cluster.GetElementNum();
        }
        for(auto& fut: futures){
            status = fut.get();
            if(!status.IsOk()) return RStatus("TaskGraphStaticEngine::Run", STRING_CODE_LOCATION);
        }
    }
    return status;
}

RStatus TaskGraphStaticEngine::AfterRunCheck(){
    if(run_element_size_ != total_element_size_){
        const std::string& error_info = "run size = [" + std::to_string(run_element_size_) +
            "], total size = [" + std::to_string(total_element_size_) + "] ...";
        return RStatus("static engine run element size check failed, " + error_info, STRING_CODE_LOCATION);
    }
    for(std::vector<TaskGraphCluster>& cluster_arr: para_cluster_arrs_){
        for(TaskGraphCluster& cluster: cluster_arr){
            if(!cluster.IsDone()){
                return RStatus("Static Engine Run Done Status Check Failed!", STRING_CODE_LOCATION);
            }
        }
    }
    return RStatus();
}

RStatus TaskGraphStaticEngine::Analyse(const std::set<TaskGraphElement*,
        TaskGraphElementSorter>& elements){
    run_element_size_ = 0;
    para_cluster_arrs_.clear();

    std::vector<TaskGraphCluster> cur_cluster_arr;
    for(TaskGraphElement* element: elements){
        if(!element->IsRunnable() || element->IsLinkable()){
            continue;
        }
        TaskGraphCluster cur_cluster;
        TaskGraphElement* cur_element = element;
        cur_cluster.AddElement(cur_element);

        while(cur_element->run_before_.size() == 1 &&
            (*cur_element->run_before_.begin())->IsLinkable()){
            cur_element = (*cur_element->run_before_.begin());
            cur_cluster.AddElement(cur_element);
        }
        cur_cluster_arr.emplace_back(cur_cluster);
    }
    para_cluster_arrs_.emplace_back(cur_cluster_arr);
    RStatus status;

    std::vector<TaskGraphCluster> runnable_cluster_arr;
    while(!cur_cluster_arr.empty() && run_element_size_ <= total_element_size_){
        runnable_cluster_arr = cur_cluster_arr;
        cur_cluster_arr.clear();

        for(TaskGraphCluster& cluster: runnable_cluster_arr){
            status = cluster.Process(true);
            if(!status.IsOk()){
                return RStatus("", STRING_CODE_LOCATION);
            }
        }
        run_element_size_ += runnable_cluster_arr.size();

        std::set<TaskGraphElement*> duplications;
        for(TaskGraphCluster& cluster: runnable_cluster_arr){
            for(TaskGraphElement* element: cluster.group_elements_arr_){
                for(TaskGraphElement* cur: element->run_before_){
                    if(cur->IsRunnable() && duplications.end() == duplications.find(cur)){
                        TaskGraphCluster cur_cluster;
                        TaskGraphElement* cur_element = cur;
                        cur_cluster.AddElement(cur_element);
                        duplications.insert(cur_element);

                        while(cur_element->run_before_.size() == 1&&
                            (*cur_element->run_before_.begin())->IsLinkable()){
                            cur_element = (*cur_element->run_before_.begin());
                            cur_cluster.AddElement(cur_element);
                            duplications.insert(cur_element);
                        }
                        cur_cluster_arr.emplace_back(cur_cluster);
                    }
                }
            }
        }

        if(!cur_cluster_arr.empty()){
            para_cluster_arrs_.emplace_back(cur_cluster_arr);
        }
    }
    return status;
}

}
