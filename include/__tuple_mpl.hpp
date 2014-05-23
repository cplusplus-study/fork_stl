// -*- C++ -*-

#ifndef __FORK_STL__TUPLE_MPL__
#define __FORK_STL__TUPLE_MPL__

#include <tuple>
#include <type_traits>
namespace xusd{

template<typename T, typename Tuple>
struct tuple_prefix;

template<typename T, typename Tuple>
struct tuple_append;

template<typename T, typename Tuple>
struct tuple_erase;

template<typename T, typename Tuple>
struct tuple_erase_all;

template<size_t N, typename Tuple>
struct tuple_erase_idx;

template<typename Tuple>
struct tuple_erase_dup; //删除相同项

template<typename Tuple>
using tuple_uniq = tuple_erase_dup<Tuple>;

template<size_t N, typename T, typename Tuple>
struct tuple_insert;

template<typename From, typename To,  typename Tuple>
struct tuple_replace;

template<typename From, typename To,  typename Tuple>
struct tuple_replace_all;

template<size_t N, typename T,  typename Tuple>
struct tuple_replace_idx;

template<typename Tuple>
struct tuple_reverse;

template<typename Tuple, template<typename , typename > class Compare>
struct tuple_sort;

template<typename Tuple, template<typename> class Operator>
struct tuple_foreach;

template<typename T, typename Tuple, template <typename T1, typename T2> class Pre = std::is_same, bool = true/*from first or last*/>
struct tuple_find;

///////////////////////////////////////////////////////////////////////////////

template<typename T, typename... Elements>
struct tuple_prefix<T, std::tuple<Elements...> >{
    using type = std::tuple<T, Elements...>;
};

template<typename T, typename... Elements>
struct tuple_append<T, std::tuple<Elements...> >{
    using type = std::tuple<Elements..., T>;
};

template<typename T>
struct tuple_erase<T, std::tuple<> >{
    using type = std::tuple<>;
};
template<typename T, typename...Elements>
struct tuple_erase<T, std::tuple<T, Elements...> >{
    using type = std::tuple<Elements...>;
};

template<typename T,typename U,  typename...Elements>
struct tuple_erase<T, std::tuple<U, Elements...> >{
    using type = typename tuple_prefix<U, typename tuple_erase<T, std::tuple<Elements...>>::type>::type;
};

template<typename T>
struct tuple_erase_all<T, std::tuple<> >{
    using type = std::tuple<>;
};
template<typename T, typename...Elements>
struct tuple_erase_all<T, std::tuple<T, Elements...> >{
    using type = typename tuple_erase_all<T, std::tuple<Elements...>>::type;
};

template<typename T,typename U,  typename...Elements>
struct tuple_erase_all<T, std::tuple<U, Elements...> >{
    using type = typename tuple_prefix<U, typename tuple_erase_all<T, std::tuple<Elements...>>::type>::type;
};

template<size_t N>
struct tuple_erase_idx<N, std::tuple<>>{
    static_assert(N>=0, "tuple_erase_idx, the index is too large.");
};

template<typename T, typename... Elements>
struct tuple_erase_idx<0, std::tuple<T, Elements...>>{
    using type = std::tuple<Elements...>;
};

template<size_t N, typename T, typename... Elements>
struct tuple_erase_idx<N, std::tuple<T, Elements...>>{
    using type = typename tuple_prefix<T, typename tuple_erase_idx<N-1, std::tuple<Elements...>>::type>::type;
};

template<>
struct tuple_erase_dup<std::tuple<>>{
    using type = std::tuple<>;
};

template<typename T, typename ...Elements>
struct tuple_erase_dup<std::tuple<T, Elements...>>{
    using type = typename tuple_prefix<T, typename tuple_erase_dup<typename tuple_erase_all<T, std::tuple<Elements...>>::type>::type>::type;
};

template<typename T>
struct tuple_insert<0, T, std::tuple<>>{
    using type = std::tuple<T>;
};
template<typename T, typename... Elements>
struct tuple_insert<0, T, std::tuple<Elements...>>{
    using type = std::tuple<T, Elements...>;
};
template<size_t N, typename T, typename U, typename... Elements>
struct tuple_insert<N, T, std::tuple<U, Elements...>>{
    using type = typename tuple_prefix<U, typename tuple_insert<N-1, T, std::tuple<Elements...>>::type>::type;
};

template<typename From, typename Tuple>
struct tuple_replace<From, From, Tuple>{
    using type = Tuple;
};
template<typename From,typename To>
struct tuple_replace<From, To, std::tuple<>>{
    using type = std::tuple<>;
};
template<typename From,typename To, typename ...Elements>
struct tuple_replace<From, To, std::tuple<From, Elements...>>{
    using type = std::tuple<To, Elements...>;
};
template<typename From,typename To, typename T, typename ...Elements>
struct tuple_replace<From, To, std::tuple<T, Elements...>>{
    using type = typename tuple_prefix<T, tuple_replace<From, To, std::tuple<Elements...>>::type>::type;
};

} //namespace xusd
#endif
