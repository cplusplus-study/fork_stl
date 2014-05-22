#include <type_traits>
#include <functional>
#include <gtest/gtest.h>

template <typename F>
struct func_traits: func_traits<decltype(&std::remove_reference<F>::type::operator())>{ };

template<typename C, typename R, typename... Args>
struct func_traits<R(C::*)(Args...)>{
    typedef R signature(Args...);
};

template<typename C, typename R, typename... Args>
struct func_traits<R(C::*)(Args...) const>{
    typedef R signature(Args...);
};


template<typename F>
std::function<typename func_traits<F>::signature> to_func(F &&f){
    return std::function<typename func_traits<F>::signature>(std::forward<F>(f));
}

TEST(TestLambdaToFunc, Test1){
    auto r1 = to_func([](){ });
    EXPECT_TRUE((std::is_same<std::function<void()>, decltype(r1)>::value));
    auto r2 = to_func([](int a, int b){ return a+b; });
    EXPECT_TRUE((std::is_same<std::function<int(int, int)>, decltype(r2)>::value));
}
int main(int argc, char* argv[]){


    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
