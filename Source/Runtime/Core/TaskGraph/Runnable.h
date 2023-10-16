#ifndef RUNNABLE_H
#define RUNNABLE_H
#include "RStatus.h"
namespace sablin{

class Runnable{
public:
    explicit Runnable() = default;

    virtual RStatus Init(){
        return RStatus();
    }

    virtual RStatus Run() = 0;

    virtual RStatus Stop(){
        return RStatus();
    }

    virtual RStatus Destroy(){
        return RStatus();
    }
    
    virtual ~Runnable() = default;
};
}
#endif
