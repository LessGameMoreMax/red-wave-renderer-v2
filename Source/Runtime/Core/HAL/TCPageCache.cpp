#include "TCPageCache.h"
#include "../Debug/Assertion.h"
#include "../Misc/MacroDefine.h"
namespace sablin{

TCSpan* TCPageCache::AllocateSpan(int8_t bucket_index, uintptr_t pages_num){
#ifdef DEBUG
    ASSERT_WITH_STRING(pages_num > 0, "TCPageCache::AllocateSpan: Pages Number Is Smaller One!")
#endif
}
}
