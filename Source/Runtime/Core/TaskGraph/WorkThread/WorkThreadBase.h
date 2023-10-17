#ifndef WORK_THREAD_BASE_H
#define WORK_THREAD_BASE_H
#include "../Runnable.h"
namespace sablin{

class WorkThreadBase: public Runnable{
protected:
    bool is_done_;
    bool is_init_;
    bool is_running_;

};

}
#endif
