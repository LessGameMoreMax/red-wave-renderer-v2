#include <iostream>
#include "../TCSizeMap.h"

using namespace sablin;
using namespace std;

int main(){
    TCSizeMap *size_map = new TCSizeMap();
    size_map->Initialize();
    for(int i = 1;i != 1154; ++i){
        cout <<i<<" : "<<(uint32_t)size_map->GetSizeToClass(i)<< endl;
    }
    // for(int i = 0;i != kBucketNum; ++i){
    //     cout <<i<<" : "<< size_map->GetClassToSize(i) << endl;
    // }
    delete size_map;
}
