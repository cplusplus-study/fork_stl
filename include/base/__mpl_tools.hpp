// -*- C++ -*-

#ifndef __FORK_STL__MPL_TOOLS__
#define __FORK_STL__MPL_TOOLS__
namespace xusd{
    template <int ...N> struct seq{ };
    template <unsigned N, unsigned...S> struct gen;
    template <unsigned N, unsigned...S> struct gen: gen<N-1, N-1, S...>{ };
    template <unsigned...S> struct gen<0, S...>{ typedef seq<S...> type; };

    template<bool c, typename T, typename F>
    struct if_;
    template<typename T, typename F>
    struct if_<true, T, F>{ typedef T type; };
    template<typename T, typename F>
    struct if_<false, T, F>{ typedef F type; };
}

#endif
