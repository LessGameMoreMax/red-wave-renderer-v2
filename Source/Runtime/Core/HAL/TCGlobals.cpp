#include "TCGlobals.h"
#include "TCSizeMap.h"
#include "TCThreadCache.h"
#include "TCCentralCache.h"
#include "TCPageCache.h"
namespace sablin{

TCSizeMap TCGlobals::size_map_;
thread_local TCThreadCache TCGlobals::thread_local_cache_;
TCCentralCache TCGlobals::central_cache_;
TCPageCache TCGlobals::page_cache_;

bool TCGlobals::Initialize(){
    if(!size_map_.Initialize()) return false;
    page_cache_.Initialize();
    central_cache_.Initialize();
    thread_local_cache_.Initialize();
    return true;
}

void TCGlobals::Exit(){
    thread_local_cache_.Clear();
    central_cache_.Clear();
    page_cache_.Clear();
    size_map_.Clear();
}

}
