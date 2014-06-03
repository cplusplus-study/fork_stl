#include <cstdio>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iterator>

#include <iostream>
#include <string>
#include <memory>
#include <locale>

auto add = []( auto a, auto b ) a + b;

auto inc = []( auto x ) ++x;

// Composition.
auto comp = 
    []( auto f, auto g )
        [=]( auto x ) f( g(x) );

// Partial application.
auto part = 
    []( auto f, auto x ) 
        [=]<class ...Y>( Y&& ...y ) 
            f( x, std::forward<Y>(y)... );

auto curry3 = 
    []( auto f )
        [=](auto x) [=](auto y) [=](auto z) 
            f(x,y,z);

auto for_each = []( auto f, auto& s ) {
    for( auto& x : s )
        f(x);
};

template< class X >
using Singleton = std::tuple<X>;

auto singleton = []<class X>( X x ) Singleton<X>( std::move(x) );

auto singleton2 = 
    []( auto x ) Singleton <
        decltype(x)
    > ( std::move(x) );

// Get the singleton value.
auto svalue = []( auto single ) std::get<0>(single);

// Map the singleton.
auto smap = []( auto f, auto single ) 
    singleton( f( svalue(single) ) );
auto smap2 = []( auto f, auto single ) 
    singleton2( f( svalue(single) ) );

template< class X, class Y >
Singleton<Y> operator >> ( const Singleton<X>&, Singleton<Y> b ) {
    return std::move(b);
}

template< class X, class F >
auto operator >>= ( const Singleton<X>& x, const F& f ) 
    -> typename std::result_of< F(X) >::type
{
    return f( svalue(x) );
}

template< class F, class G, class ...X >
constexpr auto sfinae( const F& f, const G&, X&& ...x )
    -> decltype( f( std::forward<X>(x)... ) )
{
    return f( std::forward<X>(x)... );
}

template< class F, class G, class ...X >
constexpr auto sfinae( const F&, const G& g, X&& ...x )
    -> decltype( g( std::forward<X>(x)... ) )
{
    return g( std::forward<X>(x)... );
}

template< class F > struct Forwarder : F {
    constexpr Forwarder( const F& f ) : F(f) { }

    constexpr operator F() { return *this; }
    //using F::operator();
};

template< class R, class ...X > struct Forwarder<R(*)(X...)> {
    using type = R(*)(X...);
    type f;

    constexpr Forwarder( type f ) : f(f) { }

    constexpr R operator () ( X... x ) {
        return f( std::forward<X>(x)...);
    }
};

template< class F, class G >
struct Overloaded : Forwarder<F>, Forwarder<G> {
    constexpr Overloaded( const F& f, const G& g )
        : Forwarder<F>(f), Forwarder<G>(g)
    {
    }

    constexpr operator Forwarder<F>() { return *this; }
    constexpr operator Forwarder<G>() { return *this; }
};

template< class F > F overload( F&& f ) {
    return std::forward<F>(f);
}

template< class F, class G, class ...H,
          class O1 = Overloaded<F,G> > 
auto overload( const F& f, const G& g, const H& ...h ) 
    -> decltype( overload(O1(f,g),h...) )
{
    return overload( O1(f,g), h... );
}



template< class X, class F >
struct OverloadType : F {
    OverloadType( F f ) : F(f) { }

    using result_type = typename std::result_of< F(X) >::type;

    result_type operator () ( X x ) const {
        return F::operator()(x);
    }

    operator F() const { return *this; }
};

template< class ...X, class F >
auto overload_set( const F& f ) 
    -> decltype( overload(OverloadType<X,F>(f)...) )
{
    return overload( OverloadType<X,F>(f)... );
}

auto cout1 = []( const auto& x ) { std::cout << x; };

struct sequence_tag {};
struct other_tag {};

template< class S > auto get_tag(const S& s) -> decltype( std::begin(s), sequence_tag{} );
template< class S > auto get_tag(...) -> other_tag;

template< class Tag, class F > 
struct OverloadTag : F {
    constexpr OverloadTag( const F& f ) : F(f) { }

    template< class X >
    constexpr auto operator () ( X&& x ) const
        -> typename std::enable_if < 
            std::is_same< Tag, decltype(get_tag(x)) >::value,
            typename std::result_of< F(X) >::type 
        >::type
    {
        return F::operator()( std::forward<X>(x) );
    }

    operator F() const { return *this; }
};

template< class Tag, class F, class O = OverloadTag<Tag,F> > 
O overload_tag( const F& f ) {
    return O( f );
}

void print_int( int x ) { std::cout << "i_" << x; }

auto cprint = overload (
    []{},   // No input? No output.

    // Cannot be disambiguated from Singleton and vector overload.
    //[]( const auto& x ) { std::cout << x },

    &print_int,

    overload_set</*int,*/char,float,double,
                 const char* const,
                 const std::string&>( cout1 ),

    []<class X>( const Singleton<X>& s ) {
        std::cout << "{" << svalue(s) << "}";
    },

    overload_tag< sequence_tag > (
        []( const auto& s ) {
            std::cout << "[ ";
            for( const auto& x : s ) 
                std::cout << x << ' ';
            std::cout << ']';
        }
    )
);

// Variadic void unary.
auto vvu_impl = overload (
    [] (auto,auto) {},
    []<class X, class ...Y>( const auto& self, const auto& u, 
                             X&& x, Y&& ...y ) 
    {
        u( std::forward<X>(x) );
        self( self, u, std::forward<Y>(y)... );
    }
);

// vvu(vvu,f,x,y...) = f(x); f(y)...
auto vvu = []( const auto& u )
    [&]<class ...X>( const X& ...x )
        vvu_impl( vvu_impl, u, x... );

// Variadic print.
// vprint(x,y...) = cprint(x); cprint(y)...
auto vprint = vvu( cprint );

auto print_line = []<class ...X>( const X& ...x ) 
    vprint( x..., '\n' );

auto Make  = []<class X>( auto y ) X(y);

auto Const = []<int n>(auto) n;

auto If    = []<bool B>( auto x, auto y ) B ? x : y;
auto boolTest = 
    []< class X, bool isInt = std::is_same<X,int>::value >
    ( const X& x ) If.operator()<isInt>( x*2, x/2 );

auto x = boolTest(10);

auto boolTest2 = 
    []< class X, bool isInt = std::is_same<X,int>::value >
    ( X x ) If.operator()<isInt>( x*2, x/2 );

auto boolTest3 = 
    []( auto x ) {
        constexpr bool b = std::is_same< decltype(x), int >::value;
        return If.operator()<b>( x*2, x/2 );
    };


auto chainl_impl = overload (
    []( auto self, auto b, auto r ) { return r; },
    []<class ...Z>( auto self, auto b, auto x, auto y, Z ...z ) 
        self( self, b, b(x,y), z... )
);

auto chainl = []( auto b )
    [=]<class ...X>( const X& ...x )
        chainl_impl( chainl_impl, b, x... );

auto compose = chainl( comp );
auto sum = chainl( add );

auto min = overload (
    []( auto&& a, auto&& b ) { return a > b ? std::move(b) : std::move(a); },

    //[]( auto& a, auto& b ) -> decltype(a) a > b ? b : a, // expression result unused?
    []( auto& a, auto& b ) -> decltype(a) { return a > b ? b : a; },

    []( const auto& a, const auto& b ) -> decltype(a) a > b ? b : a 
);

void f() {
    const auto g = [](auto x, auto y) x + y;

    // Inner lambda alone cannot capture g.
    //auto f = []() [g]() g;
    //auto _g = f()();

    //std::cout << "ten : " << _g(5,5) << std::endl;
}


// Error here. (OK)
//auto embed = []( auto a ) []( auto b ) a + b;
//auto embed12 = embed(1)(2);

constexpr int max(int x, int y) { return x > y ? x : y; }



int two = 2;
auto plus_two = ([](auto x) [=](auto y) x + y)(two);
auto five = ([](auto x) [=](auto y) x + y)(two)(3);

auto zzz = ([](auto x) ([=](auto y) ([](auto x,auto y)x+y)(x,y))(x));

int main() {
    f();

    std::cout << "5+2 = " << plus_two(5) << std::endl;
    std::cout << "5+2 = " << five << std::endl;
    std::cout << "5+2 = " << zzz(5) << std::endl;

    int low = 5, high = 10;
    const int& low2 = min(low,high);
    const int& low3 = min(low2,high);

    std::cout << "\nmin(5,10) = " << low3 << std::endl;
    low++;
    std::cout << "min(6,10) = " << low3 << std::endl;

    using ivoid = void (*) ( const int& );
    using svoid = void (*) ( const std::string& );

    auto cout_is = overload( ivoid(cout1), svoid(cout1) );
    cout_is("\nten : "); cout_is(10); cout_is("\n\n");

    print_line( "10(2) = ", Const.operator()<10>(2) );
    print_line( "Sum of 9 and 10 : ", sum(9,10) ); 

    print_line( "Sum of 1, 2, and 3 : ", curry3(sum)(1)(2)(3) );

    print_line( "the char 'c': ", 'c' );

    auto addThree = compose( inc, inc, inc );
    print_line( "0 + 3 = ", addThree(0) );

    auto parter = part(comp,inc);
    print_line( "0 + 2 = ", parter(inc)(0) );

    std::vector<int> v = { 1, 2, 3, 4, 5 };
    print_line( "v = ", v );

    // Cannot recurse cprint.
    //std::vector<std::vector<int>> vv = { v, v, v, v };
    //print_line( "vv = ", vv );

    puts("");

    Singleton<char> schar{ 'a' };

    auto ord = []( char c ) (int) c;

    print_line( "ord {'a'} = ", smap(ord,schar) );
    print_line( "ord {'a'} = ", smap2(ord,schar) );

    print_line( "\nx <- {'a'}"
                "\ny <- {'b'}"
                "\n{x + y} = ",
                singleton('a') >>= [&](char a)
                singleton('b') >>= [&](char b)
                singleton( char(a + b) )
                );

    print_line( "True  = ", If.operator()<true >(1,0) );
    print_line( "False = ", If.operator()<false>(1,0) );
    //print_line( "2     = ", If.operator()<3>(1,0) ); // would this work anyway?

    puts( "\nLet f x = 2x if x is an int.\n"
            "Let f x = x/2 otherwise." );

    print_line( "f 10   = ", boolTest(10) );
    print_line( "f 10   = ", boolTest2(10) );
    print_line( "f 10   = ", boolTest3(10) );
    print_line( "f 10.0 = ", (int)boolTest(10.0) );

    print_line();

    []<class X>( X x ) -> typename std::enable_if <
        std::is_fundamental<X>::value, X
    >::type { return x; }(5);
}

