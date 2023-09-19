#ifndef TC_CENTRAL_CACHE_H
#define TC_CENTRAL_CACHE_H
#include <cstdint>
#include "TCSpanList.h"
#include "TCSizeClasses.h"
namespace sablin{

class TCCentralCache{
private:
    TCSpanList span_list_[kBucketNum];
public:
    TCCentralCache() = default;
    ~TCCentralCache() = default;

    void Initialize();
    void Clear();

    uint32_t RemoveRange(uint8_t bucket_index, void** batch, uint32_t move_num);
    void InsertRange(uint8_t bucket_index, void** batch, uint32_t move_num);
};
}
#endif
