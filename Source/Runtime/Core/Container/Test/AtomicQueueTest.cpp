#include <iostream>
#include "../AtomicQueue.h"
using namespace std;
using namespace sablin;

int main(){
    AtomicQueue<int*> atomic_queue;
    int* a = new int(0);
    int* b = new int(1);
    int* c = new int(2);
    atomic_queue.Push(a);
    atomic_queue.Push(b);
    atomic_queue.Push(c);

    std::vector<int*> result;
    atomic_queue.TryPopBatch(result, 3);
    for(auto p: result){
        cout << *p << endl;
    }
    std::cout << atomic_queue.IsEmpty() << std::endl;

    delete a;
    delete b;
    delete c;
}
