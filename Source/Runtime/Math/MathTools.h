#ifndef MATH_TOOLS_H
#define MATH_TOOLS_H
#include "Debug/Assertion.h"
#include <xmmintrin.h>
#include "MathDefine.h"
namespace sablin{

template<typename T>
inline static T Max(const T &lhs, const T &rhs){
    return lhs > rhs? lhs : rhs;
}

template<typename T>
inline static T Min(const T &lhs, const T &rhs){
    return lhs < rhs? lhs : rhs;
}

template<typename T>
inline constexpr T Abs(const T &lhs){
    return ((lhs < 0)? -lhs : lhs);
}

inline float Sqrt(const float lhs){
    ASSERT_NO_STRING(lhs > 0)
    __m128 m_a = _mm_set1_ps(lhs);
    __m128 m_b = _mm_sqrt_ps(m_a);
    return m_b[0];
}

inline float Rsqrt(const float lhs){
    ASSERT_NO_STRING(lhs > kFloatError)
    __m128 m_a = _mm_set1_ps(lhs);
    __m128 m_b = _mm_rsqrt_ps(m_a);
    return m_b[0];
}

template<typename T>
inline void Swap(T& lhs, T& rhs){
    T temp = lhs;
    lhs = rhs;
    rhs = temp;
}

inline float ToRadians(const float angle){
    return angle / 180.0f * kPI;
}

}
#endif
