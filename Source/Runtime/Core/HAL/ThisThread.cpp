#include "ThisThread.h"
namespace sablin{
thread_local int32_t ThisThread::thread_id_ = -1;
}
