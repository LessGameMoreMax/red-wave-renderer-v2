#ifndef TC_THREAD_CACHE_H
#define TC_THREAD_CACHE_H
#include "TCCommon.h"
#include "TCSizeClasses.h"
namespace sablin{

class TCThreadCache{
#ifdef DEBUG
public:
#else
private:
#endif
    class TCFreeList{
        private:
            void* ptr_header_;
            uint32_t length_;
            uint32_t max_length_ ;
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
                SetNextPtr(ptr_header_, GetNextPtr(ptr_header_));
                --length_;
                return result;
            }
    };
private:
    static std::size_t over_thread_cache_size_;
    static std::size_t unclaimed_cache_size_;
private:
    std::size_t size_;
    std::size_t max_size_;
    TCThreadCache* next_thread_cache_;
    TCThreadCache* last_thread_cache_;
    TCFreeList free_list_[kBucketNum];
#ifdef DEBUG
public:
    bool has_initialized_ = false;
#endif
public:
    TCThreadCache() = default;
    ~TCThreadCache() = default;

    void Initialize();
    void Clear();
};
}
#endif
