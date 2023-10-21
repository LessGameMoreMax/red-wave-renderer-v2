#include "TaskGraphParamManager.h"
namespace sablin{

TaskGraphParamManager::TaskGraphParamManager(){}

TaskGraphParamManager::~TaskGraphParamManager(){
    Clear();
}

RStatus TaskGraphParamManager::PreSetup(){
    RStatus status;
    for(auto& param: param_map_){
        ASSERT_NO_STRING(param.second != nullptr)
        status += param.second->PreSetup();
    }
    return status;
}

RStatus TaskGraphParamManager::Setup(){
    RStatus status;
    for(auto& param: param_map_){
        status += param.second->Setup();
    }
    return status;
}

RStatus TaskGraphParamManager::Exit(){
    RStatus status;
    for(auto& param: param_map_){
        ASSERT_NO_STRING(param.second != nullptr)
        status += param.second->Exit();
    }
    ASSERT_NO_STRING(status.IsOk())
    Clear();
    return status;
}

RStatus TaskGraphParamManager::Clear(){
    for(auto& param: param_map_)
        delete param.second;
    param_map_.clear();
    return RStatus();
}

void TaskGraphParamManager::ResetWithStatus(const RStatus& cur_status){
    for(auto& param: param_map_){
        param.second->Reset(cur_status);
    }
}

RStatus TaskGraphParamManager::RemoveByKey(const std::string& key){
    std::lock_guard<std::mutex> lg(lock_);
    auto param = param_map_.find(key);
    if(param == param_map_.end()) 
        return RStatus("Param [" + key + "] No Find!", STRING_CODE_LOCATION);
    delete param->second;
    param_map_.erase(param);
    return RStatus();

}

RStatus TaskGraphParamManager::GetKeys(std::vector<std::string>& keys){
    std::lock_guard<std::mutex> lg(lock_);
    for(const auto& param: param_map_)
        keys.emplace_back(param.first);
    return RStatus();
}

}
