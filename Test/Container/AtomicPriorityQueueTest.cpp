#include <iostream>
#include "../AtomicPriorityQueue.h"
using namespace std;
using namespace sablin;

int main(){
    AtomicPriorityQueue<int*> queue;
    int* a = new int(0);
    int* b = new int(1);
    int* c = new int(2);
    queue.Push(a, 0);
    queue.Push(b, 1);
    queue.Push(c, 2);

    cout << *queue.TryPop() << endl;
    cout << *queue.TryPop() << endl;
    cout << *queue.TryPop() << endl;

    delete a;
    delete b;
    delete c;

}
