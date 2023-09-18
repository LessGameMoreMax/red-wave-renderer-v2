#include <iostream>
#include "../TCGlobals.h"
#include "../TCThreadCache.h"
#include "../TCSpanList.h"

using namespace sablin;
using namespace std;

int main(){
    TCGlobals::Initialize();
    TCGlobals::thread_local_cache_.Initialize();
    TCSpanList span_list;
    TCSpan* span1 = new TCSpan();
    TCSpan* span2 = new TCSpan();
    cout << span1 << endl;
    cout << span2 << endl;
    span_list.Push(span1);
    span_list.Push(span2);
    span_list.Erase(span1);
    cout << span_list.TryPop() << endl;
    TCGlobals::thread_local_cache_.Clear();
    TCGlobals::Exit();
}
