#ifndef SERIAL_UNIQUE_ARRAY_H
#define SERIAL_UNIQUE_ARRAY_H
#include <vector>
#include <set>

template <typename T>
class SerialUniqueArray{
private:
    std::set<T> inner_set_;
    std::vector<T> inner_array_;
public:
    void UniqueAdd(const T& value){
        if(inner_set_.find(value) != inner_set_.end()) return;
        inner_set_.insert(value);
        inner_array_.emplace_back(value);
    }

    void GetUniqueArray(std::vector<T>& array){
        for(const T& value: inner_array_)
            array.push_back(value);
    }

    void Clear(){
        inner_set_.clear();
        inner_array_.clear();
    }

};
#endif
