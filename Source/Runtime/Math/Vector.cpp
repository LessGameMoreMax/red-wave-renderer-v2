#include "Vector.h"
#include "MathTools.h"
namespace sablin{
namespace lenin{

template<>
float Vector2<int32_t>::Normal() const{
    float num = x_ * x_ + y_ * y_;
    return Sqrt(num);
}

template<>
float Vector2<float>::Normal() const{
    float num = x_ * x_ + y_ * y_;
    return Sqrt(num);
}

template<>
Vector2<float>& Vector2<float>::Normalized(){
    float num = x_ * x_ + y_ * y_;
    float r_num = Rsqrt(num);
    operator*=(r_num);
    return *this;
}

template<>
bool operator==(const Vector2<int32_t>& lhs, const Vector2<int32_t>& rhs){
    return lhs.x_ == rhs.x_ && lhs.y_ == rhs.y_;
}

template<>
bool operator==(const Vector2<float>& lhs, const Vector2<float>& rhs){
    return Abs(rhs.x_ - lhs.x_) < kFloatError &&
                Abs(rhs.y_ - lhs.y_) < kFloatError;
}

template<>
float Vector3<int32_t>::Normal() const{
    float num = x_ * x_ + y_ * y_ + z_ * z_;
    return Sqrt(num);
}

template<>
float Vector3<float>::Normal() const{
    float num = x_ * x_ + y_ * y_ + z_ * z_;
    return Sqrt(num);
}

template<>
Vector3<float>& Vector3<float>::Normalized(){
    float num = x_ * x_ + y_ * y_ + z_ * z_;
    float r_num = Rsqrt(num);
    operator*=(r_num);
    return *this;
}

template<>
bool operator==(const Vector3<int32_t>& lhs, const Vector3<int32_t>& rhs){
    return lhs.x_ == rhs.x_ && lhs.y_ == rhs.y_ && lhs.z_ == rhs.z_;
}

template<>
bool operator==(const Vector3<float>& lhs, const Vector3<float>& rhs){
    return Abs(rhs.x_ - lhs.x_) < kFloatError &&
                Abs(rhs.y_ - lhs.y_) < kFloatError &&
                    Abs(rhs.z_ - lhs.z_) < kFloatError;
}

template<>
float Vector4<int32_t>::Normal() const{
    float num = x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_;
    return Sqrt(num);
}

template<>
float Vector4<float>::Normal() const{
    float num = x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_;
    return Sqrt(num);
}

template<>
Vector4<float>& Vector4<float>::Normalized(){
    float num = x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_;
    float r_num = Rsqrt(num);
    operator*=(r_num);
    return *this;
}

template<>
bool operator==(const Vector4<int32_t>& lhs, const Vector4<int32_t>& rhs){
    return lhs.x_ == rhs.x_ && lhs.y_ == rhs.y_ && 
            lhs.z_ == rhs.z_ && lhs.w_ == rhs.w_;
}

template<>
bool operator==(const Vector4<float>& lhs, const Vector4<float>& rhs){
    return Abs(rhs.x_ - lhs.x_) < kFloatError &&
                Abs(rhs.y_ - lhs.y_) < kFloatError &&
                    Abs(rhs.z_ - lhs.z_) < kFloatError &&
                        Abs(rhs.w_ - lhs.w_) < kFloatError;
}

}

template class lenin::Vector2<int32_t>;
template class lenin::Vector2<float>;
template class lenin::Vector3<int32_t>;
template class lenin::Vector3<float>;
template class lenin::Vector4<int32_t>;
template class lenin::Vector4<float>;

}
