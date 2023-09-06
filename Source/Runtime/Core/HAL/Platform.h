#ifndef PLATFORM_H
#define PLATFORM_H
#include <assert.h>
#include <iostream>
namespace sablin{

class Assert{
public:
    [[noreturn]] static void NoImplementAssert(const std::string& classname, const std::string& function_name){
        std::cout << "NoImplementAssert: " << classname << "::" << function_name << " Doesn't Have Any Implement!" << std::endl;
        assert(false);
    }
};


}

#endif
