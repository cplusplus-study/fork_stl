#include <iostream>
#include <algorithm>
#include <vector>
#include <gtest/gtest.h>

#include "iterator.hpp"
#include "functional.hpp"
#include "algorithm.hpp"
#include <__function.hpp>

using namespace xusd;

struct A{
    int getvalue(){
        return 1;
    }
    int const_getvalue()const{
        return 2;
    }
};
struct CheckResult{
    void operator()(int x){
        EXPECT_EQ(1,x);
    }
};
template<typename T>
struct FreeMem{
    void operator()(T* p){
        delete p;
    }
};


TEST(Functional,Mem_fun){

    A a;
    A const a1 = a;
    EXPECT_EQ(1,mem_fun(&A::getvalue)(&a));
    EXPECT_EQ(1,mem_fun_ref(&A::getvalue)(a));

    EXPECT_EQ(2,mem_fun(&A::const_getvalue)(&a1));
    EXPECT_EQ(2,mem_fun_ref(&A::const_getvalue)(a1));

    std::vector<A> v(10,a);
    ASSERT_EQ(v.size(),(std::vector<A>::size_type)10);
    std::vector<int> ret;
    xusd::transform(v.begin(),v.end(),xusd::back_inserter(ret),mem_fun_ref(&A::getvalue));
    ASSERT_EQ(ret.size(),(std::vector<int>::size_type)10);
    xusd::for_each(ret.begin(),ret.end(),CheckResult());//若是我把CheckResult放在本函数里面,则这个会报编译不过





    std::vector<A*> v2;
    for(int i = 0; i< 20; ++i){
        v2.push_back(new A());//这里一定要注意释放
    }

    ASSERT_EQ(v2.size(),(std::vector<A*>::size_type)20);
    ret.clear();
    ASSERT_EQ(ret.size(),(std::vector<int>::size_type)0);
    xusd::transform(v2.begin(),v2.end(),xusd::back_inserter(ret),mem_fun(&A::getvalue));
    xusd::for_each(v2.begin(),v2.end(),FreeMem<A>());
    ASSERT_EQ(v2.size(),(std::vector<A*>::size_type)20);
    v2.clear();
    ASSERT_EQ(v2.size(),(std::vector<A*>::size_type)0);

    ASSERT_EQ(ret.size(),(std::vector<int>::size_type)20);
    xusd::for_each(ret.begin(),ret.end(),CheckResult());


}


TEST(Functional,unary_negate){

}

TEST(Functional,binary_negate){

    EXPECT_TRUE(not2(less<int>())(2,1));
}

TEST(Functional,compose1){
    int a = 10;
    EXPECT_EQ(a,compose1(identity<int>(),identity<int>())(a));
    EXPECT_EQ(-a,compose1(negate<int>(),identity<int>())(a));
    EXPECT_EQ(a,compose1(negate<int>(),negate<int>())(a));
}

TEST(Functional,compose2){
    EXPECT_EQ(0,compose2(plus<int>(),negate<int>(),identity<int>())(10));
    EXPECT_EQ(-20,compose2(minus<int>(),negate<int>(),identity<int>())(10));
}


int freefun1(){
    return 755;
}
int test_free_fun2 = 0;
void freefun2(){
    test_free_fun2 = 1;
}

int test_free_add(int x, int y, int z){

    return x + y + z;
}
TEST(TestFunction, testFreeFunction){
    xusd::function<int()> f1 = freefun1;
    EXPECT_EQ(755, f1());

    xusd::function<void()> f2 = freefun2;

    f2();
    EXPECT_EQ(1, test_free_fun2);

    EXPECT_EQ(6, (xusd::function<int(int,int, int)>(test_free_add)(1, 2, 3)));
}

class testObjectFunctor{
public:
    int add(int x, int y){
        return x + y;
    }
};
class testFunctor{
public:
    int operator()(int x, int y){
        return x + y;
    }
};
TEST(TestFunction, testMemberFunction){
    xusd::function<int(testObjectFunctor*, int, int)> of = &testObjectFunctor::add;
    testObjectFunctor a;
    EXPECT_EQ(3,(of(&a, 1, 2)));
    EXPECT_EQ(5,(xusd::function<int(testObjectFunctor&, int, int)>(&testObjectFunctor::add)(a,1,4)));
    EXPECT_EQ(3, (xusd::function<int(int,int)>(testFunctor())(1, 2)));
}


int main(int argc, char* argv[]){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
