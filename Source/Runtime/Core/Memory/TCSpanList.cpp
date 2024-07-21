#include "TCSpanList.h"
#include "Debug/Assertion.h"
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
#ifdef DEBUG
    length_ = 0;
#endif
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
#ifdef DEBUG
    length_ = 0;
#endif
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
#ifdef DEBUG
    ++length_;
#endif
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
#ifdef DEBUG
    ++length_;
#endif
}

void TCSpanList::Erase(TCSpan* position){
#ifdef DEBUG
    ASSERT_WITH_STRING(position != nullptr, "TCSpanList::TryErase: Position Is Nullptr!")
#endif
    position->next_->prev_ = position->prev_;
    position->prev_->next_ = position->next_;
#ifdef DEBUG
    position->prev_ = position->next_ = nullptr;
    --length_;
#endif
}

TCSpan* TCSpanList::TryPop(){
    if(IsEmpty())[[unlikely]] return nullptr;
    TCSpan* result = span_head_->next_;
    Erase(result);
    return result;
}

}
