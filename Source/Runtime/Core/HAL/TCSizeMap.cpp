#include "TCSizeMap.h"
#include "TCSizeClasses.h"
namespace sablin{

bool TCSizeMap::Initialize(){
#ifdef TC_SIZE_MAP_DEBUG
    std::cout << "kClassArraySize: " << kClassArraySize << std::endl;
#endif
    for(uint8_t i = 0;i != kBucketNum; ++i){
        class_to_size_[i] = kSizeClassInfoArray[i].size_;
        move_num_[i] = kSizeClassInfoArray[i].move_num_;
    }

    uint32_t size = 1;
    for(uint8_t i = 0;i != kBucketNum; ++i){
        const uint32_t max_temp_size = class_to_size_[i];
        const uint32_t align = AlignmentForSize(size);
        for(uint32_t temp = size; temp <= max_temp_size; temp += align){
#ifdef TC_SIZE_MAP_DEBUG
        std::cout << "size: " << temp << " IndexOfSizeToClass: " << IndexOfSizeToClass(temp) << std::endl;
#endif
            size_to_class_[IndexOfSizeToClass(temp)] = i;
        }
        size = max_temp_size + align;
    }
    return true;
}

void TCSizeMap::Clear(){

}

}
