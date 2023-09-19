#include "TCSpan.h"
#include "TCGlobals.h"
#include "TCSizeMap.h"
namespace sablin{

void TCSpan::Initialize(PageId first_page_id, uintptr_t pages_num){
#ifdef DEBUG
    ASSERT_WITH_STRING(first_page_id > PageId(0), "TCSpan::Initialize: first_page_id <= PageId(0)")
#endif
    first_page_id_ = first_page_id;
    pages_num_ = pages_num;
    allocated_num_ = 0;
    free_list_ = nullptr;
}

void TCSpan::SetObjectSize(std::size_t object_size){
    object_size_ = object_size;
#ifdef DEBUG
    ASSERT_WITH_STRING(GetSpanSize() % object_size_ == 0, "TCSpan::SetObjectSize: Span Size Can Not Divide By Object Size!")
#endif
    max_allocated_num_ = GetSpanSize() / object_size_;
}
}
