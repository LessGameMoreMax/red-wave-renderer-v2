#ifndef TC_PAGE_CACHE_H
#define TC_PAGE_CACHE_H
#include <cstdint>
#include "TCPageMap.h"
namespace sablin{

class TCSpan;

class TCPageCache{
private:
    TCPageMap page_map_;
public:
    TCPageCache() = default;
    ~TCPageCache() = default;

    void Initialize();
    void Clear();

    TCSpan* AllocateSpan(uint8_t bucket_index, uintptr_t pages_num);
    void DeallocateSpan(uint8_t bucket_index, TCSpan* span);
    TCSpan* MapObjectToSpan(void* object_ptr);
};
}
#endif
