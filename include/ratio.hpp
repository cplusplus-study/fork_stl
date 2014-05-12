#include <cstdint>
#include <climits>
#include <limits>
#include <type_traits>

namespace xusd{

    // -- gen {{{ 
    template <int ...N> struct seq{ };
    template <unsigned N, unsigned...S> struct gen;
    template <unsigned N, unsigned...S> struct gen: gen<N-1, N-1, S...>{ };
    template <unsigned...S> struct gen<0, S...>{ typedef seq<S...> type; };
    // }}}

    // -- ratio {{{ 
    template <intmax_t _X, intmax_t _Y>
    struct _gcd{
        const static intmax_t value = _gcd<_Y, _X % _Y>::value;
    };

    template <intmax_t _X>
    struct _gcd<_X, 0>{
        const static intmax_t value = _X;
    };
    template <>
    struct _gcd<0, 0>{
        const static intmax_t value = 1LL;
    };

    template <intmax_t _X, intmax_t _Y>
    struct _lcm{
        const static intmax_t value = _X * _Y / _gcd<_X, _Y>::value;
    };

    template <intmax_t _X>
    struct _sign{
        const static intmax_t value = _X == 0? 0 : (_X < 0 ? -1 : 1);
    };

    template <intmax_t _X>
    struct _abs{
        const static intmax_t value = _X < 0 ? (-_X) : _X;
    };

    template <intmax_t X, intmax_t Y, intmax_t S = _sign<Y>::value>
    struct _is_add_overflow;

    template <intmax_t X, intmax_t Y>
    struct _is_add_overflow<X, Y, 0>{
        const static bool value = false;
    };

    template <intmax_t X, intmax_t Y>
    struct _is_add_overflow<X, Y, 1>{
        const static intmax_t max = -((1LL << (sizeof(intmax_t)*CHAR_BIT - 1)) + 1);
        const static bool value = X < (max - Y);
    };

    template <intmax_t X, intmax_t Y>
    struct _is_add_overflow<X, Y, -1>{
        const static intmax_t min = (1LL << (sizeof(intmax_t)*CHAR_BIT - 1));
        const static bool value = X > (min - Y);
    };

    template <intmax_t X, intmax_t Y, intmax_t S = _sign<Y>::value>
    struct _is_sub_overflow;

    template <intmax_t X, intmax_t Y>
    struct _is_sub_overflow<X, Y, 0>{
        const static bool value = false;
    };

    template <intmax_t X, intmax_t Y>
    struct _is_sub_overflow<X, Y, 1>{
        const static intmax_t min = (1LL << (sizeof(intmax_t)*CHAR_BIT - 1));
        const static bool value = X > (min + Y);
    };

    template <intmax_t X, intmax_t Y>
    struct _is_sub_overflow<X, Y, -1>{
        const static intmax_t max = -((1LL << (sizeof(intmax_t)*CHAR_BIT - 1)) + 1);
        const static bool value = X < (max + Y);
    };

    template <intmax_t X, intmax_t Y>
    struct _is_mul_overflow{
        const static intmax_t max = -((1LL << (sizeof(intmax_t)*CHAR_BIT - 1)) + 1);
        const static bool value = _abs<X>::value < _abs<max / Y>::value;
    };

    template <intmax_t X>
    struct _is_mul_overflow<X, 0>{
        const static bool value = false;
    };

    template <intmax_t X, intmax_t Y>
    struct _is_div_overflow{
        const static intmax_t min = (1LL << (sizeof(intmax_t)*CHAR_BIT - 1));
        const static bool value = (Y != 0) && !(X == min);
    };

    template <intmax_t Num, intmax_t Den = 1LL>
    class ratio{
        static_assert(Den!=0LL, "ratio denominator can not is 0!");
    public:
        const static intmax_t num = _sign<Num>::value * _sign<Den>::value * _abs<Num>::value / _gcd<_abs<Num>::value, _abs<Den>::value>::value;
        const static intmax_t den = _abs<Den>::value / _gcd<_abs<Num>::value, _abs<Den>::value>::value;
        typedef ratio<num, num == 0 ? 1 : den> type;
    };

    template <class _R1, class _R2>
    struct __ratio_gcd {
        typedef ratio<_gcd<_R1::num, _R2::num>::value,
                      _lcm<_R1::den, _R2::den>::value> type;
    };
    template <intmax_t Num, intmax_t Den> const intmax_t ratio<Num,Den>::num;
    template <intmax_t Num, intmax_t Den> const intmax_t ratio<Num,Den>::den;

    //TODO +,-,*,/ 做溢出处理.
    template <typename R1, typename R2>
    struct ratio_add{
        static_assert((_is_mul_overflow<R1::num,_lcm<R1::den, R2::den>::value>::value ||
                _is_mul_overflow<R2::num, _lcm<R1::den, R2::den>::value>::value ||
                _is_div_overflow<R1::num * _lcm<R1::den, R2::den>::value, R1::den>::value ||
                _is_div_overflow<R2::num * _lcm<R1::den, R2::den>::value, R2::den>::value)
                , "ratio add is overflow");
        typedef typename ratio<R1::num * _lcm<R1::den, R2::den>::value/R1::den + R2::num * _lcm<R1::den, R2::den>::value/R2::den, _lcm<R1::den, R2::den>::value>::type type;
    };
    template <typename R1, typename R2>
    struct ratio_subtract{
        static_assert((_is_mul_overflow<R1::num,_lcm<R1::den, R2::den>::value>::value ||
                _is_mul_overflow<R2::num, _lcm<R1::den, R2::den>::value>::value ||
                _is_div_overflow<R1::num * _lcm<R1::den, R2::den>::value, R1::den>::value ||
                _is_div_overflow<R2::num * _lcm<R1::den, R2::den>::value, R2::den>::value)
                , "ratio sub is overflow");
        typedef typename ratio<R1::num * _lcm<R1::den, R2::den>::value/R1::den - R2::num * _lcm<R1::den, R2::den>::value/R2::den, _lcm<R1::den, R2::den>::value>::type type;
    };
    template <typename R1, typename R2>
    struct ratio_multiply{
        static_assert((_is_mul_overflow<R1::num, R2::num>::value || _is_mul_overflow<R1::den, R2::den>::value), "ratio multiply is overflow");
        typedef typename ratio<R1::num * R2::num, R1::den * R2::den>::type type;
    };
    template <typename R1, typename R2>
    struct ratio_divide{
        static_assert(R2::num != 0, "ratio divide 0");
        static_assert((_is_mul_overflow<R1::num, R2::den>::value || _is_mul_overflow<R1::den, R2::num>::value), "ratio devide is overflow");
        typedef typename ratio<R1::num * R2::den, R1::den * R2::num>::type type;
    };


    template <typename R1, typename R2>
    struct ratio_equal: public std::integral_constant<bool,R1::num * R2::den == R2::num * R1::den>{ };
      
    template <typename R1, typename R2>
    struct ratio_not_equal: public std::integral_constant<bool, !ratio_equal<R1, R2>::value>{ };
      
    template <typename R1, typename R2>
    struct ratio_less: public std::integral_constant<bool,(R1::num * R2::den < R2::num * R1::den)>{ };
      
    template <typename R1, typename R2>
    struct ratio_less_equal: public std::integral_constant<bool,(R1::num * R2::den <= R2::num * R1::den)>{ };
      
    template <typename R1, typename R2>
    struct ratio_greater: public std::integral_constant<bool,(R1::num * R2::den > R2::num * R1::den)>{ };
      
    template <typename R1, typename R2>
    struct ratio_greater_equal: public std::integral_constant<bool,(R1::num * R2::den >= R2::num * R1::den)>{ };

    typedef ratio<1LL, 1000000000000000000LL> atto;
    typedef ratio<1LL,    1000000000000000LL> femto;
    typedef ratio<1LL,       1000000000000LL> pico;
    typedef ratio<1LL,          1000000000LL> nano;
    typedef ratio<1LL,             1000000LL> micro;
    typedef ratio<1LL,                1000LL> milli;
    typedef ratio<1LL,                 100LL> centi;
    typedef ratio<1LL,                  10LL> deci;
    typedef ratio<                 10LL, 1LL> deca;
    typedef ratio<                100LL, 1LL> hecto;
    typedef ratio<               1000LL, 1LL> kilo;
    typedef ratio<            1000000LL, 1LL> mega;
    typedef ratio<         1000000000LL, 1LL> giga;
    typedef ratio<      1000000000000LL, 1LL> tera;
    typedef ratio<   1000000000000000LL, 1LL> peta;
    typedef ratio<1000000000000000000LL, 1LL> exa;

    template <class _Tp>                    struct __is_ratio                     : std::false_type {};
    template <intmax_t _Num, intmax_t _Den> struct __is_ratio<ratio<_Num, _Den> > : std::true_type  {};
    //}}} ratio
}
