#ifndef TC_CENTRAL_CACHE_H
#define TC_CENTRAL_CACHE_H
#include <cstdint>
namespace sablin{

template<typename T>
class TCSpan;
    
class TCCentralCache{
private:
public:
    TCCentralCache() = default;
    ~TCCentralCache() = default;

    void Initialize();
    void Clear();

    uint32_t RemoveRange(uint8_t bucket_index, void** batch, uint32_t move_num);
    void InsertRange(uint8_t bucket_index, TCSpan<void*> span);
};
}
#endif
