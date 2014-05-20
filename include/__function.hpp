// -*- C++ -*-

#ifndef __FORK_STL__FUNCTION__
#define __FORK_STL__FUNCTION__

#include <type_traits>
#include <utility>
#include <typeinfo>
#include <base/__invoke_base.hpp>


namespace xusd {

template<typename R, typename...Args>
class __base{
public:
    virtual __base* clone() = 0;
    virtual ~__base(){ }
    virtual R doCall(Args...args) = 0;
};

template<typename Functor,typename R, typename...Args>
class __func: public __base<R, Args...>{
public:
    __func(const Functor& f):_fun(f){ }
    __func(Functor&& f):_fun(std::move(f)){ }
    __func(const __func& func):_fun(func._fun){ }
    __func(__func&& func):_fun(std::move(func._fun)){ }
    __base<R,Args...>* clone(){
        return new __func(*this);
    }
    R doCall(Args... args){
        return xusd::base::__invoke(_fun, args...);
    }
    ~__func(){ }
private:
    Functor _fun;
};

class bad_function_call : public std::exception { };

template <typename R, typename... Args>
class function;
template <typename R, typename... Args>
class function<R(Args...)>{
public:
    function() noexcept:function(nullptr){ }
    function(std::nullptr_t) noexcept:__f(nullptr){ }
    function(const function& other){ __f = other.__f.clone(); }
    function(function&& other) noexcept{
        __f = other.__f;
        other.__f = nullptr;
    }

    template<class F>
    function(F f){
        __f = new __func<typename std::decay<F>::type, R, typename std::decay<Args>::type...>(f);
    }

    function& operator=(const function& other){
        if(__f){ delete __f; }
        __f = other.__f.clone(); 
    }
    function& operator=(function&& other) noexcept{
        __f = other.__f;
        other.__f = nullptr;
    }
    function& operator=(std::nullptr_t) noexcept{
        if(__f){ delete __f; }
        __f = nullptr;
    }
    template<class F>
    function& operator=(F&& f){
        if(__f){ delete __f; }
        __f = new __func<typename std::decay<F>::type, R, Args...>(f);
    }
    template<class F>
    function& operator=(std::reference_wrapper<F> other) noexcept{
        if(__f){ delete __f; }
        __f = other.__f.clone(); 
    }

    ~function(){
        if(__f){ delete __f;}
    }

    // function modifiers:
    void swap(function& other) noexcept{
        __base<R, Args...>* tmp = this->__f;
        this->__f = other.__f;
        other.__f = tmp;
    }

    // function capacity:
    explicit operator bool() const noexcept{
        return !__f;
    }

    // function invocation:
    R operator()(Args...args) const{
        if(__f){
            return __f->doCall(args...);
        }else{
            throw xusd::bad_function_call();
        }
    }

    // function target access:
    const std::type_info& target_type() const noexcept;
    template <typename T>       T* target() noexcept;
    template <typename T> const T* target() const noexcept;

private:
    __base<R, typename std::decay<Args>::type...>* __f{ nullptr };
};
} //namesapce xusd

namespace std{
template< class R, class... ArgTypes >
bool operator==(const xusd::function<R(ArgTypes...)>& f, std::nullptr_t ){
    return !f;
}
template< class R, class... ArgTypes >
bool operator==(std::nullptr_t, const xusd::function<R(ArgTypes...)>& f ){
    return !f;
}
template< class R, class... ArgTypes >
bool operator!=(const xusd::function<R(ArgTypes...)>& f, std::nullptr_t ){
    return !!f;
}
template< class R, class... ArgTypes >
bool operator!=(std::nullptr_t, const xusd::function<R(ArgTypes...)>& f ){
    return !!f;
}

template< class R, class... ArgTypes >
void swap(xusd::function<R(ArgTypes...)> &f1, xusd::function<R(ArgTypes...)> &f2){
    f1.swap(f2);
}

}

#endif
