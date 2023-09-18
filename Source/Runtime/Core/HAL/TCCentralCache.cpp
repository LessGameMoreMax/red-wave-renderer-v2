#include "TCCentralCache.h"
#include "TCSpan.h"
namespace sablin{

void TCCentralCache::Initialize(){

}

void TCCentralCache::Clear(){

}

uint32_t TCCentralCache::RemoveRange(uint8_t bucket_index, void** batch, uint32_t move_num){
    return 0;
}

void TCCentralCache::InsertRange(uint8_t bucket_index, TCSpan<void*> span){

}


}
