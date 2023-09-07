#ifndef MACRO_TOOLS_H
#define MACRO_TOOLS_H
namespace sablin{

#define MACRO_TO_STRING(s) MACRO_TO_STRING_INNER(s)
#define MACRO_TO_STRING_INNER(s) #s

#define MACRO_LINK(x, y) MACRO_LINK_INNER(x, y)
#define MACRO_LINK_INNER(x, y) x##y

}
#endif
