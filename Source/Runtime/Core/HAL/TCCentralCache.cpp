#include "TCCentralCache.h"
#include "TCGlobals.h"
#include "TCPageCache.h"
#include "TCSizeMap.h"
#include "TCCommon.h"
namespace sablin{

void TCCentralCache::Initialize(){

}

void TCCentralCache::Clear(){
#ifdef DEBUG
    for(uint8_t i = 0;i != kBucketNum; ++i){
        TCSpanList& span_list = span_list_[i];
        ASSERT_WITH_STRING(span_list.IsEmpty(), "TCCentralCache::Clear: TCCentralCache Is Not Clear!")
    }
#endif
}

uint32_t TCCentralCache::RemoveRange(uint8_t bucket_index, void** batch, uint32_t move_num){
    TCSpanList &span_list = span_list_[bucket_index];
    TCSpan* span = nullptr;

    span_list.Lock();
    while(true){
        if(span_list.IsEmpty()){
            break;
        }else{
            TCSpan* begin_span = span_list_->Begin();
            if(begin_span->TryLock()){
                if(begin_span->IsFull()){
                    begin_span->UnLock();
                }else{
                    span = span_list.TryPop();
                }
                break;
            }
        }
    }
    span_list.UnLock();

    if(span == nullptr){
        uintptr_t pages_num = TCGlobals::size_map_.GetPagesNum(bucket_index);
        uint32_t object_size = TCGlobals::size_map_.GetClassToSize(bucket_index);
        span = TCGlobals::page_cache_.AllocateSpan(bucket_index, pages_num);
        span->SetObjectSize(object_size);

        char* start_ptr = static_cast<char*>(span->GetStartAddr());
        const char* end_ptr = static_cast<char*>(span->GetEndAddr());
#ifdef DEBUG
        ASSERT_WITH_STRING(span->GetSpanSize() % object_size == 0, "TCCentralCache::RemoveRange: Span Size Can Not Be Devided By Object Size Perfectly!")
#endif
        while(start_ptr < end_ptr){
            span->Push(start_ptr);
            start_ptr += object_size;
        }
        span->Lock();
    }

    uint32_t result = 0;
    while(result != move_num){
        if(span->IsFull()) break;
        batch[result] = span->Pop();
        ++result;
    }
    
    span_list.Lock();
    if(span->IsFull()){
        span_list.PushBack(span);
    }else{
        span_list.PushFront(span);
    }
    span->UnLock();
    span_list.UnLock();

    return result;
}

void TCCentralCache::InsertRange(uint8_t bucket_index, void** batch, uint32_t move_num){ 
    TCSpanList& span_list = span_list_[bucket_index];
    
    for(uint32_t i = 0;i != move_num; ++i){
        void* ptr = batch[i];
        TCSpan* span = TCGlobals::page_cache_.MapObjectToSpan(ptr);
        span->Lock();
//TODO: Reduce the times of lock and unlock by cache!
        span_list.Lock();
        span_list.Erase(span);
        span->Push(ptr);
        if(span->IsEmpty()){
            TCGlobals::page_cache_.DeallocateSpan(bucket_index, span);
        }else{
            span_list.PushFront(span);
        }
        span_list.UnLock();
        span->UnLock();
    }
}


}
