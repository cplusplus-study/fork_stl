// -*- C++ -*-

#ifndef __FORK_STL_MEMORY__
#define __FORK_STL_MEMORY__

#include <cstddef>  //size_t
#include <limits>  //UINT_MAX
#include <type_traits>
#include <utility.hpp>
#include <iterator.hpp>


namespace xusd{

    //非c++标准
    template<typename T1,typename T2>
    void construct(T1*p,T2& value){
        new (p) T1(value);
    }

    //非c++标准
    template<typename T>
    void destroy(T* pointer){
        pointer->~T();
    }

    template<bool>
    struct __destroy;

    template<>
    struct __destroy<false>{
        template<typename ForwardIterator>
        static void destroy(ForwardIterator first, ForwardIterator last){
            for(;first != last; ++first){
                xusd::destroy(&*first);
            }
        }
    };
    template<>
    struct __destroy<true>{
        template<typename ForwardIterator>
        static void destroy(ForwardIterator first, ForwardIterator last){
            //....
        }
    };

    //非c++标准
    template<typename ForwardIterator>
    void destroy(ForwardIterator first, ForwardIterator last){              
        xusd::__destroy<
            std::is_trivially_destructible<
            typename xusd::iterator_traits<ForwardIterator>::value_type
            >::value
        >::destroy(first,last);
    }
    
    //c++标准,必须是异常安全的,含有rollback,commit语义
    template<typename InputIterator,typename ForwardIterator>
    ForwardIterator uninitialized_copy(InputIterator first,InputIterator last, ForwardIterator result){
        ForwardIterator cur = result;
        try{
            for(;first != last; ++first,++cur){
                construct(&*cur,*first);
            }
            return cur;
        }catch(...){
            destroy(result,cur);
            throw;
        }

    }

    //c++标准,必须是异常安全的,含有rollback,commit语义
    template<typename InputIterator,typename T>
    void uninitialized_fill(InputIterator first,InputIterator last, const T& value){
        InputIterator cur = first;
        try{
            for(;cur != last; ++cur){
                construct(&*cur,value);
            }
            return cur;
        }catch(...){
            destroy(first,cur);
            throw;
        }

    }

    //c++标准,必须是异常安全的,含有rollback,commit语义
    template<typename InputIterator,typename Size, typename T>
    InputIterator uninitialized_fill_n(InputIterator first,Size n, const T& value){
        InputIterator cur = first;
        try{
            for(Size x = 0;x < n; ++x,++cur){
                construct(&*(cur),value);
            }
            return cur;
        }catch(...){
            destroy(first,cur);
            throw;
        }
    }

    //C++标准
    template<typename T>
    pair<T*,std::ptrdiff_t> get_temporary_buffer(std::ptrdiff_t len){
        //这里没有做异常处理,当没有申请到内存时候应该是<null,0>
        if(len <= 0){
            return xusd::pair<T*,std::ptrdiff_t>(NULL,0);
        }
        try{
            return pair<T*,std::ptrdiff_t>(::operator new(len*sizeof(T)),len);
        }catch(...){
            return pair<T*,std::ptrdiff_t>(NULL,0);
        }
    }

    //C++标准
    template<typename T>
    void return_temporary_buffer(T *p){
        ::operator delete(p);
    }

    //allocator C++ 标准
    //这里应该特化一个void类型,只包含value_type,pointer,const_pointer,rebind等成员
    //参考实现http://www.josuttis.com/cppcode/myalloc.hpp.html
    template<typename T>
    class allocator{
    public:
        //type definitions
        typedef T value_type;
        typedef T* pointer;
        typedef T const* const_pointer;
        typedef T& reference;
        typedef T const& const_reference;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;

        template<typename U>
        class rebind{
        public:
            typedef allocator<U> other; 
        };

        allocator()throw(){}
        template<typename U>
        allocator(allocator<U> const& o)throw(){ }
        ~allocator()throw(){}
        pointer address(reference x)const{
            return (pointer)(&(x));
            //return &x;
        }
        const_pointer address(const_reference x)const{
            return const_pointer(&(x));
            //return &x;
        }

        pointer allocate(size_type n,const void* = 0){
            return static_cast<pointer>(::operator new(n*sizeof(value_type)));
            //按理说这里应该有个set_new_handler(0)的
        }

        void deallocate(pointer p,size_type n){
            ::operator delete((void*)p);
        }

        size_type max_size(){
            return std::numeric_limits<std::size_t>::max()/sizeof(T); 
            //return (UINT_MAX/sizeof(value_type));
        }
        void construct(pointer p,const_reference value){
            new ((void*)p)value_type(value);
        }

        void destroy(pointer p){
            //p->~value_type();
            xusd::destroy(p);
        }
    };
    template<typename T1,typename T2>
    bool operator==(allocator<T1> const&,allocator<T2> const&){
        return true;
    }
    template<typename T1,typename T2>
    bool operator!=(allocator<T1> const&,allocator<T2> const&){
        return false;
    }

    template<>
    class allocator<void>{
    public:
        typedef void value_type;
        typedef void* pointer;
        typedef void const* const_pointer;
        template<typename U>
        class rebind{
        public:
            typedef allocator<U> other; 
        };
    };

}
#endif



