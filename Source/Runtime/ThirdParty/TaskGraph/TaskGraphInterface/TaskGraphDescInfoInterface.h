#ifndef TASK_GRAPH_DESC_INFO_INTERFACE_H
#define TASK_GRAPH_DESC_INFO_INTERFACE_H
#include <string>
namespace sablin{

class TaskGraphDescInfoInterface{
protected:
    std::string name_;
    std::string session_;
    std::string description_;
public:

    explicit TaskGraphDescInfoInterface() = default;
    virtual ~TaskGraphDescInfoInterface() = default;

    inline const std::string& GetName() const{
        return name_;
    }

    inline const std::string& GetSession() const{
        return session_;
    }
    
    inline const std::string& GetDescription() const{
        return description_;
    }

    virtual TaskGraphDescInfoInterface* SetName(const std::string& name){
        name_ = name;
        return this;
    }

    virtual TaskGraphDescInfoInterface* SetDescription(const std::string& description){
        description_ = description;
        return this;
    }
};

}
#endif
