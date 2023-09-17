#include "TCGlobals.h"
#include "TCSizeMap.h"
#include "TCThreadCache.h"
namespace sablin{

TCSizeMap TCGlobals::size_map_;
thread_local TCThreadCache TCGlobals::thread_local_cache_;

bool TCGlobals::Initialize(){
    // if(!size_map_.Initialize()) return false;
    return true;
}

void TCGlobals::Exit(){
    size_map_.Clear();
}

}
