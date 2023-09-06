#ifndef MALLOC_BASE_H
#define MALLOC_BASE_H
#include <cstddef>
#include <cstdint>
#include <string>

namespace sablin{

class UseSystemMallocForNew{
private:
public:
    void* operator new(std::size_t size);
    void* operator new[](std::size_t size);
    void operator delete(void* ptr);
    void operator delete[](void* ptr);
};

class MallocBase : public UseSystemMallocForNew{
private:
public:
    MallocBase() = default;
    virtual ~MallocBase() = default;
    virtual void* Malloc(std::size_t size, uint32_t alignment) = 0;
    virtual void* TryMalloc(std::size_t size, uint32_t alignment);
    virtual void* Realloc(void* ptr, std::size_t new_size, uint32_t alignment);
    virtual void* TryRealloc(void* ptr, std::size_t new_size, uint32_t alignment);
    virtual void Free(void* ptr) = 0;
    virtual std::string GetDescriptName() const;
    virtual bool IsThreadSafe() const = 0;
};

}
#endif
