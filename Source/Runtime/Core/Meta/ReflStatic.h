#ifndef REFL_STATIC_H
#define REFL_STATIC_H
#include "../Misc/MacroTools.h"
#include <tuple>
#include <type_traits>
#include <functional>
namespace sablin{

#define _FIELD_CONSTRUCT(FIELD, ATTR) lenin::_Field{lenin::_TypeContainer<decay_t<decltype(ClassType::FIELD)>>{}, MACRO_TO_STRING(FIELD), &ClassType::FIELD, ATTR} 

#define _SPEARD_FIELD_2(FIELD, ATTR)       _FIELD_CONSTRUCT(FIELD, ATTR)
#define _SPEARD_FIELD_4(FIELD, ATTR, ...)  _FIELD_CONSTRUCT(FIELD, ATTR), _SPEARD_FIELD_2(__VA_ARGS__)
#define _SPEARD_FIELD_6(FIELD, ATTR, ...)  _FIELD_CONSTRUCT(FIELD, ATTR), _SPEARD_FIELD_4(__VA_ARGS__)
#define _SPEARD_FIELD_8(FIELD, ATTR, ...)  _FIELD_CONSTRUCT(FIELD, ATTR), _SPEARD_FIELD_6(__VA_ARGS__)
#define _SPEARD_FIELD_10(FIELD, ATTR, ...) _FIELD_CONSTRUCT(FIELD, ATTR), _SPEARD_FIELD_8(__VA_ARGS__)
#define _SPEARD_FIELD_12(FIELD, ATTR, ...) _FIELD_CONSTRUCT(FIELD, ATTR), _SPEARD_FIELD_10(__VA_ARGS__)
#define _SPEARD_FIELD_14(FIELD, ATTR, ...) _FIELD_CONSTRUCT(FIELD, ATTR), _SPEARD_FIELD_12(__VA_ARGS__)
#define _SPEARD_FIELD_16(FIELD, ATTR, ...) _FIELD_CONSTRUCT(FIELD, ATTR), _SPEARD_FIELD_14(__VA_ARGS__)

    
#define _MEMBER_METHOD_CONSTRUCT(METHOD, ATTR) lenin::_MemberMethod{lenin::_MemberMethodWrapper{MACRO_TO_STRING(METHOD), &ClassType::METHOD}, ATTR}

#define _SPEARD_MEMBER_METHOD_2(METHOD, ATTR)       _MEMBER_METHOD_CONSTRUCT(METHOD, ATTR)
#define _SPEARD_MEMBER_METHOD_4(METHOD, ATTR, ...)  _MEMBER_METHOD_CONSTRUCT(METHOD, ATTR), _SPEARD_MEMBER_METHOD_2(__VA_ARGS__)
#define _SPEARD_MEMBER_METHOD_6(METHOD, ATTR, ...)  _MEMBER_METHOD_CONSTRUCT(METHOD, ATTR), _SPEARD_MEMBER_METHOD_4(__VA_ARGS__)
#define _SPEARD_MEMBER_METHOD_8(METHOD, ATTR, ...)  _MEMBER_METHOD_CONSTRUCT(METHOD, ATTR), _SPEARD_MEMBER_METHOD_6(__VA_ARGS__)
#define _SPEARD_MEMBER_METHOD_10(METHOD, ATTR, ...) _MEMBER_METHOD_CONSTRUCT(METHOD, ATTR), _SPEARD_MEMBER_METHOD_8(__VA_ARGS__)
#define _SPEARD_MEMBER_METHOD_12(METHOD, ATTR, ...) _MEMBER_METHOD_CONSTRUCT(METHOD, ATTR), _SPEARD_MEMBER_METHOD_10(__VA_ARGS__)
#define _SPEARD_MEMBER_METHOD_14(METHOD, ATTR, ...) _MEMBER_METHOD_CONSTRUCT(METHOD, ATTR), _SPEARD_MEMBER_METHOD_12(__VA_ARGS__)
#define _SPEARD_MEMBER_METHOD_16(METHOD, ATTR, ...) _MEMBER_METHOD_CONSTRUCT(METHOD, ATTR), _SPEARD_MEMBER_METHOD_14(__VA_ARGS__)

#define _SPEARD_CLASS_1(CLASS) \
namespace sablin{                                                  \
    template<>                                                     \
    struct ClassInfo<CLASS>: public lenin::_ClassInfoBase<CLASS>{  \
        friend class HasParent1<CLASS>;                            \
        friend class HasParent2<CLASS>;                            \
        using ClassType = CLASS;                                   \
    private:                                                       \
        using ParentType1 = NoneType;                              \
        using ParentType2 = NoneType;                              \
    public:                                                        \

#define _SPEARD_CLASS_2(CLASS, PARENT) \
namespace sablin{                                                  \
    template<>                                                     \
    struct ClassInfo<CLASS>: public lenin::_ClassInfoBase<CLASS>{  \
        friend class HasParent1<CLASS>;                            \
        friend class HasParent2<CLASS>;                            \
        using ClassType = CLASS;                                   \
        using ParentType1 = PARENT;                                \
    private:                                                       \
        using ParentType2 = NoneType;                              \
    public:                                                        \

#define _SPEARD_CLASS_3(CLASS, PARENT1, PARENT2) \
namespace sablin{                                                  \
    template<>                                                     \
    struct ClassInfo<CLASS>: public lenin::_ClassInfoBase<CLASS>{  \
        friend class HasParent1<CLASS>;                            \
        friend class HasParent2<CLASS>;                            \
        using ClassType = CLASS;                                   \
        using ParentType1 = PARENT1;                               \
        using ParentType2 = PARENT2;                               \

#define SR_CLASS(...) \
    MACRO_LINK(_SPEARD_CLASS_, ARG_COUNT(__VA_ARGS__))(__VA_ARGS__) \

#define SR_END() \
    };   \
}        \

#define SR_FIELDS(...) \
    inline static constexpr const lenin::_FieldsInfo _fields_info{                              \
        MACRO_LINK(_SPEARD_FIELD_, ARG_COUNT(__VA_ARGS__))(__VA_ARGS__)};                       \
    inline static constexpr const size_t get_fields_count(){                                    \
        size_t size = std::tuple_size<decltype(_fields_info.fields_tuple_)>::value;             \
        if constexpr(HasParent1<ClassType>::Value){                                             \
            size += ClassInfo<ParentType1>::get_fields_count();                                 \
        }                                                                                       \
        if constexpr(HasParent2<ClassType>::Value){                                             \
            size += ClassInfo<ParentType2>::get_fields_count();                                 \
        }                                                                                       \
        return size;                                                                            \
    }                                                                                           \
    template<int32_t index>                                                                     \
    inline static constexpr const auto& get_field(){                                            \
        constexpr int32_t size = std::tuple_size<decltype(_fields_info.fields_tuple_)>::value;  \
        if constexpr(index < size){                                                             \
            return std::get<index>(_fields_info.fields_tuple_);                                 \
        }else{                                                                                  \
            constexpr int32_t index1 = index - size;                                            \
            constexpr int32_t size1 = ClassInfo<ParentType1>::get_fields_count();               \
            if constexpr(index1 < size1){                                                       \
                return ClassInfo<ParentType1>::get_field<index1>();                             \
            }else{                                                                              \
                return ClassInfo<ParentType2>::get_field<index1 - size1>();                     \
            }                                                                                   \
        }                                                                                       \
    }                                                                                           \
    inline static constexpr int32_t get_field_by_field_name(const std::string_view field_name){ \
        size_t result_index = -1;                                                               \
        auto f = [&](const size_t index, const std::string_view& name){                         \
            if(field_name.compare(name) == 0) result_index = index;                             \
        };                                                                                      \
        [&]<size_t... I>(std::index_sequence<I...>){                                            \
            ((f(I, get_field<I>().field_name_)), ...);                                          \
        }(std::make_index_sequence<get_fields_count()>{});                                      \
        return result_index;                                                                    \
    }                                                                                           \

#define SR_MEMBER_METHODS(...) \
    inline static constexpr const lenin::_MemberMethodsInfo _member_methods_info{                      \
        MACRO_LINK(_SPEARD_MEMBER_METHOD_, ARG_COUNT(__VA_ARGS__))(__VA_ARGS__)};                      \
    inline static constexpr const size_t get_member_methods_count(){                                   \
        size_t size = std::tuple_size<decltype(_member_methods_info.methods_tuple_)>::value;           \
        if constexpr(HasParent1<ClassType>::Value){                                                    \
            size += ClassInfo<ParentType1>::get_member_methods_count();                                \
        }                                                                                              \
        if constexpr(HasParent2<ClassType>::Value){                                                    \
            size += ClassInfo<ParentType2>::get_member_methods_count();                                \
        }                                                                                              \
        return size;                                                                                   \
    }                                                                                                  \
    template<int32_t index>                                                                            \
    inline static constexpr const auto& get_member_method(){                                           \
        constexpr int32_t size = std::tuple_size<decltype(_member_methods_info.methods_tuple_)>::value; \
        if constexpr(index < size){                                                                    \
            return std::get<index>(_member_methods_info.methods_tuple_);                               \
        }else{                                                                                         \
            constexpr int32_t index1 = index - size;                                                   \
            constexpr int32_t size1 = ClassInfo<ParentType1>::get_member_methods_count();              \
            if constexpr(index1 < size1){                                                              \
                return ClassInfo<ParentType1>::get_member_method<index1>();                            \
            }else{                                                                                     \
                return ClassInfo<ParentType2>::get_member_method<index1 - size1>();                    \
            }                                                                                          \
        }                                                                                              \
    }                                                                                                  \

namespace lenin{

template<typename T>
struct _ClassInfoBase{
    inline static constexpr const std::string_view get_class_type_name(){
        std::string_view type_name;
#ifdef __clang__
        type_name = __PRETTY_FUNCTION__;
        auto head = type_name.find("T = ") + 4;
        auto tail = type_name.find_last_of(';');
#elif defined(__GNUC__)
        type_name = __PRETTY_FUNCTION__;
        auto head = type_name.find("T = ") + 4;
        auto tail = type_name.find_last_of(';');
#elif defined(_MSC_VER)
        type_name = __FUNCSIG__;
        auto head = type_name.find("ClassInfoBase<") + 11;
        auto tail = type_name.find_last_of('>');
#endif
        return std::string_view{type_name.data() + head, tail - head};
    }
};

template<typename T>
struct _TypeContainer{ using Type = T; };

template<typename P, typename M, typename... Args>
struct _Field{
    using MemberType = M;
    std::string_view field_name_;
    P pointer_;
    std::tuple<Args...> attributes_;

    explicit constexpr _Field(_TypeContainer<M>, const std::string_view field_name, P pointer, const std::tuple<Args...>& attributes):
        field_name_(field_name),
        pointer_(pointer),
        attributes_(attributes){}

    inline constexpr const std::string_view get_field_type_name() const{
        std::string_view type_name;
#ifdef __clang__
        type_name = __PRETTY_FUNCTION__;
        auto head = type_name.find("M = ") + 4;
        auto tail = type_name.find_last_of(';');
#elif defined(__GNUC__)
        type_name = __PRETTY_FUNCTION__;
        auto head = type_name.find("M = ") + 4;
        auto tail = type_name.find_last_of(';');
#endif
        std::string_view temp{std::string_view{type_name.data() + head, tail - head}};
        return std::string_view{temp.data(), temp.find_last_of(';')};
    }
};

template<typename... Args>
struct _FieldsInfo{
    std::tuple<Args...> fields_tuple_;

    explicit constexpr _FieldsInfo(Args... args):
        fields_tuple_(std::forward<Args>(args)...){}
};

template<typename T, typename R, typename... Params>
struct _MemberMethodWrapper{
    using MemberMethodType = R(T::*)(Params...);
    using ReturnType = R;
    template<size_t Index>
    using ParamType = std::tuple_element_t<Index, std::tuple<Params...>>;

    std::string_view method_name_;
    MemberMethodType member_method_;

    explicit constexpr _MemberMethodWrapper(const std::string_view method_name, MemberMethodType member_method):
        method_name_(method_name),
        member_method_(member_method){}
};

template<typename W, typename... Args>
struct _MemberMethod{
    W member_method_wrapper_;
    std::tuple<Args...> attributes_;

    explicit constexpr _MemberMethod(W member_method_wrapper, const std::tuple<Args...>& attributes):
        member_method_wrapper_(member_method_wrapper),
        attributes_(attributes){}
};

template<typename... Args>
struct _MemberMethodsInfo{
    std::tuple<Args...> methods_tuple_;

    explicit constexpr _MemberMethodsInfo(Args... args):
        methods_tuple_(std::forward<Args>(args)...){}
};

}

template<typename T>
struct ClassInfo: public lenin::_ClassInfoBase<T>{};

struct NoneType{
    int _none = 0;
    int Add(int a, int b){
        return a + b;
    }
};

template<>
struct ClassInfo<NoneType>: public lenin::_ClassInfoBase<NoneType>{
    inline static constexpr const size_t get_fields_count(){ return 0;}

    template<int32_t index>
    inline static constexpr const auto& get_field(){
        return lenin::_Field{lenin::_TypeContainer<int>{}, "_none", &NoneType::_none, std::tuple{}};
    }

    inline static constexpr const size_t get_member_methods_count(){ return 0;}

    template<int32_t index>
    inline static constexpr const auto& get_member_method(){
        return lenin::_MemberMethod{lenin::_MemberMethodWrapper{"Add", &NoneType::Add}, std::tuple{}};
    }
};

template<typename T>
struct HasParent1{
    inline static constexpr const bool Value = !std::is_same_v<typename ClassInfo<T>::ParentType1, NoneType>;
};

template<typename T>
struct HasParent2{
    inline static constexpr const bool Value = !std::is_same_v<typename ClassInfo<T>::ParentType2, NoneType>;
};


}
#endif
