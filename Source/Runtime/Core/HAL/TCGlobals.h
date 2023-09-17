#ifndef TC_GLOBALS_H
#define TC_GLOBALS_H
namespace sablin{

class TCGlobals{
public:
    static class TCSizeMap size_map_;
    static thread_local class TCThreadCache thread_local_cache_;
    static class TCCentralCache central_cache_;
public:
    TCGlobals() = default;
    ~TCGlobals() = default;

    static bool Initialize();
    static void Exit();
};

}
#endif
