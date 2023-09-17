#include "TCThreadCache.h"
#include "../Debug/Assertion.h"
#include "TCGlobals.h"
namespace sablin{

void TCThreadCache::TCFreeList::Initialize(){
    length_ = 0;
    max_length_ = 1;
    ptr_header_ = nullptr;
}

std::size_t TCThreadCache::over_thread_cache_size_ = kOverThreadCacheSize;
std::size_t TCThreadCache::unclaimed_cache_size_ = kOverThreadCacheSize;


void TCThreadCache::Initialize(){
#ifdef DEBUG
    ASSERT_WITH_STRING(!has_initialized_, "TCThreadCache::Initialize: Initialize Thread Cache Multi!")
    std::cout << "unclaimed_cache_size_: " << unclaimed_cache_size_ << std::endl;
    has_initialized_ = true;
#endif
    size_ = max_size_ = 0;
//TODO: Implement Steal Cache From Other Thread!

    if(unclaimed_cache_size_ < kStealSize){
        //Just Give The Minimum Size
        max_size_ += kMinThreadCacheSize;
        unclaimed_cache_size_ -= kMinThreadCacheSize;
    }else{
        //Steal Cache From UnclaimedCache
        max_size_ += kStealSize;
        unclaimed_cache_size_ -= kStealSize;
    }

    next_thread_cache_ = last_thread_cache_ = nullptr;

    for(uint8_t i = 0;i != kBucketNum; ++i)
        free_list_[i].Initialize();
}

void TCThreadCache::Clear(){

}

}
