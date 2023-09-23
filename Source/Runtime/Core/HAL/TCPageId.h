#ifndef TC_PAGE_ID_H
#define TC_PAGE_ID_H
#include <cstdint>
#include "TCCommon.h"
#include "../Debug/Assertion.h"
namespace sablin{

class PageId{
    friend constexpr bool operator<(PageId lhs, PageId rhs);
    friend constexpr bool operator>(PageId lhs, PageId rhs);
    friend constexpr bool operator<=(PageId lhs, PageId rhs);
    friend constexpr bool operator>=(PageId lhs, PageId rhs);
    friend constexpr bool operator==(PageId lhs, PageId rhs);
    friend constexpr bool operator!=(PageId lhs, PageId rhs);
    friend constexpr uintptr_t operator-(PageId lhs, PageId rhs);
private:
    uintptr_t page_id_;
public:
    constexpr PageId(): page_id_(0){}
    constexpr explicit PageId(uintptr_t page_id): page_id_(page_id){}
    constexpr PageId(const PageId& rhs) = default;
    constexpr PageId& operator=(const PageId& rhs) = default;

    void* GetStartAddr() const{
        return reinterpret_cast<void*>(page_id_ << kPageShift);
    }

    inline uintptr_t GetStartUintptr() const {
        return page_id_ << kPageShift;
    }

    inline std::size_t GetIndex() const{
        return page_id_;
    }

    constexpr PageId& operator+=(uintptr_t length){
        page_id_ += length;
        return *this;
    }

    constexpr PageId& operator-=(uintptr_t length){
        page_id_ -= length;
        return *this;
    }
};

inline PageId& operator++(PageId& rhs){
    return rhs += 1;
}

inline constexpr bool operator<(PageId lhs, PageId rhs){
    return lhs.page_id_ < rhs.page_id_;
}

inline constexpr bool operator>(PageId lhs, PageId rhs){
    return lhs.page_id_ > rhs.page_id_;
}

inline constexpr bool operator<=(PageId lhs, PageId rhs){
    return lhs.page_id_ <= rhs.page_id_;
}

inline constexpr bool operator>=(PageId lhs, PageId rhs){
    return lhs.page_id_ >= rhs.page_id_;
}

inline constexpr bool operator==(PageId lhs, PageId rhs){
    return lhs.page_id_ == rhs.page_id_;
}

inline constexpr bool operator!=(PageId lhs, PageId rhs){
    return lhs.page_id_ != rhs.page_id_;
}

inline constexpr PageId operator+(PageId lhs, uintptr_t rhs){
    return lhs += rhs;
}

inline constexpr PageId operator+(uintptr_t lhs, PageId rhs){
    return rhs += lhs;
}

inline constexpr PageId operator-(PageId lhs, uintptr_t rhs){
    return lhs -= rhs;
}

inline constexpr uintptr_t operator-(PageId lhs, PageId rhs){
    return lhs.page_id_ - rhs.page_id_;
}

}
#endif
