#include "TCSpanList.h"
#include "../Debug/Assertion.h"
namespace sablin{

TCSpanList::TCSpanList(){
    TCSpan* ptr = new TCSpan();
    // ptr->prev_ = ptr;
    // ptr->next_ = ptr;
    ptr->prev_ = nullptr;
    ptr->next_ = nullptr;
    span_head_ = ptr;
}

TCSpanList::~TCSpanList(){
    TCSpan* ptr = span_head_->next_;
    // while(ptr != span_head_){
    //     TCSpan* next = ptr->next_;
    //     delete ptr;
    //     ptr = next;
    // }
    // delete span_head_;
    // span_head_ = nullptr;
    while(ptr != nullptr){
        TCSpan* next = ptr->next_;
        delete ptr;
        ptr = next;
    }
    delete span_head_;
    span_head_ = nullptr;
}

// void TCSpanList::PushFront(TCSpan* span){
// }
//
// void TCSpanList::PushBack(TCSpan* span){
//
// }
//
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
    if(position == span_head_->next_){
        TryPop();
    }else{
        TCSpan* position_next = position->next_;
        if(position_next != nullptr){
            position_next->prev_ = position->prev_;
        }
        position->prev_->next_ = position_next;
    }
}

void TCSpanList::Push(TCSpan* span){
#ifdef DEBUG
    ASSERT_WITH_STRING(span != nullptr, "TCSpanList::Push: Span Is Nullptr!")
#endif
    TCSpan* span_head_next = span_head_->next_;
    if(span_head_next != nullptr){
        span_head_next->prev_ = span;
    }
    span->next_ = span_head_next;
    span_head_->next_ = span;
    span->prev_ = span_head_;
}

TCSpan* TCSpanList::TryPop(){
    TCSpan* span_head_next = span_head_->next_;
    if(span_head_next != nullptr) [[likely]] {
        TCSpan* span_head_next_next = span_head_next->next_;
        if(span_head_next_next != nullptr){
            span_head_next_next->prev_ = span_head_;
        }
        span_head_->next_ = span_head_next_next;
    }
    return span_head_next;
}

}
