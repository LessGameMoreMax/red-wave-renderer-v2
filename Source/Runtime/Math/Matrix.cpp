#include "Matrix.h"
#include "MathTools.h"
namespace sablin{
namespace lenin{

template<>
Matrix3x3<float> Matrix3x3<float>::Inverse() const{
    Matrix3x3f A(*this);
    Vector3i pi = LUPDecomposition3x3f(A);
    Matrix3x3f L{
        1.0f,   0.0f,   0.0f,
        A.e10_, 1.0f,   0.0f,
        A.e20_, A.e21_, 1.0f
    };
    Matrix3x3f U{
        A.e00_, A.e01_, A.e02_,
        0.0f,   A.e11_, A.e12_,
        0.0f,   0.0f,   A.e22_
    };
    Vector3f b0{1.0f, 0.0f, 0.0f};
    Vector3f b1{0.0f, 1.0f, 0.0f};
    Vector3f b2{0.0f, 0.0f, 1.0f};
    b0 = LUPSolve3f(L, U, pi, b0);
    b1 = LUPSolve3f(L, U, pi, b1);
    b2 = LUPSolve3f(L, U, pi, b2);
    return Matrix3x3f{b0.x_, b1.x_, b2.x_,
                      b0.y_, b1.y_, b2.y_,
                      b0.z_, b1.z_, b2.z_};

}

template<>
void Matrix3x3<float>::Inversed(){
    Matrix3x3f A(*this);
    Vector3i pi = LUPDecomposition3x3f(A);
    Matrix3x3f L{
        1.0f,   0.0f,   0.0f,
        A.e10_, 1.0f,   0.0f,
        A.e20_, A.e21_, 1.0f
    };
    Matrix3x3f U{
        A.e00_, A.e01_, A.e02_,
        0.0f,   A.e11_, A.e12_,
        0.0f,   0.0f,   A.e22_
    };
    Vector3f b0{1.0f, 0.0f, 0.0f};
    Vector3f b1{0.0f, 1.0f, 0.0f};
    Vector3f b2{0.0f, 0.0f, 1.0f};
    b0 = LUPSolve3f(L, U, pi, b0);
    b1 = LUPSolve3f(L, U, pi, b1);
    b2 = LUPSolve3f(L, U, pi, b2);
    e00_ = b0.x_; e01_ = b1.x_; e02_ = b2.x_; 
    e10_ = b0.y_; e11_ = b1.y_; e12_ = b2.y_; 
    e20_ = b0.z_; e21_ = b1.z_; e22_ = b2.z_; 
}

template<>
bool operator==(const Matrix3x3<float>& lhs, const Matrix3x3<float>& rhs){
    for(int8_t i = 0;i != 9; ++i)
        if(Abs(lhs.s_[i] - rhs.s_[i]) > kFloatError)
            return false;
    return true;
}

template<>
Matrix4x4<float> Matrix4x4<float>::Inverse() const{
    Matrix4x4f A(*this);
    Vector4i pi = LUPDecomposition4x4f(A);
    Matrix4x4f L{
        1.0f,   0.0f,   0.0f,   0.0f,
        A.e10_, 1.0f,   0.0f,   0.0f,
        A.e20_, A.e21_, 1.0f,   0.0f,
        A.e30_, A.e31_, A.e32_, 1.0f
    };
    Matrix4x4f U{
        A.e00_, A.e01_, A.e02_, A.e03_,
        0.0f,   A.e11_, A.e12_, A.e13_,
        0.0f,   0.0f,   A.e22_, A.e23_,
        0.0f,   0.0f,   0.0f,   A.e33_
    };
    Vector4f b0{1.0f, 0.0f, 0.0f, 0.0f};
    Vector4f b1{0.0f, 1.0f, 0.0f, 0.0f};
    Vector4f b2{0.0f, 0.0f, 1.0f, 0.0f};
    Vector4f b3{0.0f, 0.0f, 0.0f, 1.0f};
    b0 = LUPSolve4f(L, U, pi, b0);
    b1 = LUPSolve4f(L, U, pi, b1);
    b2 = LUPSolve4f(L, U, pi, b2);
    b3 = LUPSolve4f(L, U, pi, b3);
    return Matrix4x4f{b0.x_, b1.x_, b2.x_, b3.x_,
                      b0.y_, b1.y_, b2.y_, b3.y_,
                      b0.z_, b1.z_, b2.z_, b3.z_,
                      b0.w_, b1.w_, b2.w_, b3.w_};
}

template<>
void Matrix4x4<float>::Inversed(){
    Matrix4x4f A(*this);
    Vector4i pi = LUPDecomposition4x4f(A);
    Matrix4x4f L{
        1.0f,   0.0f,   0.0f,   0.0f,
        A.e10_, 1.0f,   0.0f,   0.0f,
        A.e20_, A.e21_, 1.0f,   0.0f,
        A.e30_, A.e31_, A.e32_, 1.0f
    };
    Matrix4x4f U{
        A.e00_, A.e01_, A.e02_, A.e03_,
        0.0f,   A.e11_, A.e12_, A.e13_,
        0.0f,   0.0f,   A.e22_, A.e23_,
        0.0f,   0.0f,   0.0f,   A.e33_
    };
    Vector4f b0{1.0f, 0.0f, 0.0f, 0.0f};
    Vector4f b1{0.0f, 1.0f, 0.0f, 0.0f};
    Vector4f b2{0.0f, 0.0f, 1.0f, 0.0f};
    Vector4f b3{0.0f, 0.0f, 0.0f, 1.0f};
    b0 = LUPSolve4f(L, U, pi, b0);
    b1 = LUPSolve4f(L, U, pi, b1);
    b2 = LUPSolve4f(L, U, pi, b2);
    b3 = LUPSolve4f(L, U, pi, b3);
    e00_ = b0.x_; e01_ = b1.x_; e02_ = b2.x_; e03_ = b3.x_;
    e10_ = b0.y_; e11_ = b1.y_; e12_ = b2.y_; e13_ = b3.y_;
    e20_ = b0.z_; e21_ = b1.z_; e22_ = b2.z_; e23_ = b3.z_;
    e30_ = b0.w_; e31_ = b1.w_; e32_ = b2.w_; e33_ = b3.w_;
}

template<>
float Matrix4x4<float>::Det() const{
    Matrix4x4f A(*this);
    LUPDecomposition4x4f(A);
    return -(A.e00_ * A.e11_ * A.e22_ * A.e33_);
}

template<>
bool operator==(const Matrix4x4<float>& lhs, const Matrix4x4<float>& rhs){
    for(int8_t i = 0;i != 16; ++i)
        if(Abs(lhs.s_[i] - rhs.s_[i]) > kFloatError)
            return false;
    return true;
}

}

template class lenin::Matrix3x3<float>;
template class lenin::Matrix4x4<float>;

Vector3f LUPSolve3f(const Matrix3x3f &L, const Matrix3x3f &U,
        const Vector3i &pi, const Vector3f &b){
    Vector3f x;
    float y[3]{0.0f};

    for(int8_t i = 0; i != 3; ++i){
        float second = 0.0f;
        for(int8_t j = 0; j != i; ++j)
            second += L.m_[i][j] * y[j];
        y[i] = b.s_[pi.s_[i]] - second;
    }

    for(int8_t i = 2; i != -1; --i){
        float second = 0.0f;
        for(int8_t j = i + 1; j != 3; ++j)
            second += U.m_[i][j] * x.s_[j];
        x.s_[i] = (y[i] - second) / U.m_[i][i];
    }
    return x;
}

Vector3i LUPDecomposition3x3f(Matrix3x3f &A){
    Vector3i pi{0, 1, 2};
    for(int8_t k = 0;k != 3; ++k){
        int8_t k_s = 0;
        float p = 0.0f;
        for(int8_t i = k; i != 3; ++i){
            if(Abs(A.m_[i][k]) > p){
                p = Abs(A.m_[i][k]);
                k_s = i;
            }
        }
        ASSERT_NO_STRING(Abs(p) > kFloatError)
        Swap(pi.s_[k], pi.s_[k_s]);
        for(int8_t i = 0; i != 3; ++i)
            Swap(A.m_[k][i], A.m_[k_s][i]);
        for(int8_t i = k + 1; i != 3; ++i){
            A.m_[i][k] /= A.m_[k][k];
            for(int8_t j = k + 1; j != 3; ++j)
                A.m_[i][j] -= A.m_[i][k] * A.m_[k][j];
        }
    }
    return pi;
}

Vector4f LUPSolve4f(const Matrix4x4f &L, const Matrix4x4f &U,
        const Vector4i &pi, const Vector4f &b){
    Vector4f x;
    float y[4]{};

    for(int8_t i = 0; i != 4; ++i){
        float second = 0.0f;
        for(int8_t j = 0; j != i; ++j)
            second += L.m_[i][j] * y[j];
        y[i] = b.s_[pi.s_[i]] - second;
    }

    for(int8_t i = 3; i != -1; --i){
        float second = 0.0f;
        for(int8_t j = i + 1; j != 4; ++j)
            second += U.m_[i][j] * x.s_[j];
        x.s_[i] = (y[i] - second) / U.m_[i][i];
    }
    return x;
}

Vector4i LUPDecomposition4x4f(Matrix4x4f &A){
    Vector4i pi{0, 1, 2, 3};
    for(int8_t k = 0;k != 4; ++k){
        int8_t k_s = 0;
        float p = 0.0f;
        for(int8_t i = k; i != 4; ++i){
            if(Abs(A.m_[i][k]) > p){
                p = Abs(A.m_[i][k]);
                k_s = i;
            }
        }
        ASSERT_NO_STRING(Abs(p) < kFloatError)
        Swap(pi.s_[k], pi.s_[k_s]);
        for(int8_t i = 0; i != 4; ++i)
            Swap(A.m_[k][i], A.m_[k_s][i]);
        for(int8_t i = k + 1; i != 4; ++i){
            A.m_[i][k] /= A.m_[k][k];
            for(int8_t j = k + 1; j != 4; ++j)
                A.m_[i][j] -= A.m_[i][k] * A.m_[k][j];
        }
    }
    return pi;
}

Matrix3x3f operator*(const Matrix3x3f &lhs, const Matrix3x3f &rhs){
    Matrix3x3f result;
    result.e00_ = lhs.e00_ * rhs.e00_ + lhs.e01_ * rhs.e10_ + lhs.e02_ * rhs.e20_;
    result.e01_ = lhs.e00_ * rhs.e01_ + lhs.e01_ * rhs.e11_ + lhs.e02_ * rhs.e21_;
    result.e02_ = lhs.e00_ * rhs.e02_ + lhs.e01_ * rhs.e12_ + lhs.e02_ * rhs.e22_;
    result.e10_ = lhs.e10_ * rhs.e00_ + lhs.e11_ * rhs.e10_ + lhs.e12_ * rhs.e20_;
    result.e11_ = lhs.e10_ * rhs.e01_ + lhs.e11_ * rhs.e11_ + lhs.e12_ * rhs.e21_;
    result.e12_ = lhs.e10_ * rhs.e02_ + lhs.e11_ * rhs.e12_ + lhs.e12_ * rhs.e22_;
    result.e20_ = lhs.e20_ * rhs.e00_ + lhs.e21_ * rhs.e10_ + lhs.e22_ * rhs.e20_;
    result.e21_ = lhs.e20_ * rhs.e01_ + lhs.e21_ * rhs.e11_ + lhs.e22_ * rhs.e21_;
    result.e22_ = lhs.e20_ * rhs.e02_ + lhs.e21_ * rhs.e12_ + lhs.e22_ * rhs.e22_;
    return result;
}

Matrix4x4f operator*(const Matrix4x4f &lhs, const Matrix4x4f &rhs){
    Matrix4x4f result;
    result.e00_ = lhs.e00_ * rhs.e00_ + lhs.e01_ * rhs.e10_ + lhs.e02_ * rhs.e20_ + lhs.e03_ * rhs.e30_;
    result.e01_ = lhs.e00_ * rhs.e01_ + lhs.e01_ * rhs.e11_ + lhs.e02_ * rhs.e21_ + lhs.e03_ * rhs.e31_;
    result.e02_ = lhs.e00_ * rhs.e02_ + lhs.e01_ * rhs.e12_ + lhs.e02_ * rhs.e22_ + lhs.e03_ * rhs.e32_;
    result.e03_ = lhs.e00_ * rhs.e03_ + lhs.e01_ * rhs.e13_ + lhs.e02_ * rhs.e23_ + lhs.e03_ * rhs.e33_;

    result.e10_ = lhs.e10_ * rhs.e00_ + lhs.e11_ * rhs.e10_ + lhs.e12_ * rhs.e20_ + lhs.e13_ * rhs.e30_;
    result.e11_ = lhs.e10_ * rhs.e01_ + lhs.e11_ * rhs.e11_ + lhs.e12_ * rhs.e21_ + lhs.e13_ * rhs.e31_;
    result.e12_ = lhs.e10_ * rhs.e02_ + lhs.e11_ * rhs.e12_ + lhs.e12_ * rhs.e22_ + lhs.e13_ * rhs.e32_;
    result.e13_ = lhs.e10_ * rhs.e03_ + lhs.e11_ * rhs.e13_ + lhs.e12_ * rhs.e23_ + lhs.e13_ * rhs.e33_;

    result.e20_ = lhs.e20_ * rhs.e00_ + lhs.e21_ * rhs.e10_ + lhs.e22_ * rhs.e20_ + lhs.e23_ * rhs.e30_;
    result.e21_ = lhs.e20_ * rhs.e01_ + lhs.e21_ * rhs.e11_ + lhs.e22_ * rhs.e21_ + lhs.e23_ * rhs.e31_;
    result.e22_ = lhs.e20_ * rhs.e02_ + lhs.e21_ * rhs.e12_ + lhs.e22_ * rhs.e22_ + lhs.e23_ * rhs.e32_;
    result.e23_ = lhs.e20_ * rhs.e03_ + lhs.e21_ * rhs.e13_ + lhs.e22_ * rhs.e23_ + lhs.e23_ * rhs.e33_;

    result.e30_ = lhs.e30_ * rhs.e00_ + lhs.e31_ * rhs.e10_ + lhs.e32_ * rhs.e20_ + lhs.e33_ * rhs.e30_;
    result.e31_ = lhs.e30_ * rhs.e01_ + lhs.e31_ * rhs.e11_ + lhs.e32_ * rhs.e21_ + lhs.e33_ * rhs.e31_;
    result.e32_ = lhs.e30_ * rhs.e02_ + lhs.e31_ * rhs.e12_ + lhs.e32_ * rhs.e22_ + lhs.e33_ * rhs.e32_;
    result.e33_ = lhs.e30_ * rhs.e03_ + lhs.e31_ * rhs.e13_ + lhs.e32_ * rhs.e23_ + lhs.e33_ * rhs.e33_;
    return result;
}

Matrix4x4f LookAtRightHand(const Vector4f &from, const Vector4f &to, const Vector4f &up){
    const Vector3f from_temp(from.x_, from.y_, from.z_);
    const Vector3f to_temp(to.x_, to.y_, to.z_);
    const Vector3f up_temp(up.x_, up.y_, up.z_);

    const Vector3f f = (to_temp - from_temp).Normalized();
    const Vector3f s = CrossProduct(f, up_temp).Normalized();
    const Vector3f u = CrossProduct(s, f);

    Matrix4x4f result{
        s.x_, u.x_, -f.x_, -DotProduct(s, from_temp),
        s.y_, u.y_, -f.y_, -DotProduct(u, from_temp),
        s.z_, u.z_, -f.z_, DotProduct(f, from_temp),
        0.0f, 0.0f, 0.0f, 1.0f};
    return result;
}

}
