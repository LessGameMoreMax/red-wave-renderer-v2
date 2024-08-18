#ifndef TIME_H
#define TIME_H
#include <chrono>
namespace sablin{

inline long GetCurrentMs(){
    return std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
}

inline double GetCurrentAccurateMs(){
    return (double)std::chrono::time_point_cast<std::chrono::milliseconds> \
        (std::chrono::steady_clock::now()).time_since_epoch().count() / (double)1000.0;
}

}
#endif
