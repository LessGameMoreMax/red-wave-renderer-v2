#ifndef TC_THREAD_CACHE_H
#define TC_THREAD_CACHE_H
#include "TCCommon.h"
#include "TCSizeClasses.h"
#include "Debug/Assertion.h"
#include "MallocBase.h"
namespace sablin{

class TCThreadCache: public UseSystemMallocForNew{
private:
    class TCFreeList{
        private:
            void* ptr_header_;
            uint32_t length_;
            uint32_t max_length_ ;
            uint32_t overages_length_;
        private:
            inline static void* GetNextPtr(void* ptr){
                return *(reinterpret_cast<void**>(ptr));
            }

            inline static void SetNextPtr(void* ptr, void* next_ptr){
                *(reinterpret_cast<void**>(ptr)) = next_ptr;
            }
        public:
            TCFreeList() = default;
            ~TCFreeList() = default;

            void Initialize();

            inline uint32_t GetLength() const{
                return length_;
            }

            inline uint32_t GetMaxLength() const{
                return max_length_;
            }

            inline void SetMaxLength(const uint32_t max_length){
                max_length_ = max_length;
            }

            inline uint32_t GetOveragesLength() const{
                return overages_length_;
            }

            inline void SetOveragesLength(uint32_t new_length){
                overages_length_ = new_length;
            }

            inline bool IsEmpty() const{
                return ptr_header_ == nullptr;
            }

            inline void Push(void* ptr){
                SetNextPtr(ptr, ptr_header_);
                ptr_header_ = ptr;
                ++length_;
            }

            inline void* Pop(){
                void* result = ptr_header_;
                ptr_header_ = GetNextPtr(ptr_header_);
                --length_;
                return result;
            }

            void PushBatch(int32_t count, void** batch){
#ifdef DEBUG
                ASSERT_WITH_STRING(count > 0, "TCThreadCache::TCFreeList::PushBatch: count <= 0!")
#endif
                for(int32_t i = 0;i != count-1; ++i){
                    SetNextPtr(batch[i], batch[i+1]);
                }
                SetNextPtr(batch[count-1], ptr_header_);
                ptr_header_ = batch[0];
                length_ += count;
            }

            void PopBatch(int32_t count, void** batch){
#ifdef DEBUG
                ASSERT_WITH_STRING(count <= length_, "TCThreadCache::TCFreeList::PopBatch: count > length_!")
#endif
                void* temp = ptr_header_;
                for(int32_t i = 0;i != count; ++i){
                    batch[i] = temp;
                    temp = GetNextPtr(temp);
                }
                ptr_header_ = temp;
                length_ -= count;
            }
    };
private:
    // static std::size_t over_thread_cache_size_;
private:
    std::size_t size_;
    TCThreadCache* next_thread_cache_;
    TCThreadCache* last_thread_cache_;
    TCFreeList free_list_[kBucketNum];
#ifdef DEBUG
public:
    bool has_initialized_ = false;
    inline TCFreeList* GetFreeList(uint8_t bucket_num){
        return &free_list_[bucket_num];
    }
#endif
public:
    TCThreadCache() = default;
    ~TCThreadCache() = default;

    void Initialize();
    void Clear();

    void* Allocate(std::size_t size);
    void Deallocate(void* ptr, std::size_t size);

    void* FetchFromCentralCache(uint8_t bucket_num, std::size_t size);
    void ListTooLong(TCFreeList* free_list, uint8_t bucket_num);
    void ReleaseToCentralCache(TCFreeList* free_list, uint8_t bucket_num, uint32_t move_num);
};
}
#endif
