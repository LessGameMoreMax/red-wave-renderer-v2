#ifndef TC_SIZE_MAP_H
#define TC_SIZE_MAP_H
#include <cstdint>
#include "TCCommon.h"
#include "TCSizeClasses.h"
#include "../Debug/Assertion.h"
namespace sablin{

class TCSizeMap{
private:
    static constexpr int32_t kMaxSmallSize = 1024;
    static constexpr std::size_t kClassArraySize = 
            ((kMaxSize + 127 + (56 << 7)) >> 7) + 1;

    uint8_t size_to_class_[kClassArraySize] = {0};
    uint32_t class_to_size_[kBucketNum] = {0};

    static inline uint32_t AlignmentForSize(std::size_t size){
        if(size <= 8) return 8;
        if(size <= kMaxSmallSize) return 1 << TC_ALIGNMENT_SHIFT;
        if(size <= kMaxSize) return 128;
        return kPageSize;
    }
public:
    TCSizeMap() = default;
    ~TCSizeMap() = default;
    bool Initialize();
    void Clear();

    static inline uint16_t IndexOfSizeToClass(std::size_t size){
#ifdef DEBUG
        ASSERT_WITH_STRING(1 <= size, "TCSizeMap::IndexOfSizeToClass: size < 1!")
        ASSERT_WITH_STRING(size <= kMaxSize, "TCSizeMap::IndexOfSizeToClass: size > kMaxSize!")
#endif
        if(size <= 8) return 0;
        if(size <= kMaxSmallSize) return (size + (1 << TC_ALIGNMENT_SHIFT) - 1) >> TC_ALIGNMENT_SHIFT;
        return (size + (1 << 7) - 1 + (56 << 7)) >> 7;
    }

    inline uint8_t GetSizeToClass(std::size_t size) const{
#ifdef DEBUG
        ASSERT_WITH_STRING(size <= kMaxSize, "TCSizeMap::GetSizeToClass: size > kMaxSize!")
#endif
        return size_to_class_[IndexOfSizeToClass(size)];
    }

    inline uint32_t GetClassToSize(uint8_t class_size) const{
#ifdef DEBUG
        ASSERT_WITH_STRING(class_size < kBucketNum, "TCSizeMap::GetClassToSize: class_size >= kBucketNum!")
#endif
        return class_to_size_[class_size];
    }
};

}
#endif
