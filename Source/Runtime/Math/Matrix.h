#ifndef MATRIX_H
#define MATRIX_H
#include <cstdint>
#include "Vector.h"
#include <xmmintrin.h>
#include "MathTools.h"
namespace sablin{
namespace lenin{

template<typename T>
class Matrix3x3{
public:
    union{
        struct{
            T e00_,e01_,e02_;
            T e10_,e11_,e12_;
            T e20_,e21_,e22_;
        };
        T m_[3][3];
        T s_[9];
    };
public:
    Matrix3x3(): s_{} {}
    Matrix3x3(T e00, T e01, T e02,
              T e10, T e11, T e12,
              T e20, T e21, T e22):
        e00_(e00), e01_(e01), e02_(e02),
        e10_(e10), e11_(e11), e12_(e12),
        e20_(e20), e21_(e21), e22_(e22) {}
    Matrix3x3(const Matrix3x3& lhs) = default;
    Matrix3x3(Matrix3x3&& lhs) = default;
    Matrix3x3& operator=(const Matrix3x3& lhs) = default;
    Matrix3x3& operator=(Matrix3x3&& lhs) = default;
    ~Matrix3x3() = default;

    Matrix3x3& operator+=(const Matrix3x3& lhs){
        for(int8_t i = 0;i != 9; ++i)
            s_[i] += lhs.s_[i];
        return *this;
    }

    template<typename N>
    Matrix3x3& operator+=(const N num){
        for(int8_t i = 0;i != 9; ++i)
            s_[i] += num;
        return *this;
    }

    Matrix3x3& operator-=(const Matrix3x3& lhs){
        for(int8_t i = 0;i != 9; ++i)
            s_[i] -= lhs.s_[i];
        return *this;
    }

    template<typename N>
    Matrix3x3& operator-=(const N num){
        for(int8_t i = 0;i != 9; ++i)
            s_[i] -= num;
        return *this;
    }

    template<typename N>
    Matrix3x3& operator*=(const N num){
        for(int8_t i = 0; i != 9; ++i)
            s_[i] *= num;
        return *this;
    }

    template<typename N>
    Matrix3x3& operator/=(const N num){
        for(int8_t i = 0;i != 9; ++i)
            s_[i] /= num;
        return *this;
    }

    Matrix3x3 Transpose() const{
        Matrix3x3 result(*this);
        Swap(result.m_[0][1], result.m_[1][0]);
        Swap(result.m_[0][2], result.m_[2][0]);
        Swap(result.m_[1][2], result.m_[2][1]);
        return result;
    }

    void Transposed(){
        Swap(m_[0][1], m_[1][0]);
        Swap(m_[0][2], m_[2][0]);
        Swap(m_[1][2], m_[2][1]);
    }

    Matrix3x3 Inverse() const{
        NO_IMPLEMENT_ASSERT(Matrix3x3, Inverse)
        return *this;
    }

    void Inversed(){
        NO_IMPLEMENT_ASSERT(Matrix3x3, Inversed)
    }

    float Det() const{
        return s_[0]*s_[4]*s_[8]+
               s_[1]*s_[5]*s_[6]+
               s_[3]*s_[7]*s_[2]-
               s_[2]*s_[4]*s_[6]-
               s_[1]*s_[3]*s_[8]-
               s_[5]*s_[7]*s_[0];
    }
};

template<typename T>
Matrix3x3<T> operator+(const Matrix3x3<T>& lhs, const Matrix3x3<T>& rhs){
    Matrix3x3 result = lhs;
    result += rhs;
    return result;
}

template<typename T>
Matrix3x3<T> operator-(const Matrix3x3<T>& lhs, const Matrix3x3<T>& rhs){
    Matrix3x3 result = lhs;
    result -= rhs;
    return result;
}

template<typename T, typename N>
Matrix3x3<T> operator+(const Matrix3x3<T>& lhs, const N num){
    Matrix3x3 result = lhs;
    result += num;
    return result;
}

template<typename T, typename N>
Matrix3x3<T> operator-(const Matrix3x3<T>& lhs, const N num){
    Matrix3x3 result = lhs;
    result -= num;
    return result;
}

template<typename T, typename N>
Matrix3x3<T> operator*(const Matrix3x3<T>& lhs, const N num){
    Matrix3x3 result = lhs;
    result *= num;
    return result;
}

template<typename T, typename N>
Matrix3x3<T> operator/(const Matrix3x3<T>& lhs, const N num){
    Matrix3x3 result = lhs;
    lhs /= num;
    return result;
}

template<typename T>
bool operator==(const Matrix3x3<T>& lhs, const Matrix3x3<T>& rhs){
    NO_IMPLEMENT_ASSERT(Matrix3x3, operator==)
    return false;
}

template<>
Matrix3x3<float> Matrix3x3<float>::Inverse() const;

template<>
void Matrix3x3<float>::Inversed();

template<>
bool operator==(const Matrix3x3<float>& lhs, const Matrix3x3<float>& rhs);

template<typename T>
bool operator!=(const Matrix3x3<T>& lhs, const Matrix3x3<T>& rhs){
    return !(lhs == rhs);
}

template<typename T>
class Matrix4x4{
public:
    union{
        struct{
            T e00_,e01_,e02_,e03_;
            T e10_,e11_,e12_,e13_;
            T e20_,e21_,e22_,e23_;
            T e30_,e31_,e32_,e33_;
        };
        T m_[4][4];
        T s_[16];
        __m128 v_[4];
    };
public:
    Matrix4x4(): s_{} {}
    Matrix4x4(T e00, T e01, T e02, T e03,
              T e10, T e11, T e12, T e13,
              T e20, T e21, T e22, T e23,
              T e30, T e31, T e32, T e33):
        e00_(e00), e01_(e01), e02_(e02), e03_(e03),
        e10_(e10), e11_(e11), e12_(e12), e13_(e13),
        e20_(e20), e21_(e21), e22_(e22), e23_(e23),
        e30_(e30), e31_(e31), e32_(e32), e33_(e33) {}
    Matrix4x4(const Matrix4x4& lhs) = default;
    Matrix4x4(Matrix4x4&& lhs) = default;
    Matrix4x4& operator=(const Matrix4x4& lhs) = default;
    Matrix4x4& operator=(Matrix4x4&& lhs) = default;
    ~Matrix4x4() = default;

    Matrix4x4& operator+=(const Matrix4x4& lhs){
        for(int8_t i = 0;i != 4; ++i)
            v_[i] = _mm_add_ps(v_[i], lhs.v_[i]);
        return *this;
    }

    template<typename N>
    Matrix4x4& operator+=(const N num){
        __m128 m_b = _mm_set1_ps(num);
        for(int8_t i = 0;i != 4; ++i)
            v_[i] = _mm_add_ps(v_[i], m_b);
        return *this;
    }

    Matrix4x4& operator-=(const Matrix4x4& lhs){
        for(int8_t i = 0;i != 4; ++i)
            v_[i] = _mm_sub_ps(v_[i], lhs.v_[i]);
        return *this;
    }

    template<typename N>
    Matrix4x4& operator-=(const N num){
        __m128 m_b = _mm_set1_ps(num);
        for(int8_t i = 0;i != 4; ++i)
            v_[i] = _mm_sub_ps(v_[i], m_b);
        return *this;
    }

    template<typename N>
    Matrix4x4& operator*=(const N num){
        __m128 m_b = _mm_set1_ps(num);
        for(int8_t i = 0;i != 4; ++i)
            v_[i] = _mm_mul_ps(v_[i], m_b);
        return *this;
    }

    template<typename N>
    Matrix4x4& operator/=(const N num){
        __m128 m_b = _mm_set1_ps(num);
        for(int8_t i = 0;i != 4; ++i)
            v_[i] = _mm_div_ps(v_[i], m_b);
        return *this;
    }

    Matrix4x4 Transpose() const{
        __m128 tmp0 = _mm_shuffle_ps(v_[0], v_[1], 0x44);
        __m128 tmp2 = _mm_shuffle_ps(v_[0], v_[1], 0xee);
        __m128 tmp1 = _mm_shuffle_ps(v_[2], v_[3], 0x44);
        __m128 tmp3 = _mm_shuffle_ps(v_[2], v_[3], 0xee);

        Matrix4x4 result(*this);
        result.v_[0] = _mm_shuffle_ps(tmp0, tmp1, 0x88);
        result.v_[1] = _mm_shuffle_ps(tmp0, tmp1, 0xdd);
        result.v_[2] = _mm_shuffle_ps(tmp2, tmp3, 0x88);
        result.v_[3] = _mm_shuffle_ps(tmp2, tmp3, 0xdd);
        return result;
    }

    void Transposed(){
        __m128 tmp0 = _mm_shuffle_ps(v_[0], v_[1], 0x44);
        __m128 tmp2 = _mm_shuffle_ps(v_[0], v_[1], 0xee);
        __m128 tmp1 = _mm_shuffle_ps(v_[2], v_[3], 0x44);
        __m128 tmp3 = _mm_shuffle_ps(v_[2], v_[3], 0xee);

        v_[0] = _mm_shuffle_ps(tmp0, tmp1, 0x88);
        v_[1] = _mm_shuffle_ps(tmp0, tmp1, 0xdd);
        v_[2] = _mm_shuffle_ps(tmp2, tmp3, 0x88);
        v_[3] = _mm_shuffle_ps(tmp2, tmp3, 0xdd);
    }

    Matrix4x4 Inverse() const{
        NO_IMPLEMENT_ASSERT(Matrix4x4, Inverse)
        return *this;
    }

    void Inversed(){
        NO_IMPLEMENT_ASSERT(Matrix4x4, Inversed)
    }

    float Det() const{
        NO_IMPLEMENT_ASSERT(Matrix4x4, Det)
        return 0.0f;
    }
};

template<typename T>
Matrix4x4<T> operator+(const Matrix4x4<T>& lhs, const Matrix4x4<T>& rhs){
    Matrix4x4 result = lhs;
    result += rhs;
    return result;
}

template<typename T>
Matrix4x4<T> operator-(const Matrix4x4<T>& lhs, const Matrix4x4<T>& rhs){
    Matrix4x4 result = lhs;
    result -= rhs;
    return result;
}

template<typename T, typename N>
Matrix4x4<T> operator+(const Matrix4x4<T>& lhs, const N num){
    Matrix4x4 result = lhs;
    result += num;
    return result;
}

template<typename T, typename N>
Matrix4x4<T> operator-(const Matrix4x4<T>& lhs, const N num){
    Matrix4x4 result = lhs;
    result -= num;
    return result;
}

template<typename T, typename N>
Matrix4x4<T> operator*(const Matrix4x4<T>& lhs, const N num){
    Matrix4x4 result = lhs;
    result *= num;
    return result;
}

template<typename T, typename N>
Matrix4x4<T> operator/(const Matrix4x4<T>& lhs, const N num){
    Matrix4x4 result = lhs;
    lhs /= num;
    return result;
}

template<typename T>
bool operator==(const Matrix4x4<T>& lhs, const Matrix4x4<T>& rhs){
    NO_IMPLEMENT_ASSERT(Matrix4x4, operator==)
    return false;
}

template<>
Matrix4x4<float> Matrix4x4<float>::Inverse() const;

template<>
void Matrix4x4<float>::Inversed();

template<>
float Matrix4x4<float>::Det() const;

template<>
bool operator==(const Matrix4x4<float>& lhs, const Matrix4x4<float>& rhs);

template<typename T>
bool operator!=(const Matrix4x4<T>& lhs, const Matrix4x4<T>& rhs){
    return !(lhs == rhs);
}

}

extern template class lenin::Matrix3x3<float>;
extern template class lenin::Matrix4x4<float>;

using Matrix3x3f = lenin::Matrix3x3<float>;
using Matrix4x4f = lenin::Matrix4x4<float>;


Vector3f LUPSolve3f(const Matrix3x3f &L, const Matrix3x3f &U,
        const Vector3i &pi, const Vector3f &b);
Vector3i LUPDecomposition3x3f(Matrix3x3f &A);

Vector4f LUPSolve4f(const Matrix4x4f &L, const Matrix4x4f &U,
        const Vector4i &pi, const Vector4f &b);
Vector4i LUPDecomposition4x4f(Matrix4x4f &A);

template<typename T>
Vector3f operator*(const lenin::Vector3<T> &vector, const Matrix3x3f &matrix){
    Vector3f result;
    result.x_ = vector.x_ * matrix.e00_ + vector.y_ * matrix.e10_ + vector.z_ * matrix.e20_;
    result.y_ = vector.x_ * matrix.e01_ + vector.y_ * matrix.e11_ + vector.z_ * matrix.e21_;
    result.z_ = vector.x_ * matrix.e02_ + vector.y_ * matrix.e12_ + vector.z_ * matrix.e22_;
    return result;
}

template<typename T>
Vector3f operator*(const Matrix3x3f &matrix, const lenin::Vector3<T> &vector){
    Vector3f result;
    result.x_ = matrix.e00_ * vector.x_ + matrix.e01_ * vector.y_ + matrix.e02_ * vector.z_;
    result.y_ = matrix.e10_ * vector.x_ + matrix.e11_ * vector.y_ + matrix.e12_ * vector.z_;
    result.z_ = matrix.e20_ * vector.x_ + matrix.e21_ * vector.y_ + matrix.e22_ * vector.z_;
    return result;
}

Matrix3x3f operator*(const Matrix3x3f &lhs, const Matrix3x3f &rhs);

template<typename T>
Vector4f operator*(const lenin::Vector4<T> &vector, const Matrix4x4f &matrix){
    Vector4f result;
    result.x_ = matrix.e00_ * vector.x_ + matrix.e10_ * vector.y_ + matrix.e20_ * vector.z_ + matrix.e30_ * vector.w_;
    result.y_ = matrix.e01_ * vector.x_ + matrix.e11_ * vector.y_ + matrix.e21_ * vector.z_ + matrix.e31_ * vector.w_;
    result.z_ = matrix.e02_ * vector.x_ + matrix.e12_ * vector.y_ + matrix.e22_ * vector.z_ + matrix.e32_ * vector.w_;
    result.w_ = matrix.e03_ * vector.x_ + matrix.e13_ * vector.y_ + matrix.e23_ * vector.z_ + matrix.e33_ * vector.w_;
    return result;
}

template<typename T>
Vector4f operator*(const Matrix4x4f &matrix, const lenin::Vector4<T> &vector){
    Vector4f result;
    result.x_ = matrix.e00_ * vector.x_ + matrix.e01_ * vector.y_ + matrix.e02_ * vector.z_ + matrix.e03_ * vector.w_;
    result.y_ = matrix.e10_ * vector.x_ + matrix.e11_ * vector.y_ + matrix.e12_ * vector.z_ + matrix.e13_ * vector.w_;
    result.z_ = matrix.e20_ * vector.x_ + matrix.e21_ * vector.y_ + matrix.e22_ * vector.z_ + matrix.e23_ * vector.w_;
    result.w_ = matrix.e30_ * vector.x_ + matrix.e31_ * vector.y_ + matrix.e32_ * vector.z_ + matrix.e33_ * vector.w_;
    return result;
}

Matrix4x4f operator*(const Matrix4x4f &lhs, const Matrix4x4f &rhs);

Matrix4x4f LookAtRightHand(const Vector4f& from, const Vector4f& to, const Vector4f& up);

}
#endif
