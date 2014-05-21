///////////////////////////////////////////////////////////////////////////////
// 以下开始为测试代码.

#include <cstdlib>
#include <string>
#include <iostream>
#include <functional> //ref, cref
#include <bind.hpp>
#include <gtest/gtest.h>

int add3(int x, int y, int z){
    return x + y + z;
}

std::string to_string(std::string s1, std::string s2, std::string s3){
    return s1 + s2 + s3;
}

int g_test_voidfun =0;
void voidfun(){
    g_test_voidfun = 1;
}

class MyTest{
public:
    MyTest(){ }
    std::string to_string(std::string s1, std::string s2, std::string s3){
        return s1 + s2 + s3;
    }
    int add3(int x, int y, int z){
        return x + y + z;
    }

    int cadd3(int x, int y, int z) const {
        return -(x + y + z);
    }
    void voidfun(){
        g_test_voidfun = 2;
    }
    void constfun() const {
        g_test_voidfun = 3;
    }

    int memObj = 0;
    std::string memObj2;
};

class TestAddFuncter{
public:
    int operator()(int x, int y){
        return x + y;
    }
};


TEST(TestSeq,Test1){
    using namespace xusd;
    EXPECT_TRUE((std::is_same<gen<0>::type, seq<> >::value));
    EXPECT_TRUE((std::is_same<gen<1>::type, seq<0> >::value));
    EXPECT_TRUE((std::is_same<gen<2>::type, seq<0,1> >::value));
    EXPECT_TRUE((std::is_same<gen<3>::type, seq<0,1,2> >::value));
    EXPECT_TRUE((std::is_same<gen<4>::type, seq<0,1,2,3> >::value));
    EXPECT_TRUE((std::is_same<gen<5>::type, seq<0,1,2,3,4> >::value));
    EXPECT_TRUE((std::is_same<gen<6>::type, seq<0,1,2,3,4,5> >::value));
    EXPECT_TRUE((std::is_same<gen<7>::type, seq<0,1,2,3,4,5,6> >::value));
    EXPECT_TRUE((std::is_same<gen<8>::type, seq<0,1,2,3,4,5,6,7> >::value));
    EXPECT_TRUE((std::is_same<gen<9>::type, seq<0,1,2,3,4,5,6,7,8> >::value));
    EXPECT_TRUE((std::is_same<gen<10>::type, seq<0,1,2,3,4,5,6,7,8,9> >::value));
}

TEST(TestPlaceHolder, Test1){
    using namespace xusd;
    EXPECT_TRUE((std::is_same<decltype(_1), placeholder<1> >::value));
    EXPECT_TRUE((std::is_same<decltype(_2), placeholder<2> >::value));
    EXPECT_TRUE((std::is_same<decltype(_3), placeholder<3> >::value));
    EXPECT_TRUE((std::is_same<decltype(_4), placeholder<4> >::value));
    EXPECT_TRUE((std::is_same<decltype(_5), placeholder<5> >::value));
    EXPECT_TRUE((std::is_same<decltype(_6), placeholder<6> >::value));
    EXPECT_TRUE((std::is_same<decltype(_7), placeholder<7> >::value));
    EXPECT_TRUE((std::is_same<decltype(_8), placeholder<8> >::value));
    EXPECT_TRUE((std::is_same<decltype(_9), placeholder<9> >::value));

    EXPECT_EQ(0, (is_placeholder<int>::value));
    EXPECT_EQ(0, (is_placeholder<class A>::value));
    EXPECT_EQ(1, (is_placeholder<decltype(_1)>::value));
    EXPECT_EQ(2, (is_placeholder<decltype(_2)>::value));
    EXPECT_EQ(3, (is_placeholder<decltype(_3)>::value));
    EXPECT_EQ(4, (is_placeholder<decltype(_4)>::value));
    EXPECT_EQ(5, (is_placeholder<decltype(_5)>::value));
    EXPECT_EQ(6, (is_placeholder<decltype(_6)>::value));
    EXPECT_EQ(7, (is_placeholder<decltype(_7)>::value));
    EXPECT_EQ(8, (is_placeholder<decltype(_8)>::value));
    EXPECT_EQ(9, (is_placeholder<decltype(_9)>::value));
}

TEST(TestSelectArgs, Test1){
    using namespace xusd;
    //auto b = std::make_tuple(1,_1,2,_2,3,_3,4,_4);
    auto c = std::make_tuple(11,22,33,44);

    EXPECT_EQ(1, (select(1, c)));
    EXPECT_STREQ("22", (select("22", c)));
    EXPECT_EQ(11, (select(_1, c)));
    EXPECT_EQ(22, (select(_2, c)));
    EXPECT_EQ(33, (select(_3, c)));
    EXPECT_EQ(44, (select(_4, c)));
}


#define DT(x) decltype(x)
TEST(TestBind_t, TestResultType){
    using namespace xusd;
    EXPECT_TRUE((std::is_same<void, decltype(std::declval<bind_t<void ()>>()())>::value));
    EXPECT_TRUE((std::is_same<void, decltype(std::declval<bind_t<void (char), DT((_1))>>()('a'))>::value));
    EXPECT_TRUE((std::is_same<void, decltype(std::declval<bind_t<void (char,short), DT((_1)), DT((_2))>>()('1', 2))>::value));
    EXPECT_TRUE((std::is_same<void, decltype(std::declval<bind_t<void (char,short, int), DT((_1)), DT((_2)), DT((_3))>>()('1', (short)2, 3))>::value));
    EXPECT_TRUE((std::is_same<void, decltype(std::declval<bind_t<void (char,short, int, long), DT((_1)), DT((_2)), DT((_3)), DT((_4))>>()('a',short(2), int(3), long(4)))>::value));
    EXPECT_TRUE((std::is_same<void, decltype(std::declval<bind_t<void (char,short, int, long, long long), DT((_1)), DT((_2)), DT((_3)), DT((_4)), DT((_5))>>()(char(1),short(2), int(3), long(4), (long long)(5)))>::value));
}

TEST(TestBind_t, Test2){
    using namespace xusd;
    bind_t<int(int, int, int), DT(_3), DT(_2), DT(_1)> t1(add3, _3, _2, _1);
    EXPECT_EQ((add3(1, 2, 3)), (t1(1, 2, 3)));
    EXPECT_EQ((add3(0, 0, 0)), (t1(0, 0, 0)));


    bind_t<std::string(std::string, std::string, std::string), DT(_1), DT(_2), DT(_3)> s1(to_string, _1, _2, _3);
    bind_t<std::string(std::string, std::string, std::string), DT(_2), DT(_3), DT(_1)> s2(to_string, _2, _3, _1);
    bind_t<std::string(std::string, std::string, std::string), DT(_3), DT(_1), DT(_2)> s3(to_string, _3, _1, _2);

    EXPECT_EQ("123", (s1("1", "2", "3")));
    EXPECT_EQ("321", (s1("3", "2", "1")));
    EXPECT_EQ("23", (s1("", "2", "3")));
    EXPECT_EQ("2_3", (s1("2", "_", "3")));
    EXPECT_EQ("231", (s2("1", "2", "3")));
    EXPECT_EQ("312", (s3("1", "2", "3")));
}

TEST(TestBind, NotMemberFunction){
    using namespace xusd;
    EXPECT_EQ((add3(1, 2, 3)), (xusd::bind(add3, _3, _2, _1)(1, 2, 3)));
    EXPECT_EQ((add3(0, 0, 0)), (xusd::bind(add3, _3, _2, _1)(0, 0, 0)));

    EXPECT_EQ("123", (xusd::bind(to_string, _1, _2, _3)("1", "2", "3")));
    EXPECT_EQ("321", (xusd::bind(to_string, _1, _2, _3)("3", "2", "1")));
    EXPECT_EQ("23",  (xusd::bind(to_string, _1, _2, _3)("", "2", "3")));
    EXPECT_EQ("2_3", (xusd::bind(to_string, _1, _2, _3)("2", "_", "3")));
    EXPECT_EQ("231", (xusd::bind(to_string, _2, _3, _1)("1", "2", "3")));
    EXPECT_EQ("312", (xusd::bind(to_string, _3, _1, _2)("1", "2", "3")));

    bind((voidfun))();
    EXPECT_EQ(g_test_voidfun, 1);
}

TEST(TestBind, PassToFunctional){
    using namespace xusd;
    EXPECT_EQ("123", (std::function<std::string(std::string, std::string, std::string)>(xusd::bind(to_string, _1, _2, _3))("1", "2", "3")));
    EXPECT_EQ("321", (std::function<std::string(std::string, std::string, std::string)>(xusd::bind(to_string, _1, _2, _3))("3", "2", "1")));
    EXPECT_EQ("23",  (std::function<std::string(std::string, std::string, std::string)>(xusd::bind(to_string, _1, _2, _3))("", "2", "3")));
    EXPECT_EQ("2_3", (std::function<std::string(std::string, std::string, std::string)>(xusd::bind(to_string, _1, _2, _3))("2", "_", "3")));
    EXPECT_EQ("231", (std::function<std::string(std::string, std::string, std::string)>(xusd::bind(to_string, _2, _3, _1))("1", "2", "3")));
    EXPECT_EQ("312", (std::function<std::string(std::string, std::string, std::string)>(xusd::bind(to_string, _3, _1, _2))("1", "2", "3")));
}


TEST(TestBind, TestMumberFunction){
    using namespace xusd;
    MyTest test;
    EXPECT_EQ((add3(1, 2, 3)), (xusd::bind(&MyTest::add3, &test, _3, _2, _1)(1, 2, 3)));
    EXPECT_EQ((add3(0, 0, 0)), (xusd::bind(&MyTest::add3, &test, _3, _2, _1)(0, 0, 0)));

    EXPECT_EQ((-add3(1, 2, 3)), (xusd::bind(&MyTest::cadd3, (const MyTest*)&test, _3, _2, _1)(1, 2, 3)));
    EXPECT_EQ((-add3(0, 0, 0)), (xusd::bind(&MyTest::cadd3, (const MyTest*)&test, _3, _2, _1)(0, 0, 0)));

    EXPECT_EQ("123", (xusd::bind(&MyTest::to_string, &test, _1, _2, _3)("1", "2", "3")));
    EXPECT_EQ("321", (xusd::bind(&MyTest::to_string, &test, _1, _2, _3)("3", "2", "1")));
    EXPECT_EQ("23",  (xusd::bind(&MyTest::to_string, &test, _1, _2, _3)("", "2", "3")));
    EXPECT_EQ("2_3", (xusd::bind(&MyTest::to_string, &test, _1, _2, _3)("2", "_", "3")));
    EXPECT_EQ("231", (xusd::bind(&MyTest::to_string, &test, _2, _3, _1)("1", "2", "3")));
    EXPECT_EQ("312", (xusd::bind(&MyTest::to_string, &test, _3, _1, _2)("1", "2", "3")));

    EXPECT_EQ("312", (xusd::bind(&MyTest::to_string, _4, _3, _1, _2)("1", "2", "3", &test)));

    xusd::bind(&MyTest::voidfun, &test)();
    EXPECT_EQ(2, g_test_voidfun);

    xusd::bind(&MyTest::constfun, &test)();
    EXPECT_EQ(3, g_test_voidfun);
}

TEST(TestBind, TestFuncter){
    using namespace xusd;
    TestAddFuncter f1;
    EXPECT_EQ(3, (xusd::bind(f1, _1, _2)(2, 1)));
}

TEST(TestBind, TestCFunction){
    using namespace xusd;
    EXPECT_EQ(1, (xusd::bind(abs, _1)(-1)));
}

TEST(TestBind, TestMemberObj){
    MyTest t1;
    EXPECT_EQ(t1.memObj, 0);
    xusd::bind(&MyTest::memObj, &t1)() = 1;
    EXPECT_EQ(t1.memObj, 1);
}

#define BOOST_CHECK(...) EXPECT_TRUE((__VA_ARGS__))

static int g_Aconstruct = 0;
static int g_Acopy = 0;
static int g_Amove = 0;
static int g_MoveAmove = 0;
static int g_MoveAcopy = 0;

void reset_counter()
{
    g_Aconstruct = g_Acopy = g_Amove = g_MoveAmove = g_MoveAcopy = 0;
}

struct A
{
    explicit A(int i) : m_i(i) { ++g_Aconstruct; /*Dump("A::construct " << m_i);*/ };
    A(const A& other) : m_i(other.m_i) { ++g_Acopy; /*Dump("A::copy " << m_i);*/ }
    A(A&& other) : m_i(other.m_i) { other.m_i = 0; ++g_Amove; /*Dump("A::move " << m_i);*/ }
    ~A() = default;

    mutable int m_i{ 0 };
    int f() const
    {
        //DumpX(m_i);
        return (m_i *= 10);
    }

    int mem_func(int a, int b)
    {
        return a + b + m_i;
    }

    int mem_func_const(int a, int b) const
    {
        return a + b + m_i;
    }

    int mem_func_volatile(int a, int b) volatile
    {
        return a + b + m_i;
    }
};


int func(A construct, A copy, A & lref, A const& clref, A && rref)
{
    construct.f();
    copy.f();
    lref.f();
    clref.f();
    rref.f();
    A temp(std::move(rref));
    return 0;
}

struct object_func
{
    inline void operator()(int& a, int b) const
    {
        a += b;
    }
};

struct MoveA
{
    MoveA() = default;
    MoveA(MoveA&&) { ++g_MoveAmove; /*Dump("MoveA::move");*/ }
    ~MoveA() = default;

    /// TODO: Ö§³Övs2013 ctp
    MoveA(MoveA const&)
    {
        ++g_MoveAcopy;
    }
    //MoveA& operator=(MoveA const&) = default;
    //MoveA& operator=(MoveA&&) = default;
};

int move_func(MoveA const& a)
{
    return 0;
}

void void_move_func(MoveA const& a)
{
}

void test()
{
    namespace Bex = xusd;
    {
        /// ×ÔÓÉº¯Êý×ª·¢²âÊÔ (ÐÎ²Î: Öµ´«µÝ, ×óÖµÒýÓÃ, ³£Á¿×óÖµÒýÓÃ, ÓÒÖµÒýÓÃ)
        reset_counter();
        A a(1), b(2), c(3), d(4);
        auto f = Bex::bind(&func, Bex::_1, Bex::_2, Bex::_3, /*Bex::_5*/std::ref(c), Bex::_4);
        f(A(9), a, b, /*c,*/ std::move(d));
        BOOST_CHECK(a.m_i == 1);
        BOOST_CHECK(b.m_i == 20);
        BOOST_CHECK(c.m_i == 30);
        BOOST_CHECK(d.m_i == 0);

        BOOST_CHECK(g_Aconstruct == 5);
        BOOST_CHECK(g_Acopy == 1);
        BOOST_CHECK(g_Amove == 2);
    }

    //////////////////////////////////////////////////////////////////////////
    /// @{ ·Ç¾²Ì¬³ÉÔ±º¯Êý (cv)
    {
        reset_counter();
        A a(1);
        auto f = Bex::bind(&A::mem_func, Bex::_1, 1, Bex::_2);
        BOOST_CHECK(f(a, 2) == 4);
        BOOST_CHECK(g_Aconstruct == 1);
        BOOST_CHECK(g_Acopy == 0);
        BOOST_CHECK(g_Amove == 0);
    }
    {
        reset_counter();
        const A a(1);
        auto f = Bex::bind(&A::mem_func_const, Bex::_1, 1, Bex::_2);
        BOOST_CHECK(f(a, 2) == 4);
        BOOST_CHECK(g_Aconstruct == 1);
        BOOST_CHECK(g_Acopy == 0);
        BOOST_CHECK(g_Amove == 0);
    }
    {
        reset_counter();
        volatile A a(1);
        auto f = Bex::bind(&A::mem_func_volatile, Bex::_1, 1, Bex::_2);
        BOOST_CHECK(f(a, 2) == 4);
        BOOST_CHECK(g_Aconstruct == 1);
        BOOST_CHECK(g_Acopy == 0);
        BOOST_CHECK(g_Amove == 0);
    }
    {
        reset_counter();
        A a(2);
        auto f = Bex::bind(&A::f, std::ref(a));
        f();
    }
    ///// @}
    ////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    /// @{ ·Âº¯Êý
    {
        object_func obj;
        auto f = Bex::bind(obj, Bex::_1, 3);
        int r = 1;
        f(r);
        BOOST_CHECK(r == 4);
    }
    // @}
    ////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    /// @{ °ó¶¨·Ç¾²Ì¬³ÉÔ±±äÁ¿
    {
        reset_counter();
        A a(2);
        auto f = Bex::bind(&A::m_i, Bex::_1);
        int r = f(a);
        BOOST_CHECK(r == 2);
        BOOST_CHECK(g_Aconstruct == 1);
        BOOST_CHECK(g_Acopy == 0);
        BOOST_CHECK(g_Amove == 0);
    }
    /// @}
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    /// @{ ÒÆ¶¯ÓïÒå°ó¶¨²ÎÊý
    {
        reset_counter();
        MoveA a;
        auto f = Bex::bind(&move_func, std::move(a));
        f();
        BOOST_CHECK(g_MoveAmove == 1);
    }
    {
        reset_counter();
        MoveA a;
        auto f = Bex::bind(&void_move_func, std::move(a));
        f();
        BOOST_CHECK(g_MoveAmove == 1);
    }
    /// @}
    //////////////////////////////////////////////////////////////////////////
}
//


int freeFun(int x , int y){
    return x*y;
}

TEST(TestBind2, TestFreeFunc){
    using namespace xusd;
    auto freeFun1 = xusd::bind(&freeFun, _1, _2);
    auto freeFun2 = xusd::bind(&freeFun, 3, _1);
    auto freeFun3 = xusd::bind(&freeFun, 3, 2);
    EXPECT_EQ(6, (freeFun1(2,3)));
    EXPECT_EQ(6, (freeFun2(2)));
    EXPECT_EQ(6, (freeFun3()));
}

int overLoadFreeFun(int x, int y){
    return x + y;
}
double overLoadFreeFun(double x, double y){
    return x * y;
}

TEST(TestBind2, TestFreeFunc_OverLoad){
    using namespace xusd;
    auto f1 = xusd::bind((int(*)(int, int))&overLoadFreeFun, _1, _2);
    auto f2 = xusd::bind((double(*)(double, double))&overLoadFreeFun, _1, _2);
    EXPECT_EQ(5, (f1(2,3)));
    EXPECT_EQ(6, (f2(2,3)));
}

template<typename T1, typename T2>
auto TmplFun(T1 t1, T2 t2) -> decltype(t1+t2){
    return t1 + t2;
}
TEST(TestBind2, TestFreeFunc_Template){
    using namespace xusd;
    auto f1 = bind((int(*)(int, int))&TmplFun, _1, _2);
    auto f2 = bind((double(*)(int, double))&TmplFun, _1, _2);
    EXPECT_TRUE((std::is_same<int, decltype(f1(1,2))>::value));
    EXPECT_TRUE((std::is_same<double, decltype(f2(1,2))>::value));
    EXPECT_EQ(3, (f1(1, 2)));
    EXPECT_EQ(3, (f2(1, 2.0)));
}

using namespace xusd;
class AAA{
public:
    int mfun1(int x, int y){
        return x*y;
    }

    AAA():_i(0){ }
    AAA(AAA&&o):_i(o._i){ }
    explicit AAA(int i):_i(i){ }
    void set(int i){ _i = i; }
    int get(){ return _i; }
    AAA* thisPointer(){
        return this;
    }

private:
    int _i = 0;
};
TEST(TestBind2, TestMemberFunc){
    AAA a1;
    auto f1 = xusd::bind(&AAA::mfun1, std::ref(a1), _2, _1);
    auto f2 = xusd::bind(&AAA::mfun1, &a1, _2, _1);
    auto f3 = xusd::bind(&AAA::mfun1, _3, _2, _1);
    auto f4 = xusd::bind(&AAA::mfun1, _1, _2, _3);

    auto get = xusd::bind(&AAA::get, _1);
    auto pointer = xusd::bind(&AAA::thisPointer, _1);

    EXPECT_EQ(14, (f1(7, 2)));
    EXPECT_EQ(10, (f2(5, 2)));
    EXPECT_EQ(10, (f3(5, 2, a1)));
    EXPECT_EQ(10, (f3(5, 2, &a1)));
    EXPECT_EQ(10, (f4(&a1, 2, 5)));
    EXPECT_EQ(10, (f4(a1, 2, 5)));


    EXPECT_EQ(0, (get(&a1)));
    a1.set(1);
    EXPECT_EQ(1, (get(&a1)));

    EXPECT_EQ(&a1, pointer(&a1));
    EXPECT_EQ(&a1, pointer(a1));
    auto p1 = &a1;
    auto p2 = pointer(std::move(a1));
    EXPECT_EQ(p1, p2); // ? 
}

class BBB{
public:
    int fun1(int x, int y){ return x + y; }
    std::string fun1(std::string a, std::string b){ return a+b; }
};
TEST(TestBind2, TestMemberFunc_Overload){
    BBB b;
    auto f1 = xusd::bind((int(BBB::*)(int , int))&BBB::fun1, std::ref(b), _2, _1);
    auto f2 = xusd::bind((std::string(BBB::*)(std::string , std::string))&BBB::fun1,&b, _2, _1);

    ASSERT_EQ(3, f1(1, 2));
    ASSERT_EQ(std::string("21"), f2("1", "2"));
}
class CCC{
public:
    template<typename T1, typename T2>
    auto fun1(T1 x, T2 y) -> decltype(x+y){ return  x+y; }
};
TEST(TestBind2, TestMemberFunc_Template){
    CCC c;
    auto f1 = xusd::bind((int(CCC::*)(int , int))&CCC::fun1, std::ref(c), _2, _1);
    auto f2 = xusd::bind((std::string(CCC::*)(std::string , std::string))&CCC::fun1,&c, _2, _1);

    ASSERT_EQ(3, f1(1, 2));
    ASSERT_EQ(std::string("21"), f2("1", "2"));
}
class DDD{
public:
    DDD():_i(0){ }
    int fun1(int x, int y) { _i = x+y; return  x+y; }
    int fun1(int x, int y) const { return x*y; }
private:
    int _i;
};

TEST(TestBind2, TestMemberFunc_Const){
    DDD d1;
    const DDD d2;
    auto f1 = xusd::bind((int(DDD::*)(int , int))&DDD::fun1, _1, _2, _3);
    auto f2 = xusd::bind((int(DDD::*)(int , int) const)&DDD::fun1, _1, _2, _3);

    ASSERT_EQ(9, f1(d1, 2, 7));
    ASSERT_EQ(14, f2(d1, 2, 7));

    ASSERT_EQ(21, f2(d2, 3, 7));

}

TEST(TestBind2, TestLambda){
    auto l1 = [](int x,  int y){ return x*y; };
    auto f1 = xusd::bind(l1, _2, _1);
    ASSERT_EQ(2, f1(1, 2));
    ASSERT_EQ(std::string("yyxx"), (xusd::bind([](std::string x, std::string y) -> std::string { return x + y; }, _2, _1)(std::string("xx"), std::string("yy"))));
}
TEST(TestBind2, TestFunctionObject){
    auto l1 = [](int x,  int y){ return x*y; };
    auto f1 = xusd::bind(xusd::bind(l1, _2, _1), _1, 5);
    ASSERT_EQ(10, f1(2));
}

int main(int argc, char* argv[]){

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

