#ifndef VECTOR_H
#define VECTOR_H
#include <cstdint>
#include "Debug/Assertion.h"
namespace sablin{
namespace lenin{

template<typename T>
class Vector2{
public:
    union{
        struct{
            T x_;
            T y_;
        };
        T s_[2];
    };
public:
    explicit Vector2(): s_{} {}
    Vector2(const T x, const T y):
        x_(x), y_(y) {}
    Vector2(const Vector2& rhs) = default;
    Vector2(Vector2&& rhs) = default;
    Vector2& operator=(const Vector2& rhs) = default;
    Vector2& operator=(Vector2&& rhs) = default;
    ~Vector2() = default;

    Vector2& operator+=(const Vector2& rhs){
        x_ += rhs.x_;
        y_ += rhs.y_;
        return *this;
    }
    
    template<typename N>
    Vector2& operator+=(const N rhs){
        x_ += rhs;
        y_ += rhs;
        return *this;
    }

    Vector2& operator-=(const Vector2& rhs){
        x_ -= rhs.x_;
        y_ -= rhs.y_;
        return *this;
    }

    template<typename N>
    Vector2& operator-=(const N rhs){
        x_ -= rhs;
        y_ -= rhs;
        return *this;
    }

    template<typename N>
    Vector2& operator*=(const N rhs){
        x_ *= rhs;
        y_ *= rhs;
        return *this;
    }

    template<typename N>
    Vector2& operator/=(const N rhs){
        x_ /= rhs;
        y_ /= rhs;
        return *this;
    }

    float Normal() const{
        NO_IMPLEMENT_ASSERT(Vector2, Normal)
    }

    Vector2& Normalized(){
        NO_IMPLEMENT_ASSERT(Vector2, Normalized)
        return *this;
    }
};

template<>
float Vector2<int32_t>::Normal() const;

template<>
float Vector2<float>::Normal() const;

template<>
Vector2<float>& Vector2<float>::Normalized();

template<typename T>
Vector2<T> operator+(const Vector2<T>& lhs, const Vector2<T>& rhs){
    Vector2<T> result = lhs;
    result += rhs;
    return result;
}

template<typename T>
Vector2<T> operator-(const Vector2<T>& lhs, const Vector2<T>& rhs){
    Vector2<T> result = lhs;
    result -= rhs;
    return result;
}

template<typename T, typename N>
Vector2<T> operator+(const Vector2<T>& lhs, const N rhs){
    Vector2<T> result = lhs;
    result += rhs;
    return result;
}

template<typename T, typename N>
Vector2<T> operator-(const Vector2<T>& lhs, const N rhs){
    Vector2<T> result = lhs;
    result -= rhs;
    return result;
}

template<typename T, typename N>
Vector2<T> operator*(const Vector2<T>& lhs, const N rhs){
    Vector2<T> result = lhs;
    result *= rhs;
    return result;
}

template<typename T>
Vector2<T> operator*(const Vector2<T>& lhs, const Vector2<T>& rhs){
    return Vector2<T>(lhs.x_ * rhs.x_,
            lhs.y_ * rhs.y_);
}

template<typename T, typename N>
Vector2<T> operator/(const Vector2<T>& lhs, const N rhs){
    Vector2<T> result = lhs;
    lhs /= rhs;
    return result;
}

template<typename T>
bool operator==(const Vector2<T>& lhs, const Vector2<T>& rhs){
    NO_IMPLEMENT_ASSERT(Vector2<T>, operator==)
}

template<typename T>
bool operator!=(const Vector2<T>& lhs, const Vector2<T>& rhs){
    return !(lhs == rhs);
}

template<>
bool operator==(const Vector2<int32_t>& lhs, const Vector2<int32_t>& rhs);

template<>
bool operator==(const Vector2<float>& lhs, const Vector2<float>& rhs);

template<typename T>
class Vector3{
public:
    union{
        struct{
            T x_;
            T y_;
            T z_;
        };
        T s_[3];
    };
public:
    explicit Vector3(): s_{} {}
    Vector3(const T x, const T y, const T z):
        x_(x), y_(y), z_(z){}
    Vector3(const Vector3& rhs) = default;
    Vector3(Vector3&& rhs) = default;
    Vector3& operator=(const Vector3& rhs) = default;
    Vector3& operator=(Vector3&& rhs) = default;
    ~Vector3() = default;

    Vector3& operator+=(const Vector3& rhs){
        x_ += rhs.x_;
        y_ += rhs.y_;
        z_ += rhs.z_;
        return *this;
    }
    
    template<typename N>
    Vector3& operator+=(const N rhs){
        x_ += rhs;
        y_ += rhs;
        z_ += rhs;
        return *this;
    }

    Vector3& operator-=(const Vector3& rhs){
        x_ -= rhs.x_;
        y_ -= rhs.y_;
        z_ -= rhs.z_;
        return *this;
    }

    template<typename N>
    Vector3& operator-=(const N rhs){
        x_ -= rhs;
        y_ -= rhs;
        z_ -= rhs;
        return *this;
    }

    template<typename N>
    Vector3& operator*=(const N rhs){
        x_ *= rhs;
        y_ *= rhs;
        z_ *= rhs;
        return *this;
    }

    template<typename N>
    Vector3& operator/=(const N rhs){
        x_ /= rhs;
        y_ /= rhs;
        z_ /= rhs;
        return *this;
    }

    float Normal() const;
    Vector3& Normalized();
};

template<>
float Vector3<int32_t>::Normal() const;

template<>
float Vector3<float>::Normal() const;

template<>
Vector3<float>& Vector3<float>::Normalized();

template<typename T>
Vector3<T> operator+(const Vector3<T>& lhs, const Vector3<T>& rhs){
    Vector3<T> result = lhs;
    result += rhs;
    return result;
}

template<typename T>
Vector3<T> operator-(const Vector3<T>& lhs, const Vector3<T>& rhs){
    Vector3<T> result = lhs;
    result -= rhs;
    return result;
}

template<typename T, typename N>
Vector3<T> operator+(const Vector3<T>& lhs, const N rhs){
    Vector3<T> result = lhs;
    result += rhs;
    return result;
}

template<typename T, typename N>
Vector3<T> operator-(const Vector3<T>& lhs, const N rhs){
    Vector3<T> result = lhs;
    result -= rhs;
    return result;
}

template<typename T, typename N>
Vector3<T> operator*(const Vector3<T>& lhs, const N rhs){
    Vector3<T> result = lhs;
    result *= rhs;
    return result;
}

template<typename T>
Vector3<T> operator*(const Vector3<T>& lhs, const Vector3<T>& rhs){
    return Vector3<T>(lhs.x_ * rhs.x_,
            lhs.y_ * rhs.y_,
            lhs.z_ * rhs.z_);
}

template<typename T, typename N>
Vector3<T> operator/(const Vector3<T>& lhs, const N rhs){
    Vector3<T> result = lhs;
    lhs /= rhs;
    return result;
}

template<typename T>
bool operator==(const Vector3<T>& lhs, const Vector3<T>& rhs){
    NO_IMPLEMENT_ASSERT(Vector3<T>, operator==)
}

template<typename T>
bool operator!=(const Vector3<T>& lhs, const Vector3<T>& rhs){
    return !(lhs == rhs);
}

template<>
bool operator==(const Vector3<int32_t>& lhs, const Vector3<int32_t>& rhs);

template<>
bool operator==(const Vector3<float>& lhs, const Vector3<float>& rhs);

template<typename T>
class Vector4{
public:
    union{
        struct{
            T x_;
            T y_;
            T z_;
            T w_;
        };
        T s_[4];
    };
public:
    explicit Vector4(): s_{} {}
    Vector4(const T x, const T y, const T z, const T w):
        x_(x), y_(y), z_(z), w_(w){}
    Vector4(const Vector4& rhs) = default;
    Vector4(Vector4&& rhs) = default;
    Vector4& operator=(const Vector4& rhs) = default;
    Vector4& operator=(Vector4&& rhs) = default;
    ~Vector4() = default;

    Vector4& operator+=(const Vector4& rhs){
        x_ += rhs.x_;
        y_ += rhs.y_;
        z_ += rhs.z_;
        w_ += rhs.w_;
        return *this;
    }
    
    template<typename N>
    Vector4& operator+=(const N rhs){
        x_ += rhs;
        y_ += rhs;
        z_ += rhs;
        w_ += rhs;
        return *this;
    }

    Vector4& operator-=(const Vector4& rhs){
        x_ -= rhs.x_;
        y_ -= rhs.y_;
        z_ -= rhs.z_;
        w_ -= rhs.w_;
        return *this;
    }

    template<typename N>
    Vector4& operator-=(const N rhs){
        x_ -= rhs;
        y_ -= rhs;
        z_ -= rhs;
        w_ -= rhs;
        return *this;
    }

    template<typename N>
    Vector4& operator*=(const N rhs){
        x_ *= rhs;
        y_ *= rhs;
        z_ *= rhs;
        w_ *= rhs;
        return *this;
    }

    template<typename N>
    Vector4& operator/=(const N rhs){
        x_ /= rhs;
        y_ /= rhs;
        z_ /= rhs;
        w_ /= rhs;
        return *this;
    }

    float Normal() const;
    Vector4& Normalized();
};

template<>
float Vector4<int32_t>::Normal() const;

template<>
float Vector4<float>::Normal() const;

template<>
Vector4<float>& Vector4<float>::Normalized();

template<typename T>
Vector4<T> operator+(const Vector4<T>& lhs, const Vector4<T>& rhs){
    Vector4<T> result = lhs;
    result += rhs;
    return result;
}

template<typename T>
Vector4<T> operator-(const Vector4<T>& lhs, const Vector4<T>& rhs){
    Vector4<T> result = lhs;
    result -= rhs;
    return result;
}

template<typename T, typename N>
Vector4<T> operator+(const Vector4<T>& lhs, const N rhs){
    Vector4<T> result = lhs;
    result += rhs;
    return result;
}

template<typename T, typename N>
Vector4<T> operator-(const Vector4<T>& lhs, const N rhs){
    Vector4<T> result = lhs;
    result -= rhs;
    return result;
}

template<typename T, typename N>
Vector4<T> operator*(const Vector4<T>& lhs, const N rhs){
    Vector4<T> result = lhs;
    result *= rhs;
    return result;
}

template<typename T>
Vector4<T> operator*(const Vector4<T>& lhs, const Vector4<T>& rhs){
    return Vector4<T>(lhs.x_ * rhs.x_,
            lhs.y_ * rhs.y_,
            lhs.z_ * rhs.z_,
            lhs.w_ * rhs.w_);
}

template<typename T, typename N>
Vector4<T> operator/(const Vector4<T>& lhs, const N rhs){
    Vector4<T> result = lhs;
    lhs /= rhs;
    return result;
}

template<typename T>
bool operator==(const Vector4<T>& lhs, const Vector4<T>& rhs){
    NO_IMPLEMENT_ASSERT(Vector4<T>, operator==)
}

template<typename T>
bool operator!=(const Vector4<T>& lhs, const Vector4<T>& rhs){
    return !(lhs == rhs);
}

template<>
bool operator==(const Vector4<int32_t>& lhs, const Vector4<int32_t>& rhs);

template<>
bool operator==(const Vector4<float>& lhs, const Vector4<float>& rhs);

}

extern template class lenin::Vector2<int32_t>;
extern template class lenin::Vector2<float>;
extern template class lenin::Vector3<int32_t>;
extern template class lenin::Vector3<float>;
extern template class lenin::Vector4<int32_t>;
extern template class lenin::Vector4<float>;

using Vector2i = lenin::Vector2<int32_t>;
using Vector2f = lenin::Vector2<float>;
using Vector3i = lenin::Vector3<int32_t>;
using Vector3f = lenin::Vector3<float>;
using Vector4i = lenin::Vector4<int32_t>;
using Vector4f = lenin::Vector4<float>;

template<typename T, typename I>
float DotProduct(const lenin::Vector3<T>& lhs, const lenin::Vector3<I>& rhs){
    return rhs.x_*lhs.x_+rhs.y_*lhs.y_+rhs.z_*lhs.z_;
}

template<typename T>
lenin::Vector3<T> CrossProduct(const lenin::Vector3<T>& lhs, const lenin::Vector3<T>& rhs){
    return lenin::Vector3<T>(rhs.y_*lhs.z_-rhs.z_*lhs.y_,
                             rhs.z_*lhs.x_-rhs.x_*lhs.z_,
                             rhs.x_*lhs.y_-rhs.y_*lhs.x_);    
}

template<typename T, typename I>
float DotProduct(const lenin::Vector4<T>& lhs, const lenin::Vector4<I>& rhs){
    return rhs.x_*lhs.x_+rhs.y_*lhs.y_+rhs.z_*lhs.z_+rhs.w_*lhs.w_;
}

}
#endif
