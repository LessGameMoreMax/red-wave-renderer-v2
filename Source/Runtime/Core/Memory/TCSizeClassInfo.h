#ifndef TC_SIZE_CLASS_INFO_H
#define TC_SIZE_CLASS_INFO_H
#include <cstdint>
namespace sablin{

struct TCSizeClassInfo{
    std::size_t size_ = 0;
    std::size_t pages_num_ = 0;
    std::size_t move_num_ = 0;
};

}
#endif
