// -*- C++ -*-

#ifndef __FORK_STL__THREAD__
#define __FORK_STL__THREAD__
#include <unistd.h>
#include <pthread.h>
#include <ostream>
#include <tuple>
#include <memory>
#include <system_error>
#include <mutex>
#include <condition_variable>
#include <exception>
#include <type_traits>
#include <memory>
#include <chrono.hpp>
#include <bind.hpp>
#include <base/__mpl_tools.hpp>
#include <base/__invoke_base.hpp>

namespace xusd {

    template <class _Fp, class ..._Args, int ..._Indices>
    inline void __thread_execute(std::tuple<_Fp, _Args...>& __t, seq<_Indices...>) {
        xusd::base::__invoke(std::move(std::get<_Indices>(__t))...);
    }

    template <class _Fp>
    void* __thread_proxy(void* __vp) {
        std::unique_ptr<_Fp> __p(static_cast<_Fp*>(__vp));
        typedef typename gen<std::tuple_size<_Fp>::value>::type _Index;
        __thread_execute(*__p, _Index());
        return nullptr;
    }

    class thread {
    public:
        typedef pthread_t native_handle_type;
        class id{
            pthread_t __id_;
        public:
            explicit id(native_handle_type id):__id_(id){ }
            friend bool operator==(const id&, const id&);
            friend bool operator<(const id&, const id&);
            template< class CharT, class Traits >
            friend std::basic_ostream<CharT,Traits>&
            operator<<(std::basic_ostream<CharT,Traits>&&, thread::id);
            friend struct std::hash<id>;
        };
    private:
        native_handle_type __t_;
    public:

        thread()noexcept:__t_(0) { }

        template <class _Fp, class ..._Args>
        thread(_Fp&& __f, _Args&&... __args)
        {
            typedef std::tuple<typename std::decay<_Fp>::type, typename std::decay<_Args>::type...> _Gp;
            std::unique_ptr<_Gp> __p(new _Gp(__decay_copy(std::forward<_Fp>(__f)), __decay_copy(std::forward<_Args>(__args))...));

            int __ec = ::pthread_create(&__t_, 0, &__thread_proxy<_Gp>, __p.get());
            if (__ec == 0){
                __p.release();
            } else {
                throw std::system_error(std::error_code(__ec, std::system_category()), "thread construct failed");
            }
        }

        ~thread(){
            if(__t_ != 0){
                std::terminate();
            }
        }

        thread(const thread&) = delete;
        thread(thread&& t) noexcept: __t_(t.__t_){ t.__t_ = 0; }

        thread& operator=(const thread&) = delete;
        thread& operator=(thread&& t) noexcept{
            __t_ = t.__t_;
            t.__t_ = 0;
            return *this;
        }

        void swap(thread& t) noexcept{ std::swap(t.__t_, __t_); }

        bool joinable() const noexcept{
            return __t_ != 0;
        }
        void join(){
            int ec = pthread_join(__t_, 0);
            if(ec == 0){
                __t_ = 0;
            } else {
                throw std::system_error(std::error_code(ec, std::system_category()), "thread::join failed");
            }
        }
        void detach() {
            int ec = pthread_detach(__t_);
            if(ec == 0){
                __t_ = 0;
            } else {
                throw std::system_error(std::error_code(ec, std::system_category()), "thread::detach failed");
            }
        }
        id get_id() const noexcept{
            return id(__t_);
        }
        native_handle_type native_handle(){
            return __t_;
        }

        static unsigned hardware_concurrency() noexcept{
            long result = sysconf(_SC_NPROCESSORS_ONLN);
            if (result < 0){
                return 0;
            }else{
                return static_cast<unsigned>(result);
            }
        }
    };

    bool operator==(const thread::id& id1, const thread::id & id2){
        return id1.__id_ == id2.__id_;
    }
    bool operator!=(const thread::id& id1, const thread::id & id2){
        return !(id1 == id2);
    }
    bool operator<(const thread::id& id1, const thread::id & id2){
        return id1.__id_ < id2.__id_;
    }

    bool operator<=(const thread::id& id1, const thread::id & id2){
        return id1 == id2 || id1 < id2;
    }

    bool operator>(const thread::id& id1, const thread::id & id2){
        return !(id1 <= id2);
    }
    bool operator>=(const thread::id& id1, const thread::id & id2){
        return !(id1 < id2);
    }
    template< class CharT, class Traits >
    std::basic_ostream<CharT,Traits>& operator<<(std::basic_ostream<CharT,Traits>& out, thread::id id){
        out<<id.__id_;
        return out;        
    }

    void swap(thread& x, thread& y) noexcept{
        x.swap(y);
    }

    namespace this_thread
    {
        thread::id get_id() noexcept{
            return thread::id(::pthread_self());
        }
        void yield() noexcept{
            sched_yield(); //linux alaws return ok
        }

        void sleep_for(const chrono::nanoseconds& ns) {
            using namespace chrono;
            if (ns > nanoseconds::zero()) {
                seconds s = xusd::chrono::duration_cast<seconds>(ns);
                struct timespec ts;
                typedef decltype(ts.tv_sec) ts_sec;
                constexpr ts_sec ts_sec_max = std::numeric_limits<ts_sec>::max();
                if (s.count() < ts_sec_max) {
                    ts.tv_sec = static_cast<ts_sec>(s.count());
                    ts.tv_nsec = static_cast<decltype(ts.tv_nsec)>((ns-s).count());
                } else {
                    ts.tv_sec = ts_sec_max;
                    ts.tv_nsec = giga::num - 1;
                }
                nanosleep(&ts, 0);
            }
        }

        template <class Rep, class Period>
        void sleep_for(const chrono::duration<Rep, Period>& rel_time){
            sleep_for(xusd::chrono::duration_cast<chrono::nanoseconds>(rel_time));
        }

        template <class Clock, class Duration>
        void sleep_until(const chrono::time_point<Clock, Duration>& abs_time){
            class NullType;
            NullType sleep_until_is_not_impliment;
        }

    }


    //{{{ -- thread_specific_ptr
    template <class _Tp>
    class __thread_specific_ptr {
        pthread_key_t __key_;

        __thread_specific_ptr(const __thread_specific_ptr&);
        __thread_specific_ptr& operator=(const __thread_specific_ptr&);

        static void __at_thread_exit(void*);
    public:
        typedef _Tp* pointer;

        __thread_specific_ptr();
        ~__thread_specific_ptr();

        pointer get() const {return static_cast<_Tp*>(pthread_getspecific(__key_));}
        pointer operator*() const {return *get();}
        pointer operator->() const {return get();}
        pointer release();
        void reset(pointer __p = nullptr);
    };

    template <class _Tp>
    void __thread_specific_ptr<_Tp>::__at_thread_exit(void* __p)
    {
        delete static_cast<pointer>(__p);
    }

    template <class _Tp>
    __thread_specific_ptr<_Tp>::__thread_specific_ptr() {
        int __ec = pthread_key_create(&__key_, &__thread_specific_ptr::__at_thread_exit);
        if (__ec)
            throw std::system_error(std::error_code(__ec, std::system_category()), "thread construct failed");
    }

    template <class _Tp>
    __thread_specific_ptr<_Tp>::~__thread_specific_ptr() {
        pthread_key_delete(__key_);
    }
    template <class _Tp>
    typename __thread_specific_ptr<_Tp>::pointer
    __thread_specific_ptr<_Tp>::release() {
        pointer __p = get();
        pthread_setspecific(__key_, 0);
        return __p;
    }

    template <class _Tp>
    void __thread_specific_ptr<_Tp>::reset(pointer __p) {
        pointer __p_old = get();
        pthread_setspecific(__key_, __p);
        delete __p_old;
    }

    //}}} =====================thread_specific_ptr
 
    // -- enum class, exception for thread, future, promise {{{
    typedef enum memory_order
    {
        memory_order_relaxed,
        memory_order_consume,  // load-consume
        memory_order_acquire,  // load-acquire
        memory_order_release,  // store-release
        memory_order_acq_rel,  // store-release load-acquire
        memory_order_seq_cst   // store-release load-acquire
    } memory_order;

    //}}}

    



}
#endif
