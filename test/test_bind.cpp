///////////////////////////////////////////////////////////////////////////////
// 以下开始为测试代码.

#include <cstdlib>
#include <string>
#include <iostream>
#include <bind.hpp>
#include <gtest/gtest.h>

//int add3(int x, int y, int z){
//    return x + y + z;
//}
//
//std::string to_string(std::string s1, std::string s2, std::string s3){
//    return s1 + s2 + s3;
//}
//
//int g_test_voidfun =0;
//void voidfun(){
//    g_test_voidfun = 1;
//}
//
//class MyTest{
//public:
//    std::string to_string(std::string s1, std::string s2, std::string s3){
//        return s1 + s2 + s3;
//    }
//    int add3(int x, int y, int z){
//        return x + y + z;
//    }
//
//    int cadd3(int x, int y, int z) const {
//        return x + y + z;
//    }
//    void voidfun(){
//        g_test_voidfun = 2;
//    }
//    void constfun() const {
//        g_test_voidfun = 3;
//    }
//
//    int memObj = 0;
//    std::string memObj2;
//};
//
//class TestAddFuncter{
//public:
//    int operator()(int x, int y){
//        return x + y;
//    }
//};
//
//
//TEST(TestSeq,Test1){
//    using namespace xusd;
//    EXPECT_TRUE((std::is_same<gen<0>::type, seq<> >::value));
//    EXPECT_TRUE((std::is_same<gen<1>::type, seq<0> >::value));
//    EXPECT_TRUE((std::is_same<gen<2>::type, seq<0,1> >::value));
//    EXPECT_TRUE((std::is_same<gen<3>::type, seq<0,1,2> >::value));
//    EXPECT_TRUE((std::is_same<gen<4>::type, seq<0,1,2,3> >::value));
//    EXPECT_TRUE((std::is_same<gen<5>::type, seq<0,1,2,3,4> >::value));
//    EXPECT_TRUE((std::is_same<gen<6>::type, seq<0,1,2,3,4,5> >::value));
//    EXPECT_TRUE((std::is_same<gen<7>::type, seq<0,1,2,3,4,5,6> >::value));
//    EXPECT_TRUE((std::is_same<gen<8>::type, seq<0,1,2,3,4,5,6,7> >::value));
//    EXPECT_TRUE((std::is_same<gen<9>::type, seq<0,1,2,3,4,5,6,7,8> >::value));
//    EXPECT_TRUE((std::is_same<gen<10>::type, seq<0,1,2,3,4,5,6,7,8,9> >::value));
//}
//
//TEST(TestPlaceHolder, Test1){
//    using namespace xusd;
//    EXPECT_TRUE((std::is_same<decltype(_1), placeholder<1> >::value));
//    EXPECT_TRUE((std::is_same<decltype(_2), placeholder<2> >::value));
//    EXPECT_TRUE((std::is_same<decltype(_3), placeholder<3> >::value));
//    EXPECT_TRUE((std::is_same<decltype(_4), placeholder<4> >::value));
//    EXPECT_TRUE((std::is_same<decltype(_5), placeholder<5> >::value));
//    EXPECT_TRUE((std::is_same<decltype(_6), placeholder<6> >::value));
//    EXPECT_TRUE((std::is_same<decltype(_7), placeholder<7> >::value));
//    EXPECT_TRUE((std::is_same<decltype(_8), placeholder<8> >::value));
//    EXPECT_TRUE((std::is_same<decltype(_9), placeholder<9> >::value));
//
//    EXPECT_EQ(0, (is_placeholder<int>::value));
//    EXPECT_EQ(0, (is_placeholder<class A>::value));
//    EXPECT_EQ(1, (is_placeholder<decltype(_1)>::value));
//    EXPECT_EQ(2, (is_placeholder<decltype(_2)>::value));
//    EXPECT_EQ(3, (is_placeholder<decltype(_3)>::value));
//    EXPECT_EQ(4, (is_placeholder<decltype(_4)>::value));
//    EXPECT_EQ(5, (is_placeholder<decltype(_5)>::value));
//    EXPECT_EQ(6, (is_placeholder<decltype(_6)>::value));
//    EXPECT_EQ(7, (is_placeholder<decltype(_7)>::value));
//    EXPECT_EQ(8, (is_placeholder<decltype(_8)>::value));
//    EXPECT_EQ(9, (is_placeholder<decltype(_9)>::value));
//}
//
//TEST(TestSelectArgs, Test1){
//    using namespace xusd;
//    auto b = std::make_tuple(1,_1,2,_2,3,_3,4,_4);
//    auto c = std::make_tuple(11,22,33,44);
//
//    EXPECT_EQ(1, (select<0>(std::false_type(), b, c)));
//    EXPECT_EQ(11, (select<1>(std::true_type(), b, c)));
//    EXPECT_EQ(2, (select<2>(std::false_type(), b, c)));
//    EXPECT_EQ(22, (select<3>(std::true_type(), b, c)));
//    EXPECT_EQ(3, (select<4>(std::false_type(), b, c)));
//    EXPECT_EQ(33, (select<5>(std::true_type(), b, c)));
//    EXPECT_EQ(4, (select<6>(std::false_type(), b, c)));
//    EXPECT_EQ(44, (select<7>(std::true_type(), b, c)));
//}
//
//TEST(TestGetResult, Test1){
//    using namespace xusd;
//
//    class Ret;
//    class C;
//    class Args;
//    class Mem;
//    EXPECT_TRUE((std::is_same<void,xusd::base::__get_result_type<void()>::type>::value));
//    EXPECT_TRUE((std::is_same<int,xusd::base::__get_result_type<int(int)>::type>::value));
//    EXPECT_TRUE((std::is_same<const int,xusd::base::__get_result_type<const int()>::type>::value));
//    EXPECT_TRUE((std::is_same<Ret,xusd::base::__get_result_type<Ret(Args)>::type>::value));
//    EXPECT_TRUE((std::is_same<Ret,xusd::base::__get_result_type<Ret(C::*)(Args)>::type>::value));
//    //EXPECT_TRUE((std::is_same<Mem&,xusd::base::__get_result_type<Mem(C::*)>::type>::value));
//
//    const MyTest t1;
//    EXPECT_TRUE((std::is_same<int,decltype(t1.memObj)>::value));
//}
//
//#define DT(x) decltype(x)
//TEST(TestBind_t, Test1){
//    using namespace xusd;
//    EXPECT_TRUE((std::is_same<void, bind_t<void ()>::result_type>::value));
//    EXPECT_TRUE((std::is_same<void, bind_t<void (char), DT((_1))>::result_type>::value));
//    EXPECT_TRUE((std::is_same<void, bind_t<void (char,short), DT((_1)), DT((_2))>::result_type>::value));
//    EXPECT_TRUE((std::is_same<void, bind_t<void (char,short, int), DT((_1)), DT((_2)), DT((_3))>::result_type>::value));
//    EXPECT_TRUE((std::is_same<void, bind_t<void (char,short, int, long), DT((_1)), DT((_2)), DT((_3)), DT((_4))>::result_type>::value));
//    EXPECT_TRUE((std::is_same<void, bind_t<void (char,short, int, long, long long), DT((_1)), DT((_2)), DT((_3)), DT((_4)), DT((_5))>::result_type>::value));
//
//
//    EXPECT_TRUE((std::is_same<int, bind_t<int ()>::result_type>::value));
//    EXPECT_TRUE((std::is_same<int, bind_t<int (char), DT((_1))>::result_type>::value));
//    EXPECT_TRUE((std::is_same<int, bind_t<int (char,short), DT((_1)), DT((_2))>::result_type>::value));
//    EXPECT_TRUE((std::is_same<int, bind_t<int (char,short, int), DT((_1)), DT((_2)), DT((_3))>::result_type>::value));
//    EXPECT_TRUE((std::is_same<int, bind_t<int (char,short, int, long), DT((_1)), DT((_2)), DT((_3)), DT((_4))>::result_type>::value));
//    EXPECT_TRUE((std::is_same<int, bind_t<int (char,short, int, long, long long), DT((_1)), DT((_2)), DT((_3)), DT((_4)), DT((_5))>::result_type>::value));
//
//
//    EXPECT_TRUE((std::is_same<class AAA, bind_t<class AAA ()>::result_type>::value));
//    EXPECT_TRUE((std::is_same<class AAA, bind_t<class AAA (char), DT((_1))>::result_type>::value));
//    EXPECT_TRUE((std::is_same<class AAA, bind_t<class AAA (char,short), DT((_1)), DT((_2))>::result_type>::value));
//    EXPECT_TRUE((std::is_same<class AAA, bind_t<class AAA (char,short, int), DT((_1)), DT((_2)), DT((_3))>::result_type>::value));
//    EXPECT_TRUE((std::is_same<class AAA, bind_t<class AAA (char,short, int, long), DT((_1)), DT((_2)), DT((_3)), DT((_4))>::result_type>::value));
//    EXPECT_TRUE((std::is_same<class AAA, bind_t<class AAA (char,short, int, long, long long), DT((_1)), DT((_2)), DT((_3)), DT((_4)), DT((_5))>::result_type>::value));
//
//
//    EXPECT_TRUE((std::is_same<class AAA, bind_t<class AAA (class BBB), DT((_1))>::result_type>::value));
//    EXPECT_TRUE((std::is_same<class AAA, bind_t<class AAA (class BBB, class CCC), DT((_1))>::result_type>::value));
//    EXPECT_TRUE((std::is_same<class AAA, bind_t<class AAA (class BBB, class CCC, class DDD), DT((_1))>::result_type>::value));
//    EXPECT_TRUE((std::is_same<class AAA, bind_t<class AAA (class BBB, class CCC, class DDD, class EEE), DT((_1))>::result_type>::value));
//    EXPECT_TRUE((std::is_same<class AAA, bind_t<class AAA (class BBB, class CCC, class DDD, class EEE, class FFF), DT((_1))>::result_type>::value));
//    EXPECT_TRUE((std::is_same<class AAA, bind_t<class AAA (class BBB, class CCC, class DDD, class EEE, class FFF, class GGG), DT((_1))>::result_type>::value));
//
//
//    EXPECT_TRUE((std::is_same<class Ret, bind_t<class Ret (class Arg1), DT((_1))>::result_type>::value));
//    EXPECT_TRUE((std::is_same<class Ret, bind_t<class Ret (class Arg1, class Arg2), DT((_2)), DT((_1))>::result_type>::value));
//    EXPECT_TRUE((std::is_same<class Ret, bind_t<class Ret (class Arg1, class Arg2, class Arg3), DT((_2)), DT((_1))>::result_type>::value));
//    EXPECT_TRUE((std::is_same<class Ret, bind_t<class Ret (class Arg1, class Arg2, class Arg3, class Arg4),DT((_4)), DT((_1))>::result_type>::value));
//    EXPECT_TRUE((std::is_same<class Ret, bind_t<class Ret (class Arg1, class Arg2, class Arg3, class Arg4, class Arg5), DT((_5))>::result_type>::value));
//    EXPECT_TRUE((std::is_same<class Ret, bind_t<class Ret (class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6), DT((_6))>::result_type>::value));
//}
//
//TEST(TestBind_t, Test2){
//    using namespace xusd;
//    bind_t<int(int, int, int), DT(_3), DT(_2), DT(_1)> t1(add3, _3, _2, _1);
//    EXPECT_EQ((add3(1, 2, 3)), (t1(1, 2, 3)));
//    EXPECT_EQ((add3(0, 0, 0)), (t1(0, 0, 0)));
//
//
//    bind_t<std::string(std::string, std::string, std::string), DT(_1), DT(_2), DT(_3)> s1(to_string, _1, _2, _3);
//    bind_t<std::string(std::string, std::string, std::string), DT(_2), DT(_3), DT(_1)> s2(to_string, _2, _3, _1);
//    bind_t<std::string(std::string, std::string, std::string), DT(_3), DT(_1), DT(_2)> s3(to_string, _3, _1, _2);
//
//    EXPECT_EQ("123", (s1("1", "2", "3")));
//    EXPECT_EQ("321", (s1("3", "2", "1")));
//    EXPECT_EQ("23", (s1("", "2", "3")));
//    EXPECT_EQ("2_3", (s1("2", "_", "3")));
//    EXPECT_EQ("231", (s2("1", "2", "3")));
//    EXPECT_EQ("312", (s3("1", "2", "3")));
//}
//
//
//
//TEST(TestBind, NotMemberFunction){
//    using namespace xusd;
//    EXPECT_EQ((add3(1, 2, 3)), (xusd::bind(add3, _3, _2, _1)(1, 2, 3)));
//    EXPECT_EQ((add3(0, 0, 0)), (xusd::bind(add3, _3, _2, _1)(0, 0, 0)));
//
//    EXPECT_EQ("123", (xusd::bind(to_string, _1, _2, _3)("1", "2", "3")));
//    EXPECT_EQ("321", (xusd::bind(to_string, _1, _2, _3)("3", "2", "1")));
//    EXPECT_EQ("23",  (xusd::bind(to_string, _1, _2, _3)("", "2", "3")));
//    EXPECT_EQ("2_3", (xusd::bind(to_string, _1, _2, _3)("2", "_", "3")));
//    EXPECT_EQ("231", (xusd::bind(to_string, _2, _3, _1)("1", "2", "3")));
//    EXPECT_EQ("312", (xusd::bind(to_string, _3, _1, _2)("1", "2", "3")));
//
//
//    bind((voidfun))();
//    EXPECT_EQ(g_test_voidfun, 1);
//}
//
//TEST(TestBind, PassToFunctional){
//    using namespace xusd;
//    EXPECT_EQ("123", (std::function<std::string(std::string, std::string, std::string)>(xusd::bind(to_string, _1, _2, _3))("1", "2", "3")));
//    EXPECT_EQ("321", (std::function<std::string(std::string, std::string, std::string)>(xusd::bind(to_string, _1, _2, _3))("3", "2", "1")));
//    EXPECT_EQ("23",  (std::function<std::string(std::string, std::string, std::string)>(xusd::bind(to_string, _1, _2, _3))("", "2", "3")));
//    EXPECT_EQ("2_3", (std::function<std::string(std::string, std::string, std::string)>(xusd::bind(to_string, _1, _2, _3))("2", "_", "3")));
//    EXPECT_EQ("231", (std::function<std::string(std::string, std::string, std::string)>(xusd::bind(to_string, _2, _3, _1))("1", "2", "3")));
//    EXPECT_EQ("312", (std::function<std::string(std::string, std::string, std::string)>(xusd::bind(to_string, _3, _1, _2))("1", "2", "3")));
//}
//
//
//TEST(TestBind, TestMumberFunction){
//    using namespace xusd;
//    MyTest test;
//    EXPECT_EQ((add3(1, 2, 3)), (xusd::bind(&MyTest::add3, &test, _3, _2, _1)(1, 2, 3)));
//    EXPECT_EQ((add3(0, 0, 0)), (xusd::bind(&MyTest::add3, &test, _3, _2, _1)(0, 0, 0)));
//
//    EXPECT_EQ("123", (xusd::bind(&MyTest::to_string, &test, _1, _2, _3)("1", "2", "3")));
//    EXPECT_EQ("321", (xusd::bind(&MyTest::to_string, &test, _1, _2, _3)("3", "2", "1")));
//    EXPECT_EQ("23",  (xusd::bind(&MyTest::to_string, &test, _1, _2, _3)("", "2", "3")));
//    EXPECT_EQ("2_3", (xusd::bind(&MyTest::to_string, &test, _1, _2, _3)("2", "_", "3")));
//    EXPECT_EQ("231", (xusd::bind(&MyTest::to_string, &test, _2, _3, _1)("1", "2", "3")));
//    EXPECT_EQ("312", (xusd::bind(&MyTest::to_string, &test, _3, _1, _2)("1", "2", "3")));
//
//    EXPECT_EQ("312", (xusd::bind(&MyTest::to_string, _4, _3, _1, _2)("1", "2", "3", &test)));
//
//    xusd::bind(&MyTest::voidfun, &test)();
//    EXPECT_EQ(2, g_test_voidfun);
//
//    xusd::bind(&MyTest::constfun, &test)();
//    EXPECT_EQ(3, g_test_voidfun);
//}
//
//TEST(TestBind, TestFuncter){
//    using namespace xusd;
//    TestAddFuncter f1;
//    //EXPECT_EQ(3, (xusd::bind(f1, _1, _2)(2, 1)));
//}
//
//TEST(TestBind, TestCFunction){
//    using namespace xusd;
//    EXPECT_EQ(1, (xusd::bind(abs, _1)(-1)));
//}
//
//TEST(TestBind, TestMemberObj){
//    MyTest t1;
//    EXPECT_EQ(t1.memObj, 0);
//    //xusd::bind(&MyTest::memObj, &t1)() = 1;
//    //EXPECT_EQ(t1.memObj, 1);
//}

int main(int argc, char* argv[]){

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

