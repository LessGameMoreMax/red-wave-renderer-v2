#ifndef TC_PAGE_CACHE_H
#define TC_PAGE_CACHE_H
#include <cstdint>
namespace sablin{

class TCSpan;

class TCPageCache{
private:
public:
    TCPageCache() = default;
    ~TCPageCache() = default;

    void Initialize();
    void Clear();

    TCSpan* AllocateSpan(int8_t bucket_index, uintptr_t pages_num);
    void DeallocateSpan(int8_t bucket_index, TCSpan* span);
    TCSpan* MapObjectToSpan(void* object_ptr);
};
}
#endif
