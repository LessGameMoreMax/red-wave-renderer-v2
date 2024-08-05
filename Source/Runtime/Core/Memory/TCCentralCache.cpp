#include "TCCentralCache.h"
#include "TCGlobals.h"
#include "TCPageCache.h"
#include "TCSizeMap.h"
#include "TCCommon.h"
namespace sablin{

void TCCentralCache::Initialize(){

}

void TCCentralCache::Clear(){
    for(uint8_t i = 0;i != kBucketNum; ++i){
        TCSpanList& span_list = span_list_[i];
        while(!span_list.IsEmpty()){
            TCGlobals::page_cache_.DeallocateSpan(span_list.TryPop());
        }
    }
}

uint32_t TCCentralCache::RemoveRange(uint8_t bucket_index, void** batch, uint32_t move_num){
    TCSpanList &span_list = span_list_[bucket_index];
    TCSpan* span = nullptr;

    span_list.Lock();
    if(!span_list.IsEmpty()){
        TCSpan* begin_span = span_list.Begin();
        if(!begin_span->IsFull())
            span = span_list.TryPop();
    }

    if(span == nullptr){
        uintptr_t pages_num = TCGlobals::size_map_.GetPagesNum(bucket_index);
        uint32_t object_size = TCGlobals::size_map_.GetClassToSize(bucket_index);
        span = TCGlobals::page_cache_.AllocateSpan(pages_num);
        span->SetObjectSize(object_size);

        const char* start_ptr = static_cast<char*>(span->GetStartAddr());
        char* end_ptr = static_cast<char*>(span->GetEndAddr());
        char* first_ptr = end_ptr - span->GetSpanSize() % object_size - object_size;
        while(first_ptr >= start_ptr){
            span->InitializePush(first_ptr);
            first_ptr -= object_size;
        }
    }

    uint32_t result = 0;
    while(result != move_num){
        if(span->IsFull()) break;
        batch[result] = span->Pop();
        ++result;
    }
    
    if(span->IsFull()){
        span_list.PushBack(span);
    }else{
        span_list.PushFront(span);
    }
    span_list.UnLock();

    return result;
}

void TCCentralCache::InsertRange(uint8_t bucket_index, void** batch, uint32_t move_num){ 
    TCSpanList& span_list = span_list_[bucket_index];
    
    span_list.Lock();
    for(uint32_t i = 0;i != move_num; ++i){
        void* ptr = batch[i];
        TCSpan* span = TCGlobals::page_cache_.MapObjectToSpan(ptr);
        span_list.Erase(span);
        span->Push(ptr);
        if(span->IsEmpty()){
            TCGlobals::page_cache_.DeallocateSpan(span);
        }else{
            span_list.PushFront(span);
        }
    }
    span_list.UnLock();
}


}
