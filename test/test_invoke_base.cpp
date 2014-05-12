#include <type_traits>
#include <base/__invoke_base.hpp>
#include <gtest/gtest.h>

TEST(TestInvokeBase, __get_result_type){

    class A{}; class B{}; class C{}; class D{};
    EXPECT_TRUE((std::is_same< A, xusd::base::__get_result_type< A( B)>::type>::value));
    EXPECT_TRUE((std::is_same< A, xusd::base::__get_result_type< A(&)( B)>::type>::value));
    EXPECT_TRUE((std::is_same< A, xusd::base::__get_result_type< A(*)( B)>::type>::value));

    EXPECT_TRUE((std::is_same< A, xusd::base::__get_result_type< A(C::*)( B)>::type>::value));
    EXPECT_TRUE((std::is_same< A, xusd::base::__get_result_type< A(C::*)( B)const>::type>::value));
    EXPECT_TRUE((std::is_same< A, xusd::base::__get_result_type< A(C::*)( B)volatile>::type>::value));
    EXPECT_TRUE((std::is_same< A, xusd::base::__get_result_type< A(C::*)( B)const volatile>::type>::value));
}

int test_add(int a, int b){
    return a + b;
}

class AAA{
public:
    AAA(){ }
    int operator()(int a, int b){ return a+b; }
    double operator()(int a, int b, int c){ return a+b+c; }
    void operator()(int a){ }
    template<typename A1, typename A2>
    long operator()(A1 a, A2 b){
        return a+b;
    }
    int member{3};
    long long int memberFun(){
        return 1LL;
    }
    //long long int memberFun()const{
    //    return 2LL;
    //}
};
TEST(TestInvokeBase, __invoke){
    EXPECT_TRUE((std::is_same<int, decltype(xusd::base::__invoke(test_add, 1, 2))>::value));
    auto lam = [](int a, int b)->int{ return a+b; };
    EXPECT_TRUE((std::is_same<int, decltype(xusd::base::__invoke(lam, 1, 2))>::value));

    EXPECT_TRUE((std::is_same<void, decltype(xusd::base::__invoke(AAA(), 1))>::value));
    EXPECT_TRUE((std::is_same<int, decltype(xusd::base::__invoke(AAA(), 1, 2))>::value));
    EXPECT_TRUE((std::is_same<double, decltype(xusd::base::__invoke(AAA(), 1, 2, 3))>::value));
    EXPECT_TRUE((std::is_same<long, decltype(xusd::base::__invoke(AAA(), 1L, 2L))>::value));

    AAA a;
    EXPECT_TRUE((std::is_same<int&, decltype(xusd::base::__invoke(&AAA::member, &a))>::value));
    EXPECT_TRUE((std::is_same<int&, decltype(xusd::base::__invoke(&AAA::member, a))>::value));
    EXPECT_TRUE((std::is_same<int&&, decltype(xusd::base::__invoke(&AAA::member, std::move(a)))>::value));

    EXPECT_TRUE((std::is_same<long long int, decltype(xusd::base::__invoke(&AAA::memberFun, &a))>::value));
    EXPECT_TRUE((std::is_same<long long int, decltype(xusd::base::__invoke(&AAA::memberFun, a))>::value));
    EXPECT_TRUE((std::is_same<long long int, decltype(xusd::base::__invoke(&AAA::memberFun, std::move(a)))>::value));

    const AAA b;
    EXPECT_TRUE((std::is_same<const int&, decltype(xusd::base::__invoke(&AAA::member, &b))>::value));
    EXPECT_TRUE((std::is_same<const int&, decltype(xusd::base::__invoke(&AAA::member, b))>::value));
    EXPECT_TRUE((std::is_same<const int&&, decltype(xusd::base::__invoke(&AAA::member, std::move(b)))>::value));

}

int main(int argc, char* argv[]){

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
