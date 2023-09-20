#ifndef TC_PAGE_CACHE_H
#define TC_PAGE_CACHE_H
#include <cstdint>
#include "TCPageMap.h"
#include "TCSpanList.h"
namespace sablin{

class TCSpan;

class TCPageCache{
private:
    TCPageMap page_map_;
    TCSpanList span_list_[kMaxPages + 1];
public:
    TCPageCache() = default;
    ~TCPageCache() = default;

    void Initialize();
    void Clear();

    TCSpan* AllocateSpan(uint8_t bucket_index, uintptr_t pages_num);
    void DeallocateSpan(uint8_t bucket_index, TCSpan* span);
    TCSpan* MapObjectToSpan(void* object_ptr);
    bool GrowHeap(uintptr_t pages_num);
    TCSpan* SearchSpanList(uintptr_t pages_num);
    void MergeIntoSpanList(TCSpan* span);
    TCSpan* Carve(TCSpan* span, uintptr_t pages_num);

    TCSpan* AllocBig(std::size_t size);
    void FreeBig(void* ptr, TCSpan* span);

    TCSpan* AllocLarge(uintptr_t pages_num);
    void RecordSpan(TCSpan* span);

    void AddToSpanList(TCSpan* span);
    void RemoveFromSpanList(TCSpan* span);

    static inline PageId PtrToPageId(const void* ptr){
        return PageId(reinterpret_cast<uintptr_t>(ptr) >> kPageShift);
    }
};
}
#endif
