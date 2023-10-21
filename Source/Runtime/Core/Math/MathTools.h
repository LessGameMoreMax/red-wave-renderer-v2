#ifndef MATH_TOOLS_H
#define MATH_TOOLS_H
#include <chrono>
namespace sablin{

template<typename T>
inline static T Max(const T &lhs, const T &rhs){
    return lhs > rhs? lhs : rhs;
}

template<typename T>
inline static T Min(const T &lhs, const T &rhs){
    return lhs < rhs? lhs : rhs;
}

inline long GetCurrentMs(){
    return std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
}
}
#endif
