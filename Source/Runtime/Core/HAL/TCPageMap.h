#ifndef TC_PAGE_MAP_H
#define TC_PAGE_MAP_H
#include <cstdint>
#include "TCCommon.h"
#include "TCSpan.h"
#include "MallocBase.h"
#include "MemoryBase.h"
namespace sablin{

template <uint32_t BITS>
class TCPageMap2{
private:
    static constexpr uint32_t kLeafBits = 15;
    static constexpr uint64_t kLeafLength = 1 << kLeafBits;
    static constexpr uint32_t kRootBits = BITS - kLeafBits;
    static constexpr uint64_t kRootLength = 1 << kRootBits;

    struct Leaf: public UseSystemMallocForNew{
        TCSpan* span_[kLeafLength];
    };

    Leaf* root_[kRootLength];
public:
    constexpr TCPageMap2(): root_{} {}

    inline void Initialize(){}
    inline void Clear(){
        for(uintptr_t i = 0; i != kRootLength; ++i)
            if(root_[i] != nullptr) delete root_[i];
    }

    inline TCSpan* GetSpan(uintptr_t addr) const{
        const uintptr_t level_1 = addr >> kLeafBits;
        const uintptr_t level_2 = addr & (kLeafLength - 1);
        if((addr >> BITS) > 0) return nullptr;
        if(root_[level_1] == nullptr) return nullptr;
        return root_[level_1]->span_[level_2];
    }

    inline void SetSpan(uintptr_t addr, TCSpan *span){
#ifdef DEBUG
        ASSERT_WITH_STRING(addr >> BITS == 0, "TCPageMap2::SetSpan: Addr Is Not BITS Length!")
#endif
        const uintptr_t level_1 = addr >> kLeafBits;
        const uintptr_t level_2 = addr & (kLeafLength - 1);
        root_[level_1]->span_[level_2] = span;
    }

    bool Ensure(uintptr_t start, std::size_t count){
        for(uintptr_t temp = start; temp <= start + count - 1;){
            const uintptr_t level_1 = temp >> kLeafBits;
            if(level_1 >= kRootLength)[[unlikely]] return false;
            if(root_[level_1] == nullptr)[[likely]]{
                Leaf* leaf = new Leaf();
                if(leaf == nullptr)[[unlikely]] return false;
                MemoryBase::Memzero(leaf, sizeof(*leaf));
                root_[level_1] = leaf;
            }
            temp = ((temp >> kLeafBits) + 1) << kLeafBits;
        }
        return true;
    }
};

class TCPageMap{
private:
    TCPageMap2<kAddressBits - kPageShift> page_map_;
    std::mutex page_map_lock_;
public:
    constexpr TCPageMap(): page_map_(){}

    inline void Initialize(){}

    inline void Clear(){
        page_map_.Clear();
    }

    void SetSpan(PageId page_id, TCSpan* span){
        page_map_lock_.lock();
        page_map_.SetSpan(page_id.GetIndex(), span);
        page_map_lock_.unlock();
    }

    bool Ensure(PageId page_id, std::size_t length){
        page_map_lock_.lock();
        bool result = page_map_.Ensure(page_id.GetIndex(), length);
        page_map_lock_.unlock();
        return result;
    }

    TCSpan* GetSpan(PageId page_id){
        page_map_lock_.lock();
        TCSpan* result = page_map_.GetSpan(page_id.GetIndex());
        page_map_lock_.unlock();
        return result;
    }
};
}
#endif
