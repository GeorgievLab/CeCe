
#pragma once

#include <memory>

template <typename T, size_t TALIGN=16, size_t TBLOCK=8>
class aligned_allocator : public std::allocator<T>
{

public:

    using pointer = T*;
    using size_type = std::size_t;

public:
     aligned_allocator() {}
     aligned_allocator(const aligned_allocator& src) : std::allocator<T>(src) {}

     template<typename Allocator>
     aligned_allocator(const Allocator& src) : std::allocator<T>(src) {}

     aligned_allocator& operator=(const aligned_allocator &rhs){
         std::allocator<T>::operator=(rhs);
         return *this;
     }

     pointer allocate(size_type n, const void *hint){
         pointer p = NULL;
         size_t count = sizeof(T) * n;
         size_t count_left = count % TBLOCK;
         if( count_left != 0 )
         {
             count += TBLOCK - count_left;
         }

         posix_memalign(&p, count, TALIGN);
         return p;
     }

     void deallocate(pointer p, size_type n){
         free(p);
     }

     void construct(pointer p, const T &val){
         new(p) T(val);
     }

     void destroy(pointer p){
         p->~T();
     }
};
