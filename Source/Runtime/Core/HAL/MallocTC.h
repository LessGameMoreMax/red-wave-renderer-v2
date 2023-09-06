#ifndef MALLOC_TC_H
#define MALLOC_TC_H
#include "MallocBase.h"

namespace sablin{

class MallocTC final: public MallocBase{
private:
public:
    MallocTC();
    virtual ~MallocTC() override;

    virtual void* Malloc(std::size_t size, uint32_t alignment) override;
    virtual void* TryMalloc(std::size_t size, uint32_t alignment) override;
    virtual void* Realloc(void* ptr, std::size_t new_size, uint32_t alignment) override;
    virtual void* TryRealloc(void* ptr, std::size_t new_size, uint32_t alignment) override;
    virtual void Free(void* ptr) override;
    virtual std::string GetDescriptName() const override;
    virtual bool IsThreadSafe() const override;
};

}
#endif
