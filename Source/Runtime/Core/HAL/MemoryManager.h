#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H
namespace sablin{

class MemoryManager final{
public:
    static void Initialize();
    static void Exit();
};

class MemoryManagerRAII{
public:
    explicit MemoryManagerRAII(){
        MemoryManager::Initialize();
    }

    ~MemoryManagerRAII(){
        MemoryManager::Exit();
    }

};

}
#endif
