// -*- C++ -*-

#ifndef __FORK_STL__CHRONO__
#define __FORK_STL__CHRONO__
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <type_traits>
#include <ratio.hpp>

namespace xusd{
    namespace chrono{
        template <class _Rep, class _Period> class duration;
    }
}

namespace std {
    template <class _Rep1, class _Period1, class _Rep2, class _Period2>
    struct common_type<xusd::chrono::duration<_Rep1, _Period1>,
    xusd::chrono::duration<_Rep2, _Period2> > {
        typedef xusd::chrono::duration<typename common_type<_Rep1, _Rep2>::type, typename xusd::__ratio_gcd<_Period1, _Period2>::type> type;
    };
}

namespace xusd {

    // -- chrono {{{
    namespace chrono {
        template <class _Rep, class _Period> class duration;

        template <class _Tp>
        struct __is_duration : std::false_type {};

        template <class _Rep, class _Period>
        struct __is_duration<duration<_Rep, _Period> > : std::true_type  {};

        template <class _Rep, class _Period>
        struct __is_duration<const duration<_Rep, _Period> > : std::true_type  {};

        template <class _Rep, class _Period>
        struct __is_duration<volatile duration<_Rep, _Period> > : std::true_type  {};

        template <class _Rep, class _Period>
        struct __is_duration<const volatile duration<_Rep, _Period> > : std::true_type  {};


        template <class _FromDuration, class _ToDuration,
                  class _Period = typename ratio_divide<typename _FromDuration::period, typename _ToDuration::period>::type,
                  bool = _Period::num == 1,
                  bool = _Period::den == 1>
        struct __duration_cast;

        template <class _FromDuration, class _ToDuration, class _Period>
        struct __duration_cast<_FromDuration, _ToDuration, _Period, true, true> {
            constexpr _ToDuration operator()(const _FromDuration& __fd) const {
                return _ToDuration(static_cast<typename _ToDuration::rep>(__fd.count()));
            }
        };

        template <class _FromDuration, class _ToDuration, class _Period>
        struct __duration_cast<_FromDuration, _ToDuration, _Period, true, false> {
            constexpr _ToDuration operator()(const _FromDuration& __fd) const {
                typedef typename std::common_type<typename _ToDuration::rep, typename _FromDuration::rep, intmax_t>::type _Ct;
                return _ToDuration(static_cast<typename _ToDuration::rep>(static_cast<_Ct>(__fd.count()) / static_cast<_Ct>(_Period::den)));
            }
        };

        template <class _FromDuration, class _ToDuration, class _Period>
        struct __duration_cast<_FromDuration, _ToDuration, _Period, false, true> {
            constexpr _ToDuration operator()(const _FromDuration& __fd) const {
                typedef typename std::common_type<typename _ToDuration::rep, typename _FromDuration::rep, intmax_t>::type _Ct;
                return _ToDuration(static_cast<typename _ToDuration::rep>(static_cast<_Ct>(__fd.count()) * static_cast<_Ct>(_Period::num)));
            }
        };

        template <class _FromDuration, class _ToDuration, class _Period>
        struct __duration_cast<_FromDuration, _ToDuration, _Period, false, false> {
            constexpr _ToDuration operator()(const _FromDuration& __fd) const {
                typedef typename std::common_type<typename _ToDuration::rep, typename _FromDuration::rep, intmax_t>::type _Ct;
                return _ToDuration(static_cast<typename _ToDuration::rep>(static_cast<_Ct>(__fd.count()) * static_cast<_Ct>(_Period::num) / static_cast<_Ct>(_Period::den)));
            }
        };

        template <class _ToDuration, class _Rep, class _Period>
        typename std::enable_if
        <
            __is_duration<_ToDuration>::value,
            _ToDuration
        >::type constexpr
        duration_cast(const duration<_Rep, _Period>& __fd) {
            return __duration_cast<duration<_Rep, _Period>, _ToDuration>()(__fd);
        }

        template <class Rep> struct treat_as_floating_point : std::is_floating_point<Rep> {};

        template <class Rep>
        struct duration_values {
        public:
            static constexpr Rep zero(){
                return 0;
            }
            static constexpr Rep max(){
                return std::numeric_limits<Rep>::max();
            }
            static constexpr Rep min(){
                return std::numeric_limits<Rep>::min();
            }
        };

        // duration
        template <class Rep, class Period = ratio<1>>
        class duration {
            static_assert(!__is_duration<Rep>::value, "A duration representation can not be a duration");
            static_assert(__is_ratio<Period>::value, "Second template parameter of duration must be a xusd::ratio");
            static_assert(Period::num > 0, "duration period must be positive");
        public:
            typedef Rep rep;
            typedef Period period;
        private:
            rep __rep_{0};
        public:
            constexpr duration() = default;
            template <class Rep2>
            constexpr explicit duration(const Rep2& r,
                typename std::enable_if
                <
                   std::is_convertible<Rep2, rep>::value &&
                   (treat_as_floating_point<rep>::value ||
                   (!treat_as_floating_point<rep>::value && !treat_as_floating_point<Rep2>::value))
                >::type* = 0): __rep_(r){ }

            // conversions
            template <class Rep2, class Period2>
            constexpr duration(const duration<Rep2, Period2>& __d,
                typename std::enable_if
                <
                    treat_as_floating_point<rep>::value ||
                    ratio_divide<Period2, period>::type::den == 1
                >::type* = 0):__rep_(duration_cast<duration>(__d).count()){ }

            // observer

            constexpr rep count() const{ return __rep_; }

            // arithmetic

            constexpr duration  operator+() const{
                return *this;
            }
            constexpr duration  operator-() const{
                return duration(-__rep_);
            }
            duration& operator++(){
                ++__rep_;
                return *this;
            }
            duration operator++(int){
                duration ret(__rep_);
                ++__rep_;
                return ret;
            }
            duration& operator--(){
                --__rep_;
                return *this;
            }
            duration  operator--(int){
                duration ret(__rep_);
                --__rep_;
                return ret;
            }

            duration& operator+=(const duration& d){
                __rep_ += d.__rep_;
                return *this;
            }
            duration& operator-=(const duration& d){
                __rep_ -= d.__rep_;
                return *this;
            }

            duration& operator*=(const rep& rhs){
                __rep_ *= rhs.__rep_;
                return *this;
            }
            duration& operator/=(const rep& rhs){
                __rep_ /= rhs.__rep_;
                return *this;
            }

            // special values

            static constexpr duration zero(){
                return duration(0);
            }
            static constexpr duration min(){
                return duration(std::numeric_limits<rep>::min());
            }
            static constexpr duration max(){
                return duration(std::numeric_limits<rep>::max());
            }
        };

        typedef duration<long long,         nano> nanoseconds;
        typedef duration<long long,        micro> microseconds;
        typedef duration<long long,        milli> milliseconds;
        typedef duration<long long              > seconds;
        typedef duration<     long, ratio<  60> > minutes;
        typedef duration<     long, ratio<3600> > hours;


        // Duration ==
        template <class _LhsDuration, class _RhsDuration>
        struct __duration_eq {
            constexpr bool operator()(const _LhsDuration& __lhs, const _RhsDuration& __rhs) const {
                    typedef typename std::common_type<_LhsDuration, _RhsDuration>::type _Ct;
                    return _Ct(__lhs).count() == _Ct(__rhs).count();
                }
        };

        template <class _LhsDuration>
        struct __duration_eq<_LhsDuration, _LhsDuration> {
            constexpr bool operator()(const _LhsDuration& __lhs, const _LhsDuration& __rhs) const {
                return __lhs.count() == __rhs.count();
            }
        };

        template <class _Rep1, class _Period1, class _Rep2, class _Period2>
        inline constexpr bool operator==(const duration<_Rep1, _Period1>& __lhs, const duration<_Rep2, _Period2>& __rhs) {
            return __duration_eq<duration<_Rep1, _Period1>, duration<_Rep2, _Period2> >()(__lhs, __rhs);
        }

        // Duration !=

        template <class _Rep1, class _Period1, class _Rep2, class _Period2>
        inline constexpr bool operator!=(const duration<_Rep1, _Period1>& __lhs, const duration<_Rep2, _Period2>& __rhs) {
            return !(__lhs == __rhs);
        }

        // Duration <

        template <class _LhsDuration, class _RhsDuration>
        struct __duration_lt {
            constexpr bool operator()(const _LhsDuration& __lhs, const _RhsDuration& __rhs) const {
                typedef typename std::common_type<_LhsDuration, _RhsDuration>::type _Ct;
                return _Ct(__lhs).count() < _Ct(__rhs).count();
            }
        };

        template <class _LhsDuration>
        struct __duration_lt<_LhsDuration, _LhsDuration>
        {
            constexpr bool operator()(const _LhsDuration& __lhs, const _LhsDuration& __rhs) const
                {return __lhs.count() < __rhs.count();}
        };

        template <class _Rep1, class _Period1, class _Rep2, class _Period2>
        inline 
        constexpr bool
        operator< (const duration<_Rep1, _Period1>& __lhs, const duration<_Rep2, _Period2>& __rhs)
        {
            return __duration_lt<duration<_Rep1, _Period1>, duration<_Rep2, _Period2> >()(__lhs, __rhs);
        }

        // Duration >

        template <class _Rep1, class _Period1, class _Rep2, class _Period2>
        inline constexpr bool operator> (const duration<_Rep1, _Period1>& __lhs, const duration<_Rep2, _Period2>& __rhs) {
            return __rhs < __lhs;
        }

        // Duration <=

        template <class _Rep1, class _Period1, class _Rep2, class _Period2>
        inline constexpr bool operator<=(const duration<_Rep1, _Period1>& __lhs, const duration<_Rep2, _Period2>& __rhs) {
            return !(__rhs < __lhs);
        }

        // Duration >=

        template <class _Rep1, class _Period1, class _Rep2, class _Period2>
        inline constexpr bool operator>=(const duration<_Rep1, _Period1>& __lhs, const duration<_Rep2, _Period2>& __rhs) {
            return !(__lhs < __rhs);
        }

        // Duration +

        template <class _Rep1, class _Period1, class _Rep2, class _Period2>
        inline constexpr typename std::common_type<duration<_Rep1, _Period1>, duration<_Rep2, _Period2> >::type
        operator+(const duration<_Rep1, _Period1>& __lhs, const duration<_Rep2, _Period2>& __rhs) {
            typedef typename std::common_type<duration<_Rep1, _Period1>, duration<_Rep2, _Period2> >::type _Cd;
            return _Cd(_Cd(__lhs).count() + _Cd(__rhs).count());
        }

        // Duration -

        template <class _Rep1, class _Period1, class _Rep2, class _Period2>
        inline 
        constexpr
        typename std::common_type<duration<_Rep1, _Period1>, duration<_Rep2, _Period2> >::type
        operator-(const duration<_Rep1, _Period1>& __lhs, const duration<_Rep2, _Period2>& __rhs) {
            typedef typename std::common_type<duration<_Rep1, _Period1>, duration<_Rep2, _Period2> >::type _Cd;
            return _Cd(_Cd(__lhs).count() - _Cd(__rhs).count());
        }

        // Duration *

        template <class _Rep1, class _Period, class _Rep2>
        inline 
        constexpr
        typename std::enable_if
        <
            std::is_convertible<_Rep2, typename std::common_type<_Rep1, _Rep2>::type>::value,
            duration<typename std::common_type<_Rep1, _Rep2>::type, _Period>
        >::type
        operator*(const duration<_Rep1, _Period>& __d, const _Rep2& __s) {
            typedef typename std::common_type<_Rep1, _Rep2>::type _Cr;
            typedef duration<_Cr, _Period> _Cd;
            return _Cd(_Cd(__d).count() * static_cast<_Cr>(__s));
        }

        template <class _Rep1, class _Period, class _Rep2>
        inline 
        constexpr
        typename std::enable_if
        <
            std::is_convertible<_Rep1, typename std::common_type<_Rep1, _Rep2>::type>::value,
            duration<typename std::common_type<_Rep1, _Rep2>::type, _Period>
        >::type
        operator*(const _Rep1& __s, const duration<_Rep2, _Period>& __d) { return __d * __s;
        }

        // Duration /

        template <class _Duration, class _Rep, bool = __is_duration<_Rep>::value>
        struct __duration_divide_result { };

        template <class _Duration, class _Rep2,
            bool = std::is_convertible<_Rep2,
                                  typename std::common_type<typename _Duration::rep, _Rep2>::type>::value>
        struct __duration_divide_imp { };

        template <class _Rep1, class _Period, class _Rep2>
        struct __duration_divide_imp<duration<_Rep1, _Period>, _Rep2, true> {
            typedef duration<typename std::common_type<_Rep1, _Rep2>::type, _Period> type;
        };

        template <class _Rep1, class _Period, class _Rep2>
        struct __duration_divide_result<duration<_Rep1, _Period>, _Rep2, false>
            : __duration_divide_imp<duration<_Rep1, _Period>, _Rep2> { };

        template <class _Rep1, class _Period, class _Rep2>
        inline constexpr typename __duration_divide_result<duration<_Rep1, _Period>, _Rep2>::type
        operator/(const duration<_Rep1, _Period>& __d, const _Rep2& __s)
        {
            typedef typename std::common_type<_Rep1, _Rep2>::type _Cr;
            typedef duration<_Cr, _Period> _Cd;
            return _Cd(_Cd(__d).count() / static_cast<_Cr>(__s));
        }

        template <class _Rep1, class _Period1, class _Rep2, class _Period2>
        inline constexpr typename std::common_type<_Rep1, _Rep2>::type
        operator/(const duration<_Rep1, _Period1>& __lhs, const duration<_Rep2, _Period2>& __rhs)
        {
            typedef typename std::common_type<duration<_Rep1, _Period1>, duration<_Rep2, _Period2> >::type _Ct;
            return _Ct(__lhs).count() / _Ct(__rhs).count();
        }

        // Duration %
        template <class _Rep1, class _Period, class _Rep2>
        inline constexpr typename __duration_divide_result<duration<_Rep1, _Period>, _Rep2>::type
        operator%(const duration<_Rep1, _Period>& __d, const _Rep2& __s) {
            typedef typename std::common_type<_Rep1, _Rep2>::type _Cr;
            typedef duration<_Cr, _Period> _Cd;
            return _Cd(_Cd(__d).count() % static_cast<_Cr>(__s));
        }

        template <class _Rep1, class _Period1, class _Rep2, class _Period2>
        inline constexpr typename std::common_type<duration<_Rep1, _Period1>, duration<_Rep2, _Period2> >::type
        operator%(const duration<_Rep1, _Period1>& __lhs, const duration<_Rep2, _Period2>& __rhs) {
            typedef typename std::common_type<_Rep1, _Rep2>::type _Cr;
            typedef typename std::common_type<duration<_Rep1, _Period1>, duration<_Rep2, _Period2> >::type _Cd;
            return _Cd(static_cast<_Cr>(_Cd(__lhs).count()) % static_cast<_Cr>(_Cd(__rhs).count()));
        }

        template <class Clock, class Duration = typename Clock::duration>
        class time_point {
        public:
            typedef Clock                     clock;
            typedef Duration                  duration;
            typedef typename duration::rep    rep;
            typedef typename duration::period period;
        private:
            duration __d_;  // exposition only

        public:
            time_point():__d_(duration::zero()){ }

            explicit time_point(const duration& d):__d_(d){ } 
            // conversions
            template <class _Duration2>
            time_point(const time_point<clock, _Duration2>& t,
                typename std::enable_if
                <
                    std::is_convertible<_Duration2, duration>::value
                >::type* = 0)
            : __d_(t.time_since_epoch()) {}

            // observer
            duration time_since_epoch() const{
                return __d_;
            }

            // arithmetic
            time_point& operator+=(const duration& d){
                __d_+=d; 
                return *this;
            }
            time_point& operator-=(const duration& d){
                __d_+=d;
                return *this;
            }

            // special values
            static constexpr time_point min();
            static constexpr time_point max();
        };

        // Clocks
        class system_clock {
        public:
            typedef microseconds                     duration;
            typedef duration::rep                    rep;
            typedef duration::period                 period;
            typedef xusd::chrono::time_point<system_clock> time_point;
            static const bool is_steady =            false; // constexpr in C++14

            static time_point now() noexcept{
                timeval tv;
                gettimeofday(&tv, 0);
                return time_point(seconds(tv.tv_sec) +microseconds(tv.tv_usec));
            }
            static time_t to_time_t (const time_point& __t) noexcept{
                return time_t(duration_cast<seconds>(__t.time_since_epoch()).count());
            }
            static time_point from_time_t(time_t __t) noexcept{
                return time_point(seconds(__t));
            }
        };

        class steady_clock
        {
        public:
            typedef nanoseconds                                   duration;
            typedef duration::rep                                 rep;
            typedef duration::period                              period;
            typedef xusd::chrono::time_point<steady_clock, duration>    time_point;
            static const bool is_steady =                         true; // constexpr in C++14

            static time_point now() noexcept{
                struct timespec t;
                clock_gettime(CLOCK_MONOTONIC, &t);
                return time_point(seconds(t.tv_sec) + nanoseconds(t.tv_nsec));
            }
        };

        class process_clock
        {
        public:
            typedef nanoseconds                                   duration;
            typedef duration::rep                                 rep;
            typedef duration::period                              period;
            typedef xusd::chrono::time_point<process_clock, duration>    time_point;
            static const bool is_steady =                         true; // constexpr in C++14

            static time_point now() noexcept{
                struct timespec t;
                clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);
                return time_point(seconds(t.tv_sec) + nanoseconds(t.tv_nsec));
            }
        };

        class thread_clock
        {
        public:
            typedef nanoseconds                                   duration;
            typedef duration::rep                                 rep;
            typedef duration::period                              period;
            typedef xusd::chrono::time_point<thread_clock, duration>    time_point;
            static const bool is_steady =                         true; // constexpr in C++14

            static time_point now() noexcept{
                struct timespec t;
                clock_gettime(CLOCK_THREAD_CPUTIME_ID, &t);
                return time_point(seconds(t.tv_sec) + nanoseconds(t.tv_nsec));
            }
        };

        typedef steady_clock high_resolution_clock;

        // time_point arithmetic (all constexpr in C++14)
        template <class Clock, class Duration1, class Rep2, class Period2>
        time_point<Clock, typename std::common_type<Duration1, duration<Rep2, Period2>>::type>
        operator+(const time_point<Clock, Duration1>& lhs, const duration<Rep2, Period2>& rhs){
            return time_point<Clock, typename std::common_type<Duration1, duration<Rep2, Period2>>::type>(lhs + rhs);
        }

        template <class Rep1, class Period1, class Clock, class Duration2>
        time_point<Clock, typename std::common_type<duration<Rep1, Period1>, Duration2>::type>
        operator+(const duration<Rep1, Period1>& lhs, const time_point<Clock, Duration2>& rhs){
            return time_point<Clock, typename std::common_type<duration<Rep1, Period1>, Duration2>::type>(rhs + lhs);
        }

        template <class Clock, class Duration1, class Rep2, class Period2>
        time_point<Clock, typename std::common_type<Duration1, duration<Rep2, Period2>>::type>
        operator-(const time_point<Clock, Duration1>& lhs, const duration<Rep2, Period2>& rhs){
        return time_point<Clock, typename std::common_type<Duration1, duration<Rep2, Period2>>::type>(lhs + rhs);
        }

        template <class Clock, class Duration1, class Duration2>
        typename std::common_type<Duration1, Duration2>::type
        operator-(const time_point<Clock, Duration1>& lhs, const time_point<Clock, Duration2>& rhs){
            return typename std::common_type<Duration1, Duration2>::type(lhs.time_since_epoch() - rhs.time_since_epoch());
        }

        // time_point comparisons (all constexpr in C++14)
        template <class Clock, class Duration1, class Duration2>
        bool operator==(const time_point<Clock, Duration1>& lhs, const time_point<Clock, Duration2>& rhs){
            return lhs.time_since_epoch() == rhs.time_since_epoch();
        }
        template <class Clock, class Duration1, class Duration2>
        bool operator!=(const time_point<Clock, Duration1>& lhs, const time_point<Clock, Duration2>& rhs){
            return !(lhs == rhs);
        }
        template <class Clock, class Duration1, class Duration2>
        bool operator< (const time_point<Clock, Duration1>& lhs, const time_point<Clock, Duration2>& rhs){
            return lhs.time_since_epoch() < rhs.time_since_epoch();
        }
        template <class Clock, class Duration1, class Duration2>
        bool operator<=(const time_point<Clock, Duration1>& lhs, const time_point<Clock, Duration2>& rhs){
            return lhs < rhs || lhs == rhs;
        }
        template <class Clock, class Duration1, class Duration2>
        bool operator> (const time_point<Clock, Duration1>& lhs, const time_point<Clock, Duration2>& rhs){
            return !(lhs<=rhs);
        }
        template <class Clock, class Duration1, class Duration2>
        bool operator>=(const time_point<Clock, Duration1>& lhs, const time_point<Clock, Duration2>& rhs){
            return !(lhs<rhs);
        }

        //// time_point_cast (constexpr in C++14)
        template <class ToDuration, class Clock, class Duration>
        time_point<Clock, ToDuration> time_point_cast(const time_point<Clock, Duration>& t){
            return time_point<Clock, ToDuration>(duration_cast<ToDuration>(t.time_since_epoch()));
        }

    } // namespace chrono
    //constexpr chrono::hours                                 operator "" h(unsigned long long); // C++14
    //constexpr chrono::duration<unspecified , ratio<3600,1>> operator "" h(long double); // C++14
    //constexpr chrono::minutes                               operator "" min(unsigned long long); // C++14
    //constexpr chrono::duration<unspecified , ratio<60,1>>   operator "" min(long double); // C++14
    //constexpr chrono::seconds                               operator "" s(unsigned long long); // C++14
    //constexpr chrono::duration<unspecified >                operator "" s(long double); // C++14
    //constexpr chrono::milliseconds                          operator "" ms(unsigned long long); // C++14
    //constexpr chrono::duration<unspecified , milli>         operator "" ms(long double); // C++14
    //constexpr chrono::microseconds                          operator "" us(unsigned long long); // C++14
    //constexpr chrono::duration<unspecified , micro>         operator "" us(long double); // C++14
    //constexpr chrono::nanoseconds                           operator "" ns(unsigned long long); // C++14
    //constexpr chrono::duration<unspecified , nano>          operator "" ns(long double); // C++14

    //}}} =====================chrono
}
#endif
