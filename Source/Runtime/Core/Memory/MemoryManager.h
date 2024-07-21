#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H
namespace sablin{

class MemoryManager final{
private:
    static void Initialize();
    static void Exit();
public:
    explicit MemoryManager(){
        Initialize();
    }

    ~MemoryManager(){
        Exit();
    }
};

}
#endif
