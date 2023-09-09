#ifndef MALLOC_ANSI_H
#define MALLOC_ANSI_H
#include "MallocBase.h"

namespace sablin{

class MallocAnsi final: public MallocBase{
private:
    void* AnsiMalloc(std::size_t size, uint32_t alignment);
    void AnsiFree(void* ptr);
public:
    MallocAnsi();
    virtual ~MallocAnsi() override;

    virtual void* Malloc(std::size_t size, uint32_t alignment) override;
    virtual void* TryMalloc(std::size_t size, uint32_t alignment) override;
    virtual void* Realloc(void* ptr, std::size_t new_size, uint32_t alignment) override;
    virtual void* TryRealloc(void* ptr, std::size_t new_size, uint32_t alignment) override;
    virtual void Free(void* ptr) override;
    virtual std::string GetDescriptName() const override;
    virtual bool IsThreadSafe() const override;
    virtual std::size_t GetAllocatedSize(void* ptr) override;
};

}
#endif
