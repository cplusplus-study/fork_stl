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

    enum class future_errc {
        future_already_retrieved = 1,
        promise_already_satisfied,
        no_state,
        broken_promise
    };

    enum class launch {
        async = 1,
        deferred = 2,
        any = async | deferred
    };

    enum class future_status {
        ready,
        timeout,
        deferred
    };

    class future_error: public std::logic_error{
    public:
        future_error():std::logic_error("xxx"){ }
    };
    //}}}

    // -- __assoc_state {{{

    class __assoc_state_base;
    class __make_assoc_ready_at_thread_exit{
    public:
        __make_assoc_ready_at_thread_exit(__assoc_state_base* state):__state_(state){ }
        ~__make_assoc_ready_at_thread_exit();
    private:
        __assoc_state_base* __state_;
    };
    __thread_specific_ptr<__make_assoc_ready_at_thread_exit>&
    __thread_local_make_assoc_ready_at_thread_exit_data() {
        static __thread_specific_ptr<__make_assoc_ready_at_thread_exit> __p;
        return __p;
    }


    class __assoc_state_base {
    protected:
        std::exception_ptr __exception_;
        mutable std::mutex __mut_;
        mutable std::condition_variable __cv_;
        unsigned __state_;
        enum {
            __constructed = 1,
            __future_attached = 2,
            ready = 4,
            deferred = 8
        };
    public:
        virtual void __invoke();
        __assoc_state_base() : __state_(0) {}
        void set_value() {
            std::unique_lock<std::mutex> __lk(__mut_);
            if (__has_value())
                throw future_error();
            __state_ |= __constructed | ready;
            __cv_.notify_all();
            __lk.unlock();
        }

        void set_value_at_thread_exit() {
            std::unique_lock<std::mutex> __lk(__mut_);
            if (__has_value())
                throw future_error();
            __state_ |= __constructed;
            __thread_local_make_assoc_ready_at_thread_exit_data().reset(new __make_assoc_ready_at_thread_exit(this));
            __lk.unlock();
        }

        void set_exception(std::exception_ptr __p) {
            std::unique_lock<std::mutex> __lk(__mut_);
            if (__has_value())
                throw future_error();
            __exception_ = __p;
            __state_ |= ready;
            __lk.unlock();
            __cv_.notify_all();
        }

        void set_exception_at_thread_exit(std::exception_ptr __p) {
            std::unique_lock<std::mutex> __lk(__mut_);
            if (__has_value())
                throw future_error();
            __exception_ = __p;
            __thread_local_make_assoc_ready_at_thread_exit_data().reset(new __make_assoc_ready_at_thread_exit(this));
            __lk.unlock();
        }


        void copy() {
            std::unique_lock<std::mutex> __lk(__mut_);
            __wait(__lk);
            if (__exception_ != nullptr)
                std::rethrow_exception(__exception_);
            __lk.unlock();
        }

        void wait() {
            std::unique_lock<std::mutex> __lk(__mut_);
            __wait(__lk);
        }

        void __wait(std::unique_lock<std::mutex>& __lk){
            if (!__is_ready()) {
                if (__state_ & static_cast<unsigned>(deferred)) {
                    __state_ &= ~static_cast<unsigned>(deferred);
                    __lk.unlock();
                    __invoke();
                } else {
                    while (!__is_ready()){
                        __cv_.wait(__lk);
                    }
                }
            }
        }

        bool __has_value() const {return (__state_ & __constructed) || (__exception_ != nullptr);}

        bool __has_future_attached() const {return (__state_ & __future_attached) != 0;}
        void __set_future_attached() {
            std::lock_guard<std::mutex> __lk(__mut_);
            __state_ |= __future_attached;
        }


        void __set_deferred() {__state_ |= deferred;}

        void __make_ready(){
            std::unique_lock<std::mutex> __lk(__mut_);
            __state_ |= ready;
            __lk.unlock();
            __cv_.notify_all();
        }
        bool __is_ready() const {return (__state_ & ready) != 0;}

        template<typename _Clock, typename _Duration>
        future_status wait_until(const xusd::chrono::time_point<_Clock, _Duration>& __abs_time) const {
            std::unique_lock<std::mutex> __lk(__mut_);
            if (__state_ & deferred)
                return future_status::deferred;
            while (!(__state_ & ready) && _Clock::now() < __abs_time)
                __cv_.wait_until(__lk, __abs_time);
            if (__state_ & ready)
                return future_status::ready;
            return future_status::timeout;
        }

        template <class _Rep, class _Period>
        future_status wait_for(const xusd::chrono::duration<_Rep, _Period>& __rel_time) const {
            return wait_until(xusd::chrono::steady_clock::now() + __rel_time);
        }
    };
    void __assoc_state_base::__invoke(){
        throw future_error();
    }

    __make_assoc_ready_at_thread_exit::~__make_assoc_ready_at_thread_exit(){
        if(__state_){
            __state_->__make_ready();
        }
    }
    
    template <class _Rp>
    class __assoc_state :public __assoc_state_base {
        typedef __assoc_state_base base;
        typedef typename std::aligned_storage<sizeof(_Rp), std::alignment_of<_Rp>::value>::type _ValueType;
        _ValueType __value_;
    public:

        __assoc_state() : __assoc_state_base() {}

        template <class _Arg>
        void set_value(_Arg&& __arg){
            std::unique_lock<std::mutex> __lk(this->__mut_);
            if (this->__has_value())
                throw future_error();
            ::new(&__value_) _Rp(std::forward<_Arg>(__arg));
            this->__state_ |= __constructed | ready;
            __lk.unlock();
            __cv_.notify_all();
        }

        template <class _Arg>
        void set_value_at_thread_exit(_Arg&& __arg){
            std::unique_lock<std::mutex> __lk(this->__mut_);
            if (this->__has_value())
                throw future_error();
            ::new(&__value_) _Rp(std::forward<_Arg>(__arg));
            this->__state_ |= __constructed;
            __thread_local_make_assoc_ready_at_thread_exit_data().reset(new __make_assoc_ready_at_thread_exit(this));
            __lk.unlock();
        }

        _Rp move(){
            std::unique_lock<std::mutex> __lk(this->__mut_);
            this->__wait(__lk);
            if (this->__exception_ != nullptr)
                std::rethrow_exception(this->__exception_);
            return std::move(*(_Rp*)&__value_);
        }
        typename std::add_lvalue_reference<_Rp>::type copy(){
            std::unique_lock<std::mutex> __lk(this->__mut_);
            this->__wait(__lk);
            if (this->__exception_ != nullptr)
                std::rethrow_exception(this->__exception_);
            return *(_Rp*)&__value_;
        }

    };
    template <class _Rp>
    class __assoc_state<_Rp&> :public __assoc_state_base {
        typedef __assoc_state_base base;
        typedef _Rp* _ValueType;
        _ValueType __value_;
    public:

        __assoc_state() : __assoc_state_base() {}

        void set_value(_Rp& __arg){
            std::unique_lock<std::mutex> __lk(this->__mut_);
            if (this->__has_value())
                throw future_error();
            __value_ = std::addressof(__arg);
            this->__state_ |= __constructed | ready;
            __lk.unlock();
            __cv_.notify_all();
        }

        void set_value_at_thread_exit(_Rp& __arg){
            std::unique_lock<std::mutex> __lk(this->__mut_);
            if (this->__has_value())
                throw future_error();
            __value_ = std::addressof(__arg);
            this->__state_ |= __constructed;
            __thread_local_make_assoc_ready_at_thread_exit_data().reset(new __make_assoc_ready_at_thread_exit(this));
            __lk.unlock();
        }

        _Rp& copy(){
            std::unique_lock<std::mutex> __lk(this->__mut_);
            this->__wait(__lk);
            if (this->__exception_ != nullptr)
                std::rethrow_exception(this->__exception_);
            return *__value_;
        }

    };

    template <class _Rp, class _Fp>
    class __deferred_assoc_state : public __assoc_state<_Rp> {
        typedef __assoc_state<_Rp> base;

        _Fp __func_;

    public:
        explicit __deferred_assoc_state(_Fp&& __f);

        virtual void __execute();
    };

    template <class _Rp, class _Fp>
    inline __deferred_assoc_state<_Rp, _Fp>::__deferred_assoc_state(_Fp&& __f)
        : __func_(std::forward<_Fp>(__f)) {
        this->__set_deferred();
    }

    template <class _Rp, class _Fp>
    void __deferred_assoc_state<_Rp, _Fp>::__execute()
    {
        try {
            this->set_value(__func_());
        }
        catch (...) {
            this->set_exception(std::current_exception());
        }
    }

    template <class _Fp>
    class __deferred_assoc_state<void, _Fp> : public __assoc_state_base {
        typedef __assoc_state_base base;

        _Fp __func_;

    public:
        explicit __deferred_assoc_state(_Fp&& __f);

        virtual void __execute();
    };


    template <class _Fp>
    inline __deferred_assoc_state<void, _Fp>::__deferred_assoc_state(_Fp&& __f)
        : __func_(std::forward<_Fp>(__f)) {
        this->__set_deferred();
    }


    template <class _Fp>
    void __deferred_assoc_state<void, _Fp>::__execute() {
        try {
            __func_();
            this->set_value();
        } catch (...) {
            this->set_exception(std::current_exception());
        }
    }

    //}}} __assoc_state
    
    // -- {{{ future
    template <class R> class shared_future;
    template <class R>
    class future
    {
        friend class shared_future<R>;
        typename std::shared_ptr<__assoc_state<R> > __state_;
    public:
        future(typename std::shared_ptr<__assoc_state<R> > _o) :__state_(_o){
            if(__state_){
                if(__state_->__has_future_attached()){
                    throw future_error();
                }
                __state_->__set_future_attached();
            }
        }
        future() noexcept: __state_(nullptr){ }
        future(future&& _o) noexcept{
            __state_=_o.__state_;
            _o.__state_.reset();
        }
        future(const future& rhs) = delete;
        ~future(){ }

        future& operator=(const future& rhs) = delete;
        future& operator=(future&& _o) noexcept{
            __state_=_o.__state_;
            _o.__state_.reset();
            return *this;
        }
        shared_future<R> share();

        // retrieving the value
        R get(){
            if(!__state_){
                throw future_error();
            }
            std::shared_ptr<__assoc_state<R> > tmp = __state_;
            __state_.reset();
            return tmp->move();
        }

        // functions to check state
        bool valid() const noexcept{
            return !!__state_;
        }

        void wait() const{
            if(__state_){
                __state_->wait();
            }
        }
        template <class Rep, class Period>
        future_status wait_for(const chrono::duration<Rep, Period>& rel_time) const{
            return __state_->wait_for(rel_time);
        }
        template <class Clock, class Duration>
        future_status wait_until(const chrono::time_point<Clock, Duration>& abs_time) const{
            return __state_->wait_until(abs_time);
        }
    };

    template <class R>
    class future<R&>
    {
        friend class shared_future<R&>;
        typedef __assoc_state<R&>  AssocStateType;
    private:
        typename std::shared_ptr<AssocStateType> __state_;
    public:
        future(typename std::shared_ptr<AssocStateType> _o): __state_(_o){
            if(__state_){
                if(__state_->__has_future_attached()){
                    throw future_error();
                }
                __state_->__set_future_attached();
            }
        }
        future() noexcept: __state_(nullptr){ }
        future(future&& _o) noexcept{
            __state_=_o;
            _o.reset();
        }
        future(const future& rhs) = delete;
        ~future(){ }

        future& operator=(const future& rhs) = delete;
        future& operator=(future&& _o) noexcept{
            __state_=_o.__state;
            _o.__state_.reset();
            return *this;
        }
        shared_future<R&> share();

        // retrieving the value
        R& get(){
            if(!__state_){
                throw future_error();
            }
            std::shared_ptr<AssocStateType> tmp = __state_;
            __state_.reset();
            return tmp->copy();
        }

        // functions to check state
        bool valid() const noexcept{
            return !!__state_;
        }

        void wait() const{
            if(__state_){
                __state_->wait();
            }
        }
        template <class Rep, class Period>
        future_status wait_for(const chrono::duration<Rep, Period>& rel_time) const{
            return __state_->wait_for(rel_time);
        }
        template <class Clock, class Duration>
        future_status wait_until(const chrono::time_point<Clock, Duration>& abs_time) const{
                return __state_->wait_until(abs_time);
        }
    };

    template <>
    class future<void>
    {
        friend class shared_future<void>;
        typedef __assoc_state_base  AssocStateType;
    private:
        typename std::shared_ptr<AssocStateType> __state_;
    public:
        future(typename std::shared_ptr<AssocStateType> _o) : __state_(_o){
            if(__state_){
                if(__state_->__has_future_attached()){
                    throw future_error();
                }
                __state_->__set_future_attached();
            }
        }
        future() noexcept: __state_(nullptr){ }
        future(future&& _o) noexcept{
            __state_=_o.__state_;
            _o.__state_.reset();
        }
        future(const future& rhs) = delete;
        ~future(){ }

        future& operator=(const future& rhs) = delete;
        future& operator=(future&& _o) noexcept{
            __state_=_o.__state_;
            _o.__state_.reset();
            return *this;
        }
        shared_future<void> share();

        // retrieving the value
        void get(){
            if(!__state_){
                throw future_error();
            }
            std::shared_ptr<AssocStateType> tmp = __state_;
            __state_.reset();
            return tmp->copy();
        }

        // functions to check state
        bool valid() const noexcept{
            return !!__state_;
        }

        void wait() const{
            if(__state_){
                __state_->wait();
            }
        }
        template <class Rep, class Period>
        future_status wait_for(const chrono::duration<Rep, Period>& rel_time) const{
            return __state_->wait_for(rel_time);
        }
        template <class Clock, class Duration>
        future_status wait_until(const chrono::time_point<Clock, Duration>& abs_time) const{
                return __state_->wait_until(abs_time);
        }
    };
    //}}} future

    // -- {{{ shared_future
    template <class R>
    class shared_future {
        typename std::shared_ptr<__assoc_state<R> > __state_;
    public:
        shared_future(typename std::shared_ptr<__assoc_state<R> > _o) :__state_(_o){
            if(__state_){
                __state_->__set_future_attached();
            }
        }
        shared_future() noexcept: __state_(nullptr){ }
        shared_future(shared_future&& _o) noexcept{
            __state_=_o.__state_;
            _o.__state_.reset();
        }
        shared_future(future<R>&& _o) noexcept{
            __state_=_o.__state_;
            _o.__state_.reset();
        }
        shared_future(const shared_future& rhs): __state_(rhs.__state_){ }
        ~shared_future(){ }

        shared_future& operator=(const shared_future& rhs){
            __state_ = rhs.__state_;
            return *this;
        }

        shared_future& operator=(future<R>&& rhs){
            __state_ = rhs.__state_;
            rhs.__state_.reset();
            return *this;
        }
        shared_future& operator=(shared_future&& _o) noexcept{
            __state_=_o.__state_;
            _o.__state_.reset();
            return *this;
        }

        // retrieving the value
        R get(){
            if(!__state_){
                throw future_error();
            }
            return __state_->move();
        }

        // functions to check state
        bool valid() const noexcept{
            return !!__state_;
        }

        void wait() const{
            if(__state_){
                __state_->wait();
            }
        }
        template <class Rep, class Period>
        future_status wait_for(const chrono::duration<Rep, Period>& rel_time) const{
            return __state_->wait_for(rel_time);
        }
        template <class Clock, class Duration>
        future_status wait_until(const chrono::time_point<Clock, Duration>& abs_time) const{
                return __state_->wait_until(abs_time);
        }
    };

    template <class R>
    class shared_future<R&>
    {
    public:
        typedef __assoc_state<R&>  AssocStateType;
    private:
        typename std::shared_ptr<AssocStateType> __state_;
    public:
        shared_future(typename std::shared_ptr<AssocStateType> _o): __state_(_o){
            if(__state_){
                __state_->__set_future_attached();
            }
        }
        shared_future() noexcept: __state_(nullptr){ }
        shared_future(shared_future&& _o) noexcept{
            __state_=_o;
            _o.reset();
        }
        shared_future(const shared_future& rhs):__state_(rhs.__state_){ }
        shared_future(future<R&>&& other):__state_(other.__state_){
            other.__state_.reset();
        }
        ~shared_future(){ }

        shared_future& operator=(const shared_future& rhs){
            __state_ = rhs.__state_;
            return *this;
        }
        shared_future& operator=(shared_future&& _o) noexcept{
            __state_=_o.__state;
            _o.__state_.reset();
            return *this;
        }
        shared_future& operator=(future<R&>&& other){
            __state_ = other.__state_;
            other.__state_.reset();
            return *this;
        }

        // retrieving the value
        R& get(){
            if(!__state_){
                throw future_error();
            }
            return __state_->copy();
        }

        // functions to check state
        bool valid() const noexcept{
            return !!__state_;
        }

        void wait() const{
            if(__state_){
                __state_->wait();
            }
        }
        template <class Rep, class Period>
        future_status wait_for(const chrono::duration<Rep, Period>& rel_time) const{
            return __state_->wait_for(rel_time);
        }
        template <class Clock, class Duration>
        future_status wait_until(const chrono::time_point<Clock, Duration>& abs_time) const{
            return __state_->wait_until(abs_time);
        }
    };

    template <>
    class shared_future<void>
    {
    public:
        typedef __assoc_state_base  AssocStateType;
    private:
        typename std::shared_ptr<AssocStateType> __state_;
    public:
        shared_future(typename std::shared_ptr<AssocStateType> _o) : __state_(_o){
            if(__state_){
                __state_->__set_future_attached();
            }
        }
        shared_future() noexcept: __state_(nullptr){ }
        shared_future(shared_future&& _o) noexcept{
            __state_=_o.__state_;
            _o.__state_.reset();
        }
        shared_future(future<void>&& _o) noexcept{
            __state_=_o.__state_;
            _o.__state_.reset();
        }
        shared_future(const shared_future& rhs):__state_(rhs.__state_){ }
        ~shared_future(){ }

        shared_future& operator=(const shared_future& rhs){
            __state_ = rhs.__state_;
            return *this;
        }
        shared_future& operator=(shared_future&& _o) noexcept{
            __state_=_o.__state_;
            _o.__state_.reset();
            return *this;
        }
        shared_future& operator=(future<void>&& _o) noexcept{
            __state_=_o.__state_;
            _o.__state_.reset();
            return *this;
        }

        // retrieving the value
        void get(){
            if(!__state_){
                throw future_error();
            }
            return __state_->copy();
        }

        // functions to check state
        bool valid() const noexcept{
            return !!__state_;
        }

        void wait() const{
            if(__state_){
                __state_->wait();
            }
        }
        template <class Rep, class Period>
        future_status wait_for(const chrono::duration<Rep, Period>& rel_time) const{
               return __state_->wait_for(rel_time);
        }
        template <class Clock, class Duration>
        future_status wait_until(const chrono::time_point<Clock, Duration>& abs_time) const{
            return __state_->wait_until(abs_time);
        }
    };
    //}}} future

    // -- promise {{{

    template <class R>
    class promise {
        typename std::shared_ptr<__assoc_state<R> > __state_;
        promise(std::shared_ptr<__assoc_state<R&> > s):__state_(s){ }
        template<typename> friend class packaged_task;
    public:
        promise():__state_(new __assoc_state<R>()){ }
        promise(promise&& rhs) noexcept:__state_(rhs.__state_){
           rhs.__state_.reset();
        }
        promise(const promise& rhs) = delete;
        ~promise(){ }

        // assignment
        promise& operator=(promise&& rhs) noexcept{
             __state_ = rhs.__state_;
            rhs.__state_.reset();
            return *this;
        }
        promise& operator=(const promise& rhs) = delete;
        void swap(promise& other) noexcept{
            std::swap(__state_, other.__state_);
        }

        // retrieving the result
        future<R> get_future(){
            return std::move(future<R>(__state_));
        }

        // setting the result
        void set_value(const R& r){
            if (__state_ == nullptr)
                throw future_error();
            __state_->set_value(r);
        }
        void set_value(R&& r){
            if (__state_ == nullptr)
                throw future_error();
            __state_->set_value(std::move(r));
        }
        void set_exception(std::exception_ptr p){
            if (__state_ == nullptr)
                throw future_error();
            __state_->set_exception(p);
        }

        // setting the result with deferred notification
        void set_value_at_thread_exit(const R& r){
            __state_->set_value_at_thread_exit(r);
        }
        void set_value_at_thread_exit(R&& r){
            __state_->set_value_at_thread_exit(std::move(r));
        }
        void set_exception_at_thread_exit(std::exception_ptr p){
            __state_->set_exception_at_thread_exit(p);
        }
    };

    template <class R>
    class promise<R&>
    {
        typename std::shared_ptr<__assoc_state<R&> > __state_;
        promise(std::shared_ptr<__assoc_state<R&> > s):__state_(s){ }
        template<typename> friend class packaged_task;

    public:
        promise():__state_(new __assoc_state<R&>()){ }
        promise(promise&& rhs) noexcept:__state_(rhs.__state_){
           rhs.__state_.reset();
        }
        promise(const promise& rhs) = delete;
        ~promise(){ }

        // assignment
        promise& operator=(promise&& rhs) noexcept{
             __state_ = rhs.__state_;
            rhs.__state_.reset();
            return *this;
        }
        promise& operator=(const promise& rhs) = delete;
        void swap(promise& other) noexcept{
            std::swap(__state_, other.__state_);
        }

        // retrieving the result
        future<R&> get_future(){
            return std::move(future<R&>(__state_));
        }

        // setting the result
        void set_value(R& r){
            if (__state_ == nullptr)
                throw future_error();
            __state_->set_value(r);
        }

        void set_exception(std::exception_ptr p){
            if (__state_ == nullptr)
                throw future_error();
            __state_->set_exception(p);
        }

        // setting the result with deferred notification
        void set_value_at_thread_exit(const R& r){
            __state_->set_value_at_thread_exit(r);
        }

        void set_exception_at_thread_exit(std::exception_ptr p){
            __state_->set_exception_at_thread_exit(p);
        }
    };

    template <>
    class promise<void>
    {
        template<typename> friend class packaged_task;
        typename std::shared_ptr<__assoc_state_base > __state_;
    public:
        promise(std::shared_ptr<__assoc_state_base >);
        promise():__state_(new __assoc_state_base()){ }
        promise(promise&& rhs) noexcept:__state_(rhs.__state_){
           rhs.__state_.reset();
        }
        promise(const promise& rhs) = delete;
        ~promise(){ }

        // assignment
        promise& operator=(promise&& rhs) noexcept{
             __state_ = rhs.__state_;
            rhs.__state_.reset();
            return *this;
        }
        promise& operator=(const promise& rhs) = delete;
        void swap(promise& other) noexcept{
            std::swap(__state_, other.__state_);
        }

        // retrieving the result
        future<void> get_future(){
            return std::move(future<void>(__state_));
        }

        // setting the result
        void set_value(){
            if (__state_ == nullptr)
                throw future_error();
            __state_->set_value();
        }

        void set_exception(std::exception_ptr p){
            if (__state_ == nullptr)
                throw future_error();
            __state_->set_exception(p);
        }

        // setting the result with deferred notification
        void set_value_at_thread_exit(){
            __state_->set_value_at_thread_exit();
        }
        void set_exception_at_thread_exit(std::exception_ptr p){
            __state_->set_exception_at_thread_exit(p);
        }
    };
    //}}} promise

    // -- async {{{
    template <class _Rp, class _Fp, typename... Args>
    future<_Rp> __make_async_assoc_state(_Fp&& __f, Args&&... args) {

        //promise<_Rp> pro = promise<_Rp>();
        //future<_Rp> f  = pro.get_future();

        std::shared_ptr<__assoc_state<_Rp>> __state_(new __assoc_state<_Rp>());
        future<_Rp> f(__state_);
        auto myfun = [](__assoc_state<_Rp> * s, typename std::decay<_Fp>::type fun, typename std::decay<Args>::type... arg) -> void {
            try {
                s->set_value(fun(std::move(arg)...));
            } catch(...) {
                s->set_exception(std::current_exception());
            }
        };
        auto ss = xusd::bind(myfun, __state_.get(),std::move(__f), std::move(args)...);
        thread(std::move(ss)).detach();

        return std::move(f);
    }

    template <class _Fp, typename... Args>
    future<void> __make_async_assoc_state(_Fp&& __f, Args&& ...args) {
        auto p = promise<void>();
        auto f  = p.get_future();
        thread(xusd::bind([](promise<void>& pr, _Fp&& fun,Args&&...arg) -> void {
            try {
                fun(arg...);
                pr.set_value();
            } catch(...) {
                pr.set_exception(std::current_exception());
            }
        }, std::move(p),std::move(__f), std::move(args)...)).detach();

        return std::move(f);
    }
    template <class _Rp, class _Fp>
    future<_Rp> __make_deferred_assoc_state(_Fp&& __f) {

        return future<_Rp>{};
    }

    template <class F, class... Args>
    future<typename std::result_of<typename std::decay<F>::type(typename std::decay<Args>::type...)>::type>
    async(launch policy, F&& __f, Args&&... args){
        typedef typename std::result_of<typename std::decay<F>::type(typename std::decay<Args>::type...)>::type _Rp;

        try {

            if ((int(policy) & int(launch::async)) != 0){
                return __make_async_assoc_state<_Rp>(std::forward<F>(__f), std::forward<Args>(args)...);
            }
        }
        catch ( ... ) { if (policy == launch::async) throw ; }

        if ((int(policy) & int(launch::deferred)) != 0){
            return __make_deferred_assoc_state<_Rp>(xusd::bind(std::forward<F>(__f), std::forward<Args>(args)...));
        } else {
            return future<_Rp>{};
        }
    }

    template <class F, class... Args>
    future<typename std::result_of<typename std::decay<F>::type(typename std::decay<Args>::type...)>::type>
    async(F&& __f, Args&&... args){
        return async(launch::any, std::forward<F>(__f), std::forward<Args>(args)...);
    }

    //}}} async

    // -- packaged_task{{{
    template <class> class packaged_task; // undefined

    template <class R, class... ArgTypes>
    class packaged_task<R(ArgTypes...)> {
        std::function<R(ArgTypes...)> __f_;
        promise<R> __p_;
    public:
        typedef R result_type;

        // construction and destruction
        packaged_task() noexcept: __f_(), __p_(std::shared_ptr<__assoc_state<R>>(nullptr)){ }
        template <class F>
        explicit packaged_task(F&& f): __f_(std::move(f)), __p_(){ }

        ~packaged_task(){ }

        // no copy
        packaged_task(const packaged_task&) = delete;
        packaged_task& operator=(const packaged_task&) = delete;

        // move support
        packaged_task(packaged_task&& other) noexcept:__f_(std::move(other.__f_)), __p_(std::move(other.__p_)){ }

        packaged_task& operator=(packaged_task&& other) noexcept{
            __f_ = std::move(other.__f_);
            __p_ = std::move(other.__p_);
            return *this;
        }
        void swap(packaged_task& other) noexcept{
            std::swap(__f_, other.__f_);
            std::swap(__p_, other.__p_);
        }

        bool valid() const noexcept{
            return !!__p_.__state_;
        }

        // result retrieval
        future<R> get_future(){
            if (__p_.__state_ == nullptr){
                throw future_error();
            }
            return __p_.get_future();
        }

        // execution
        void operator()(ArgTypes... args){
            if (__p_.__state_ == nullptr){
                throw future_error();
            }
            if (__p_.__state_->__has_value()){
                throw future_error();
            }
            try {
                __p_.set_value(__f_(std::forward<ArgTypes>(args)...));
            } catch (...) {
                __p_.set_exception(std::current_exception());
            }
        }
        void make_ready_at_thread_exit(ArgTypes...args){
            if (__p_.__state_ == nullptr)
                throw future_error();
            if (__p_.__state_->__has_value())
                throw future_error();
            try {
                __p_.set_value_at_thread_exit(__f_(std::forward<ArgTypes>(args)...));
            } catch (...) {
                __p_.set_exception_at_thread_exit(std::current_exception());
            }
        }

        void reset(){
            __p_ = promise<R>();
        }
    };


    template <class... ArgTypes>
    class packaged_task<void(ArgTypes...)> {
        std::function<void(ArgTypes...)> __f_;
        promise<void> __p_;
    public:
        typedef void result_type;

        // construction and destruction
        packaged_task() noexcept: __f_(), __p_(std::shared_ptr<__assoc_state_base>(nullptr)){ }
        template <class F>
        explicit packaged_task(F&& f): __f_(std::move(f)), __p_(){ }

        ~packaged_task(){ }

        // no copy
        packaged_task(const packaged_task&) = delete;
        packaged_task& operator=(const packaged_task&) = delete;

        // move support
        packaged_task(packaged_task&& other) noexcept:__f_(std::move(other.__f_)), __p_(std::move(other.__p_)){ }

        packaged_task& operator=(packaged_task&& other) noexcept{
            __f_ = std::move(other.__f_);
            __p_ = std::move(other.__p_);
            return *this;
        }
        void swap(packaged_task& other) noexcept{
            std::swap(__f_, other.__f_);
            std::swap(__p_, other.__p_);
        }

        bool valid() const noexcept{
            return !!__p_.__state_;
        }

        // result retrieval
        future<void> get_future(){
            if (__p_.__state_ == nullptr){
                throw future_error();
            }
            return __p_.get_future();
        }

        // execution
        void operator()(ArgTypes... args){
            if (__p_.__state_ == nullptr){
                throw future_error();
            }
            if (__p_.__state_->__has_value()){
                throw future_error();
            }
            try {
                __f_(std::forward<ArgTypes>(args)...);
                __p_.set_value();
            } catch (...) {
                __p_.set_exception(std::current_exception());
            }
        }
        void make_ready_at_thread_exit(ArgTypes...args){
            if (__p_.__state_ == nullptr)
                throw future_error();
            if (__p_.__state_->__has_value())
                throw future_error();
            try {
                __f_(std::forward<ArgTypes>(args)...);
                __p_.set_value_at_thread_exit();
            } catch (...) {
                __p_.set_exception_at_thread_exit(std::current_exception());
            }
        }

        void reset(){
            __p_ = promise<void>();
        }
    };


    template <class R> 
    void swap(xusd::promise<R>& x, xusd::promise<R>& y) noexcept{
        x.swap(y);
    }

    template <class F>
    void swap(packaged_task<F>& x,  packaged_task<F>& y) noexcept {
       x.swap(y);
    }

    //}}} packaged_task end

}
#endif
