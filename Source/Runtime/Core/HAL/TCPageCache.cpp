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

TCSpan* TCPageCache::MapObjectToSpan(void* object_ptr){
    return nullptr;
}

}
