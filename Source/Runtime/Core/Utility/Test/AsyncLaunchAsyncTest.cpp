#include "../Async.h"
#include <algorithm>
#include <future>
#include <iostream>
#include <mutex>
#include <numeric>
#include <string>
#include <vector>
#include "../../HAL/MemoryManager.h"
#include "../../HAL/ThisThread.h"

using namespace sablin;
using namespace std;

std::mutex m;

struct X
{
    void foo(int i, const std::string& str)
    {
        std::lock_guard<std::mutex> lk(m);
        // std::cout << str << ' ' << i << '\n';
        std::cout << "id:" << ThisThread::GetThreadId() << std::endl;
    }

    void bar(const std::string& str)
    {
        std::lock_guard<std::mutex> lk(m);
        std::cout << str << '\n';
    }

    int operator()(int i)
    {
        std::lock_guard<std::mutex> lk(m);
        std::cout << "id:" << ThisThread::GetThreadId() << std::endl;
        return i + 10;
    }
};

int main()
{
    MemoryManager memory_manager;
    X x;
    auto a1 = AsyncLanuchAsync(&X::foo, &x, 42, "Hello");
    auto a3 = AsyncLanuchAsync(X(), 43);
    auto a2 = AsyncLanuchAsync(&X::foo, &x, 42, "Hello");
    auto a4 = AsyncLanuchAsync(&X::foo, &x, 42, "Hello");
    auto a5 = AsyncLanuchAsync(&X::foo, &x, 42, "Hello");
    auto a6 = AsyncLanuchAsync(&X::foo, &x, 42, "Hello");
    auto a7 = AsyncLanuchAsync(&X::foo, &x, 42, "Hello");
    a3.Get();

    this_thread::sleep_for(chrono::seconds(3));
    return 0;
}

