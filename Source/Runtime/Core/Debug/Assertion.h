#ifndef ASSERTION_H
#define ASSERTION_H
#include <assert.h>
#include <iostream>

namespace sablin{

#define NO_IMPLEMENT_ASSERT(classname, function_name)\
    std::cout << "NoImplementAssert: " << #classname << "::" <<\
    #function_name << " Doesn't Have Any Implement!" << std::endl;\
    assert(false);

#define ASSERT_WITH_STRING(condition, string)\
    assert(condition && string);

#define ASSERT_NO_STRING(condition)\
    assert(condition);

}
#endif
