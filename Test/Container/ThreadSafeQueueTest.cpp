#include "../ThreadSafeQueue.h"
#include <thread>
#include <iostream>
#include <vector>
#include <atomic>
using namespace std;
using namespace sablin;

void Producer(ThreadSafeQueue<int>* queue){
    for(int i = 0;i != 1000; ++i){
        queue->PushBack(i);
        this_thread::sleep_for(chrono::milliseconds((rand() % 10)));
    }
}

void Consumer(ThreadSafeQueue<int>* queue, ThreadSafeQueue<int>* result, atomic<bool>* flag){
    while(flag->load()){
        auto r = queue->PopFront();
        if(r) result->PushBack(r.value());
    }
}

int main(){
    ThreadSafeQueue<int> queue;
    ThreadSafeQueue<int> result;
    atomic<bool> flag{true};

    vector<thread> threads;
    for(int i = 0;i != 3; ++i)
        threads.push_back(thread(Producer, &queue));
    for(int i = 0;i != 3; ++i)
        threads.push_back(thread(Consumer, &queue, &result, &flag));
    for(int i = 0;i != 3; ++i)
        threads[i].join();

    flag = false;
    for(int i = 3;i != 6; ++i)
        threads[i].join();

    vector<int> vec;
    while(true){
        auto r = result.PopFront();
        if(!r.has_value()) break;
        vec.push_back(r.value());
    }
    cout << vec.size() << endl;

    return 0;
}
