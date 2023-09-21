#include "TCSpanList.h"
#include "../Debug/Assertion.h"
namespace sablin{

TCSpanList::TCSpanList(){
    TCSpan* ptr = new TCSpan();
    ptr->prev_ = ptr;
    ptr->next_ = ptr;
    span_head_ = ptr;
}

TCSpanList::~TCSpanList(){
    TCSpan* ptr = span_head_->next_;
    while(ptr != span_head_){
        TCSpan* next = ptr->next_;
        delete ptr;
        ptr = next;
    }
    delete span_head_;
    span_head_ = nullptr;
}

void TCSpanList::Clear(){
    TCSpan* ptr = span_head_->next_;
    while(ptr != span_head_){
        TCSpan* next = ptr->next_;
        delete ptr;
        ptr = next;
    }
    delete span_head_;
    span_head_ = nullptr;
}

void TCSpanList::PushFront(TCSpan* span){
#ifdef DEBUG
    ASSERT_WITH_STRING(span != nullptr, "TCSpanList::PushFront: Span Is Nullptr!")
#endif
    TCSpan* span_head_next = span_head_->next_;
    span_head_next->prev_ = span;
    span->next_ = span_head_next;
    span_head_->next_ = span;
    span->prev_ = span_head_;
}

void TCSpanList::PushBack(TCSpan* span){
#ifdef DEBUG
    ASSERT_WITH_STRING(span != nullptr, "TCSpanList::PushBack: Span Is Nullptr!")
#endif
    TCSpan* span_head_prev = span_head_->prev_;
    span_head_prev->next_ = span;
    span->prev_ = span_head_prev;
    span_head_->prev_ = span;
    span->next_ = span_head_;
}

// TCSpan* TCSpanList::TryPopFront(){
//
// }
//
// TCSpan* TCSpanList::TryPopBack(){
//
// }
//
// void TCSpanList::Insert(TCSpan* position, TCSpan* new_span){
//
// }

void TCSpanList::Erase(TCSpan* position){
#ifdef DEBUG
    ASSERT_WITH_STRING(position != nullptr, "TCSpanList::TryErase: Position Is Nullptr!")
#endif
    position->next_->prev_ = position->prev_;
    position->prev_->next_ = position->next_;
}

TCSpan* TCSpanList::TryPop(){
    if(IsEmpty())[[unlikely]] return nullptr;
    TCSpan* result = span_head_->next_;
    Erase(result);
    return result;
}

}
