#ifndef MATH_TOOLS_H
#define MATH_TOOLS_H
namespace sablin{

template<typename T>
inline static T Max(const T &lhs, const T &rhs){
    return lhs > rhs? lhs : rhs;
}
}
#endif
