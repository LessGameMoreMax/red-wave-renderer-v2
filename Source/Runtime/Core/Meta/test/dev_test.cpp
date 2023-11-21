#include "../ReflStatic.h"
#include <iostream>

using namespace sablin;
using namespace std;

class Node{
public:
    int n_i = 1;
    float n_f = 0.4;

    int Add(int a, std::string b){
        return a;
    }

    int Sub(int a, int b){
        return a - b;
    }

};

class Node2{
public:
    int n_i2 = 12;
    float n_f2 = 0.6;
};

class Child: public Node{
public:
    int c_i = 2;
    float c_f = 0.2;

    int Mul(int a, int b){
        return a * b;
    }

};

class Final: public Child, public Node2{
public:
    int f_i = 3;
    float f_f = 0.5;
};


SR_CLASS(Node)
    SR_FIELDS(n_i, std::tuple{1}, n_f, std::tuple{0.4})
    SR_MEMBER_METHODS(Add, std::tuple{1}, Sub, std::tuple{2})
SR_END()

SR_CLASS(Node2)
    SR_FIELDS(n_i2, std::tuple{}, n_f2, std::tuple{})
SR_END()

SR_CLASS(Child, Node)
    SR_FIELDS(c_i, std::tuple{2}, c_f, std::tuple{0.2})
    SR_MEMBER_METHODS(Mul, std::tuple{})
SR_END()

SR_CLASS(Final, Child, Node2)
    SR_FIELDS(f_i, std::tuple{1}, f_f, std::tuple{2})
SR_END()

int main(){
    // cout << ClassInfo<Node>::get_class_type_name() << endl;
    // cout << ClassInfo<Child>::get_class_type_name() << endl;
    // cout << ClassInfo<ClassInfo<Child>::ParentType>::get_class_type_name() << endl;
    // Node n;
    // cout << std::get<0>(ClassInfo<Node>::_fields_info.fields_tuple_).field_name_ << endl;
    // cout << std::get<1>(ClassInfo<Node>::_fields_info.fields_tuple_).field_name_ << endl;
    // n.*std::get<0>(ClassInfo<Node>::_fields_info.fields_tuple_).pointer_ = 2;
    // cout << n.a << endl;
    // cout << std::get<0>(std::get<0>(ClassInfo<Node>::_fields_info.fields_tuple_).attributes_) << endl;
    // cout << std::get<0>(ClassInfo<Child>::_fields_info.fields_tuple_).get_field_type_name() << endl;
    // cout << std::get<1>(ClassInfo<Child>::_fields_info.fields_tuple_).get_field_type_name() << endl;
    // cout << std::get<0>(ClassInfo<ClassInfo<Child>::ParentType>::_fields_info.fields_tuple_).get_field_type_name() << endl;
    // cout << std::get<1>(ClassInfo<ClassInfo<Child>::ParentType>::_fields_info.fields_tuple_).get_field_type_name() << endl;
    // cout << ClassInfo<Child>::get_field<0>().get_field_type_name() << endl;
    // Node a;
    // cout << ClassInfo<decltype(a)>::get_field<1>().get_field_type_name() << endl;
    // cout << ClassInfo<Node>::get_fields_count() << endl;
    // cout << ClassInfo<Node2>::get_fields_count() << endl;
    // cout << ClassInfo<Child>::get_fields_count() << endl;
    // cout << ClassInfo<Final>::get_fields_count() << endl;

    // cout << ClassInfo<Child>::get_field<0>().field_name_ << endl;
    // cout << ClassInfo<Child>::get_field<1>().field_name_ << endl;
    // cout << ClassInfo<Child>::get_field<2>().field_name_ << endl;
    // cout << ClassInfo<Child>::get_field<3>().field_name_ << endl;
    // cout << ClassInfo<Final>::get_field<0>().field_name_ << endl;
    // cout << ClassInfo<Final>::get_field<1>().field_name_ << endl;
    // cout << ClassInfo<Final>::get_field<2>().field_name_ << endl;
    // cout << ClassInfo<Final>::get_field<3>().field_name_ << endl;
    // cout << ClassInfo<Final>::get_field<4>().field_name_ << endl;
    // cout << ClassInfo<Final>::get_field<5>().field_name_ << endl;
    // cout << ClassInfo<Final>::get_field<6>().field_name_ << endl;
    // cout << ClassInfo<Final>::get_field<7>().field_name_ << endl;
    // cout << ClassInfo<Final>::get_field_by_field_name("x_f") << endl;
    // cout << ClassInfo<Final>::get_field_by_field_name("ff") << endl;
    // cout << ClassInfo<Final>::get_field_by_field_name("if") << endl;
    //
    // cout << ClassInfo<Final>::get_field_by_field_name("n_i") << endl;
    // cout << ClassInfo<Final>::get_field_by_field_name("n_f") << endl;
    //
    // cout << ClassInfo<Final>::get_field_by_field_name("c_i") << endl;
    // cout << ClassInfo<Final>::get_field_by_field_name("c_f") << endl;
    //
    // cout << ClassInfo<Final>::get_field_by_field_name("f_i") << endl;
    // cout << ClassInfo<Final>::get_field_by_field_name("f_f") << endl;
    //
    // cout << ClassInfo<Final>::get_field_by_field_name("n_i2") << endl;
    // cout << ClassInfo<Final>::get_field_by_field_name("n_f2") << endl;
    // decltype(std::get<0>(ClassInfo<Node>::_member_methods_info.methods_tuple_).member_method_wrapper_)::ReturnType a = 1;

    // cout << std::get<0>(std::get<0>(ClassInfo<Node>::_member_methods_info.methods_tuple_).attributes_) << endl;
    // decltype(std::get<0>(ClassInfo<Node>::_member_methods_info.methods_tuple_).member_method_wrapper_)::ParamType<1> a = 1;
    cout << ClassInfo<Child>::get_member_methods_count() << endl;
    // cout << ClassInfo<Child>::get_member_method<0>().member_method_wrapper_.method_name_ << endl;
    // cout << ClassInfo<Child>::get_member_method<1>().member_method_wrapper_.method_name_ << endl;
    // cout << ClassInfo<Child>::get_member_method<2>().member_method_wrapper_.method_name_ << endl;
}
