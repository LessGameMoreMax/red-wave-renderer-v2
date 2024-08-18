#include "Math/Vector.h"
#include "Math/Matrix.h"
#include <iostream>
using namespace std;
using namespace sablin;

int main(){
    Vector3f a{1.0f, 2.0f, 3.0f};
    Vector3f b{2.0f, 1.0f, 3.0f};
    Vector3i c{1, 2, 3};
    cout << DotProduct(a, c) << endl;
    Matrix3x3f d{
        10.0f, 1.0f, 3.0f,
        1.0f, 6.0f, 3.0f,
        1.0f, 10.0f, 3.0f
    };
    Vector3f e = a * d;
    Vector3f f = c * d;
    cout << e.x_ << " " << e.y_ << " " << e.z_ << " " << endl;
    cout << f.x_ << " " << f.y_ << " " << f.z_ << " " << endl;
    cout << d.Det() << endl;
    d.Transposed();
    cout << d.e12_ << endl;
}
