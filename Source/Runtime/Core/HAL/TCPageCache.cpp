#include "TCPageCache.h"
#include "../Debug/Assertion.h"
#include "../Misc/MacroDefine.h"
namespace sablin{

void TCPageCache::Initialize(){

}

void TCPageCache::Clear(){
    page_map_.Clear();
}

TCSpan* TCPageCache::AllocateSpan(uint8_t bucket_index, uintptr_t pages_num){
#ifdef DEBUG
    ASSERT_WITH_STRING(pages_num > 0, "TCPageCache::AllocateSpan: Pages Number Is Smaller One!")
#endif
    return nullptr;
}

void TCPageCache::DeallocateSpan(uint8_t bucket_index, TCSpan* span){

}

//TODO: Use MapObjectToSpans To Reduce Function Calls And Speed!
TCSpan* TCPageCache::MapObjectToSpan(void* object_ptr){
    TCSpan* result = page_map_.GetSpan(PtrToPageId(object_ptr));
#ifdef DEBUG
    ASSERT_WITH_STRING(result != nullptr, "TCPageCache::MapObjectToSpan: Object Ptr TCSpan Is Nullptr!")
#endif
    return result;
}

bool TCPageCache::GrowHeap(uintptr_t pages_num){

}

TCSpan* TCPageCache::SearchSpanList(uintptr_t pages_num){
//TODO: Normal and Returned?
    for(uintptr_t i = pages_num-1;i != kMaxPages; ++i){
        TCSpanList& span_list = span_list_[i];
        if(!span_list.IsEmpty()) 
            return Carve(span_list.TryPop(), pages_num);
    }
    return AllocLarge(pages_num);
}

void TCPageCache::MergeIntoSpanList(TCSpan* span){
    const PageId page_id = span->GetFirstPageId();
    const uintptr_t pages_num = span->GetPageNum();
    TCSpan* prev = page_map_.GetSpan(page_id - 1);
    if(prev != nullptr){
        const uintptr_t len = prev->GetPageNum();
        RemoveFromSpanList(prev);
        delete prev;
        span->SetFirstPageId(span->GetFirstPageId() - len);
        span->SetPageNum(span->GetPageNum() + len);
        page_map_.SetSpan(span->GetFirstPageId(), span);
    }

    TCSpan* next = page_map_.GetSpan(page_id + pages_num);
    if(next != nullptr){
        const uintptr_t len = next->GetPageNum();
        RemoveFromSpanList(next);
        delete next;
        span->SetPageNum(span->GetPageNum() + len);
        page_map_.SetSpan(span->GetLastPageId(), span);
    }
    AddToSpanList(span);
}

TCSpan* TCPageCache::Carve(TCSpan* span, uintptr_t pages_num){
    const uintptr_t extra = span->GetPageNum() - pages_num;
    if(extra > 0){        
        TCSpan* left_over = new TCSpan();
        if(page_map_.GetSpan(span->GetFirstPageId()-1) == nullptr && page_map_.GetSpan(span->GetLastPageId()+1) != nullptr){
            left_over->Initialize(span->GetFirstPageId(), extra);
            span->SetFirstPageId(span->GetFirstPageId() + extra);
            page_map_.SetSpan(span->GetFirstPageId(), span);
        }else{
            left_over->Initialize(span->GetFirstPageId() + pages_num, extra);
        }
        RecordSpan(left_over);
        AddToSpanList(left_over);
        span->SetPageNum(pages_num);
        page_map_.SetSpan(span->GetLastPageId(), span);
    }
    return span;
}

TCSpan* TCPageCache::AllocBig(std::size_t size){

}

void TCPageCache::FreeBig(void* ptr, TCSpan* span){

}

TCSpan* TCPageCache::AllocLarge(uintptr_t pages_num){
    if(span_list_[kMaxPages].IsEmpty()) return nullptr;
    return Carve(span_list_[kMaxPages].TryPop(), pages_num);
}

void TCPageCache::RecordSpan(TCSpan* span){
    page_map_.SetSpan(span->GetFirstPageId(), span);
    if(span->GetPageNum() > 1)
        page_map_.SetSpan(span->GetLastPageId(), span);
}

void TCPageCache::AddToSpanList(TCSpan* span){
    uintptr_t pages_num = span->GetPageNum();
    uintptr_t index = (pages_num > kMaxPages? kMaxPages: pages_num-1);
    span_list_[index].PushFront(span);
}

void TCPageCache::RemoveFromSpanList(TCSpan* span){
    uintptr_t pages_num = span->GetPageNum();
    uintptr_t index = (pages_num > kMaxPages? kMaxPages: pages_num-1);
    span_list_[index].Erase(span);
}

}
