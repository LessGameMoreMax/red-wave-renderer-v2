#include "TCGlobals.h"
#include "TCSizeMap.h"
#include "TCThreadCache.h"
#include "TCCentralCache.h"
#include "TCPageCache.h"
namespace sablin{

TCSizeMap TCGlobals::size_map_;
thread_local TCThreadCache* TCGlobals::thread_local_cache_ = nullptr;
TCCentralCache TCGlobals::central_cache_;
TCPageCache TCGlobals::page_cache_;

bool TCGlobals::Initialize(){
    if(!size_map_.Initialize()) return false;
    page_cache_.Initialize();
    central_cache_.Initialize();
    CreateThreadLocalCache();
    return true;
}

void TCGlobals::Exit(){
    DestroyThreadLocalCache();
    central_cache_.Clear();
    page_cache_.Clear();
    size_map_.Clear();
}

void TCGlobals::CreateThreadLocalCache(){
    thread_local_cache_ = new TCThreadCache();
    thread_local_cache_->Initialize();
}

void TCGlobals::DestroyThreadLocalCache(){
    thread_local_cache_->Clear();
    delete thread_local_cache_;
    thread_local_cache_ = nullptr;
}

}
