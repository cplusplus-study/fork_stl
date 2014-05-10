// -*- C++ -*-

#ifndef __FORK_STL__BIND__
#define __FORK_STL__BIND__
#include <type_traits>
#include <utility>
#include <tuple>
#include <functional>
#include <base/__invoke_base.hpp>

namespace xusd{
    template <int NUM> struct placeholder{ };

    template <typename T> struct is_placeholder;
    template <int NUM> struct is_placeholder<placeholder<NUM> >{ enum{ value = NUM }; };
    template <typename T> struct is_placeholder{ enum{ value = 0 }; };

    template <int ...N> struct seq{ };
    template <unsigned N, unsigned...S> struct gen;
    template <unsigned N, unsigned...S> struct gen: gen<N-1, N-1, S...>{ };
    template <unsigned...S> struct gen<0, S...>{ typedef seq<S...> type; };

    template <int N, typename B, typename C>
    auto select(std::false_type, B&& b, C&& c) -> decltype(std::get<N>(b)){
        return std::get<N>(b);
    }

    template <int N, typename B, typename C>
    auto select(std::true_type, B&& b, C&& c) ->
    decltype(std::get< is_placeholder< typename std::tuple_element< N, typename std::decay<B>::type >::type >::value -1 >(c)) {
        return std::get< is_placeholder< typename std::tuple_element< N, typename std::decay<B>::type >::type >::value -1 >(c);
    }

    template<typename F, typename... Args>
    class bind_t {
        typedef std::tuple<Args...> BindArgs;
        typedef F CallFun;
    public:
        typedef typename base::__get_result_type<F>::type result_type;

        bind_t(F fun, Args... args):_fun(fun), _bindArgs(args...){ }

        template<typename... CArgs>
        result_type operator()(CArgs&&... c){
            std::tuple<CArgs...> cargs(c...);
            return _call(cargs, typename gen<std::tuple_size<BindArgs>::value>::type());
        }

    private:
        template<typename T, int ...S>
        result_type _call(T&& t, seq<S...>){ 
            base::__invoke(_fun,select<S>( std::integral_constant< bool, is_placeholder< typename std::tuple_element<S, BindArgs>::type >::value != 0 >(), _bindArgs, t)...);
        }
    private:
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
