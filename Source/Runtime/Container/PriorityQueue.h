#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H
#include <type_traits>
#include <queue>
#include "Debug/Assertion.h"
#include "Macro/MacroTools.h"
namespace sablin{

template <typename T, typename P>
requires std::is_pointer_v<T>
class PriorityQueue{
private:
    struct Element{
        T pointer_;
        P priority_;

        friend bool operator<(const Element& lhs, const Element& rhs){
            return lhs.priority_ > rhs.priority_;
        }
    };

    std::priority_queue<Element> priority_queue_;
public:
    PriorityQueue() = default;
    ~PriorityQueue(){
        ASSERT_NO_STRING(IsEmpty())
    }
    CLASS_NO_ALLOWED_COPY(PriorityQueue)

    bool IsEmpty(){
        return priority_queue_.empty();
    }

    void Push(T pointer, P priority){
        priority_queue_.push({std::move(pointer), priority});
    }

    std::pair<T,P> Top(){
        const Element& temp = priority_queue_.top();
        return {temp.pointer_, temp.priority_};
    }

    void Pop(){
        priority_queue_.pop();
    }

};
}
#endif
