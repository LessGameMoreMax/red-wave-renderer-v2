#include "TCPageMap.h"
namespace sablin{

template <uint32_t BITS>
bool TCPageMap2<BITS>::Ensure(uintptr_t start, std::size_t count){
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

}
