// -*- C++ -*-

#ifndef __FORK_STL__BIND__
#define __FORK_STL__BIND__
#include <type_traits>
#include <utility>
#include <tuple>
#include <functional>
#include <base/__invoke_base.hpp>
#include <base/__mpl_tools.hpp>

namespace xusd{
    template <int NUM> struct placeholder{ };

    template <typename T> struct is_placeholder;
    template <int NUM> struct is_placeholder<placeholder<NUM> >{ enum{ value = NUM }; };
    template <typename T> struct is_placeholder{ enum{ value = 0 }; };


    template <typename B, typename C>
    auto select(B&& b, C&& c) -> B&& {
        return std::forward<B>(b);
    }
    template <typename B, typename C>
    auto select(std::reference_wrapper<B> b, C&& c) -> decltype(b.get()) {
        return b.get();
    }

    template <int N, typename C>
    auto select(placeholder<N> b, C&& c) -> typename if_<std::is_rvalue_reference<typename std::tuple_element<N-1, typename std::decay<C>::type>::type>::value,typename std::tuple_element<N-1, typename std::decay<C>::type>::type,decltype(std::get<N-1>(c))>::type {
        return static_cast<typename if_<std::is_rvalue_reference<typename std::tuple_element<N-1, typename std::decay<C>::type>::type>::value,typename std::tuple_element<N-1, typename std::decay<C>::type>::type,decltype(std::get<N-1>(std::forward<C>(c)))>::type>(std::get<N-1>(std::forward<C>(c)));
    }

    template<typename F, typename BindArgs, typename CallArgs, typename Gen = typename gen<std::tuple_size<BindArgs>::value>::type >
    struct __bind_return_type;

    template<typename F, typename... BindArgs, typename... CallArgs, int ...S>
    struct __bind_return_type<F, std::tuple<BindArgs...>, std::tuple<CallArgs...> , seq<S...> >{
        typedef decltype(base::__invoke(std::declval<F>(),select(std::get<S>(std::declval<typename std::tuple<BindArgs...>>()), std::declval<typename std::tuple<CallArgs...>&>())...)) type;
    };

    template<typename F, typename... Args>
    class bind_t {
        typedef std::tuple<typename std::decay<Args>::type...> BindArgs;
        typedef typename std::decay<F>::type CallFun;
    public:
        bind_t(F fun, Args... args):_fun(fun), _bindArgs(args...){ }

        template<typename... CArgs>
        typename __bind_return_type<CallFun, BindArgs, typename std::tuple<CArgs&&...>>::type operator()(CArgs&&... c){
            std::tuple<CArgs&&...> cargs(std::forward<CArgs>(c)...);
            return _call(cargs, typename gen<std::tuple_size<BindArgs>::value>::type());
        }

    private:
        template<typename T, int ...S>
        typename __bind_return_type<CallFun, BindArgs, typename std::decay<T>::type>::type _call(T&& t, seq<S...>){
            return base::__invoke(_fun,select(std::get<S>(_bindArgs), std::forward<T>(t))...);
        }

        CallFun _fun;
        BindArgs _bindArgs;
    };

    template <typename F, typename... Args>
    bind_t<typename std::decay<F>::type, typename std::decay<Args>::type...>
    bind(F f, Args&&... args){
        return bind_t< typename std::decay<F>::type, typename std::decay<Args>::type...>(f, args...);
    }

    extern placeholder<1> _1; extern placeholder<2> _2; extern placeholder<3> _3; extern placeholder<4> _4;
    extern placeholder<5> _5; extern placeholder<6> _6; extern placeholder<7> _7; extern placeholder<8> _8;
    extern placeholder<9> _9; extern placeholder<10> _10; extern placeholder<11> _11; extern placeholder<12> _12;
    extern placeholder<13> _13; extern placeholder<14> _14; extern placeholder<15> _15; extern placeholder<16> _16;
    extern placeholder<17> _17; extern placeholder<18> _18; extern placeholder<19> _19; extern placeholder<20> _20;
    extern placeholder<21> _21; extern placeholder<22> _22; extern placeholder<23> _23; extern placeholder<24> _24;
}
#endif
