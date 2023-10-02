#include "TCPageCache.h"
#include "../Debug/Assertion.h"
#include "../Misc/MacroDefine.h"
#include "TCSystemMalloc.h"
namespace sablin{
//TODO: Smaller Lock!

void TCPageCache::Initialize(){

}

void TCPageCache::Clear(){
    page_map_.Clear();
    for(uint32_t i = 0;i != kMaxPages + 1; ++i){
        while(!span_list_[i].IsEmpty()){
            TCSpan* span = span_list_[i].TryPop();
#ifdef DEBUG
            ASSERT_WITH_STRING(span != nullptr, "TCPageCache::Clear: Span Pointer Is Nullptr!")
#endif
            TCSystemMalloc::Release(span->GetStartAddr(), span->GetSpanSize());
        }
    }
}

TCSpan* TCPageCache::AllocateSpan(uintptr_t pages_num){
#ifdef DEBUG
    ASSERT_WITH_STRING(pages_num > 0, "TCPageCache::AllocateSpan: Pages Number Is Smaller One!")
#endif
    page_cache_lock_.lock();
    TCSpan* result = SearchSpanList(pages_num);
    if(result != nullptr) {
        page_cache_lock_.unlock();
        return result;
    }
    if(!GrowHeap(pages_num)){
        page_cache_lock_.unlock();
        return nullptr;
    }
    result = SearchSpanList(pages_num);
    page_cache_lock_.unlock();
    return result;
}

void TCPageCache::DeallocateSpan(TCSpan* span){
    page_cache_lock_.lock();
    // RemoveFromSpanList(span);
    MergeIntoSpanList(span);
    page_cache_lock_.unlock();
}

//TODO: Use MapObjectToSpans To Reduce Function Calls And Speed!
TCSpan* TCPageCache::MapObjectToSpan(void* object_ptr){
    return page_map_.GetSpan(PtrToPageId(object_ptr));
}

bool TCPageCache::GrowHeap(uintptr_t pages_num){
    auto [ptr, actual_size] = TCSystemMalloc::Malloc(pages_num * kPageSize, kPageSize);
    if(ptr == nullptr) return false;
    pages_num = actual_size >> kPageShift;
    PageId page_id = PtrToPageId(ptr);
    if(page_map_.Ensure(page_id-1, pages_num+2))[[likely]]{
        TCSpan* span = new TCSpan();
        span->Initialize(page_id, pages_num);
        MergeIntoSpanList(span);
        return true;
    }else{
        TCSystemMalloc::Release(ptr, actual_size);
        return false;
    }
}

TCSpan* TCPageCache::SearchSpanList(uintptr_t pages_num){
//TODO: Normal and Returned?
    for(uintptr_t i = pages_num-1;i < kMaxPages; ++i){
        TCSpanList& span_list = span_list_[i];
        if(!span_list.IsEmpty()){
            return Carve(span_list.TryPop(), pages_num);
        }
    }
    return AllocLarge(pages_num);
}

void TCPageCache::MergeIntoSpanList(TCSpan* span){
    const PageId page_id = span->GetFirstPageId();
    const uintptr_t pages_num = span->GetPageNum();

    TCSpan* prev = page_map_.GetSpan(page_id - 1);
    if(prev != nullptr && prev->IsEmpty()){
        const uintptr_t len = prev->GetPageNum();
        RemoveFromSpanList(prev);
        delete prev;
        span->SetFirstPageId(span->GetFirstPageId() - len);
        span->SetPageNum(span->GetPageNum() + len);
        page_map_.SetSpan(span->GetFirstPageId(), span);
    }

    TCSpan* next = page_map_.GetSpan(page_id + pages_num);
    if(next != nullptr && next->IsEmpty()){
        const uintptr_t len = next->GetPageNum();
        RemoveFromSpanList(next);
        delete next;
        span->SetPageNum(span->GetPageNum() + len);
        page_map_.SetSpan(span->GetLastPageId(), span);
    }
    RecordSpan(span);
    AddToSpanList(span);
}

TCSpan* TCPageCache::Carve(TCSpan* span, uintptr_t pages_num){
    const int64_t extra = span->GetPageNum() - pages_num;
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
    }
    return extra < 0? nullptr : span;
}

void* TCPageCache::AllocBig(std::size_t size){
    size = RoundUp(size, kPageSize);
    std::size_t page_num = size >> kPageShift;
    TCSpan* result = AllocateSpan(page_num);
    result->SetObjectSize(size);
    result->InitializePush(result->GetStartAddr());
    return result->Pop();
}

void TCPageCache::FreeBig(TCSpan* span){
    DeallocateSpan(span);
}

TCSpan* TCPageCache::AllocLarge(uintptr_t pages_num){
    if(span_list_[kMaxPages].IsEmpty()) return nullptr;
    TCSpan* span = span_list_[kMaxPages].TryPop();
    TCSpan* result = Carve(span, pages_num);
    if(result == nullptr) span_list_[kMaxPages].PushFront(span);
    return result;
}

void TCPageCache::RecordSpan(TCSpan* span){
    for(PageId i = span->GetFirstPageId();i != span->GetLastPageId() + 1; ++i)
        page_map_.SetSpan(i, span);
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
