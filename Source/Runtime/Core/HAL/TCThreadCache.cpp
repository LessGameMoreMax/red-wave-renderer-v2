#include "TCThreadCache.h"
#include "../Debug/Assertion.h"
#include "TCGlobals.h"
#include "TCSizeMap.h"
#include "../Math/MathTools.h"
namespace sablin{

void TCThreadCache::TCFreeList::Initialize(){
    length_ = 0;
    max_length_ = 1;
    ptr_header_ = nullptr;
    overages_length_ = 0;
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
//TODO: Lock Lock unclaimed_cache_size_!
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

void* TCThreadCache::Allocate(std::size_t size){
    const uint8_t bucket_index = TCGlobals::size_map_.GetSizeToClass(size);
    std::size_t allocated_size = TCGlobals::size_map_.GetClassToSize(bucket_index);
    TCFreeList* free_list = free_list_ + bucket_index;
    if(!free_list->IsEmpty()) {
        size -= allocated_size;
        return free_list->Pop();
    }
    return FetchFromCentralCache(bucket_index, allocated_size);
}

void TCThreadCache::Deallocate(void* ptr, std::size_t size){
//TODO: What the size represent?
    size_ += size;
    const uint8_t bucket_index = TCGlobals::size_map_.GetSizeToClass(size);
    TCFreeList* free_list = free_list_ + bucket_index;
    free_list->Push(ptr);
    if(free_list_->GetLength() >= free_list_->GetMaxLength()){
        ListTooLong(free_list, bucket_index);
    }
//TODO: Implement Scavenge!
    // if(size_ >= max_size_){
    //     Scavenge();
    // }
}

void* TCThreadCache::FetchFromCentralCache(uint8_t bucket_index, std::size_t size){
    TCFreeList* free_list = free_list_ + bucket_index;
#ifdef DEBUG
    ASSERT_WITH_STRING(free_list->IsEmpty(), "TCThreadCache::FetchFromCentralCache: free_list Is Not Empty!")
#endif
    const int32_t batch_size = TCGlobals::size_map_.GetMoveNum(bucket_index);
    const int32_t move_num = Min<uint32_t>(batch_size, free_list->GetMaxLength());

    void* batch[kMaxMoveNum];
    int32_t moved_num = TCGlobals::central_cache_.RemoveRange(bucket_index, batch, move_num);
    if(moved_num == 0) return nullptr;
    --moved_num;
    if(moved_num > 0){
        size_ += size * moved_num;
        free_list->PushBatch(moved_num, batch + 1);
    }

    if(free_list->GetMaxLength() < batch_size){
        free_list->SetMaxLength(free_list->GetMaxLength() + 1);
    }else{
        int32_t new_length = Min<int32_t>(free_list_->GetMaxLength() + batch_size, kMaxDynamicFreeListLength);
        new_length -= new_length % batch_size;
        free_list->SetMaxLength(new_length);
    }
    return batch[0];
}

void TCThreadCache::ListTooLong(TCFreeList* free_list, uint8_t bucket_num){
    const int32_t move_num = TCGlobals::size_map_.GetMoveNum(bucket_num);
    ReleaseToCentralCache(free_list, bucket_num, move_num);

    uint32_t list_max_length = free_list_->GetMaxLength();
    if(list_max_length < move_num){
        free_list->SetMaxLength(list_max_length + 1);
    }else if(list_max_length > move_num){
        free_list->SetOveragesLength(free_list_->GetOveragesLength() + 1);
        if(free_list_->GetOveragesLength() > kMaxOverages){
            free_list_->SetMaxLength(free_list_->GetMaxLength() - move_num);
            free_list_->SetOveragesLength(0);
        }
    }
}

void TCThreadCache::ReleaseToCentralCache(TCFreeList* free_list, uint8_t bucket_num, uint8_t move_num){

}



}
