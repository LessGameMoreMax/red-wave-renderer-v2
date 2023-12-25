#ifndef THREAD_LOCAL_MEMORY_H
#define THREAD_LOCAL_MEMORY_H
#include "../Misc/MacroDefine.h"
#include "../Misc/MacroTools.h"
#ifdef USE_TC_ALLOCTOR
#include "TCGlobals.h"
#endif
namespace sablin{

class ThreadLocalMemory{
public:
    explicit ThreadLocalMemory(){
#ifdef USE_TC_ALLOCTOR
        TCGlobals::CreateThreadLocalCache();
#endif
    }

    CLASS_NO_ALLOWED_COPY(ThreadLocalMemory)

    ~ThreadLocalMemory(){
#ifdef USE_TC_ALLOCTOR
        TCGlobals::DestroyThreadLocalCache();
#endif
    }
};

}
#endif
