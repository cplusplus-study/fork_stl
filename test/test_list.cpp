#include "list.hpp"
#include "algorithm.hpp"
#include <algorithm>
#include<gtest/gtest.h>

using namespace xusd;

struct A{};
struct B{};

TEST(List,begin){

    int Arr[] = { 1, 2, 3, 4};
    list<int> a(Arr,Arr+4);
    ASSERT_EQ(a.size(),(list<int>::size_type)4);
    list<int>::iterator i = a.begin();
    ASSERT_EQ(*i,a.front());
    ++i;
    ASSERT_EQ(*i,2);
    list<int>::iterator i2 = a.erase(a.begin());
    ASSERT_TRUE((i == i2));
    a.clear();
    ASSERT_TRUE(a.empty());


    list<int> b;
    ASSERT_EQ((b.begin()),(b.end()));
    EXPECT_TRUE(b.empty());
    copy(Arr+1,Arr+4,back_inserter(b));
    equal(b.begin(),b.end(),a.begin());
    EXPECT_EQ(b.size(),list<int>::size_type(3));

}
TEST(List,end){
    
}

TEST(List,rbegin){

}
TEST(List,rend){

}

TEST(List,size){
    list<int> a;
    EXPECT_EQ(a.size(),(list<int>::size_type)0);

    list<A> b(10,A());
    EXPECT_EQ(b.size(),(list<int>::size_type)10);


    list<int> c((std::size_t)10,10);
    EXPECT_EQ(c.size(),(list<int>::size_type)10);
}

TEST(List,max_size){

}

TEST(List,empty){

}

TEST(List,constructor){

}

TEST(List,destructor){

}

TEST(List,swap){
    int Arr[] = { 1,2,3,4,5,6};
    list<int> a(Arr,Arr+6);

    list<int> b;
    ASSERT_EQ(a.size(),list<int>::size_type(6));
    ASSERT_TRUE(b.empty());
    ASSERT_TRUE(equal(a.begin(),a.end(),Arr));

    swap(a,b);

    ASSERT_NE(a.size(),list<int>::size_type(6));
    ASSERT_FALSE(b.empty());
    ASSERT_TRUE(a.empty());
    ASSERT_TRUE(equal(b.begin(),b.end(),Arr));
}

TEST(List,get_allocator){

}

TEST(List,Operator_assign){

}

TEST(List,front){

}
TEST(List,back){

}

TEST(List,push_front){

}
TEST(List,push_back){

}

TEST(List,pop_front){

}
TEST(List,pop_back){

}

TEST(List,clear){

}

TEST(List,insert){

}

TEST(List,Erase){
    int A[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    list<int> a(A,A+10);

    ASSERT_EQ(a.size(),list<int>::size_type(10));
    EXPECT_TRUE((xusd::equal(A,A+10,a.begin())));

}

TEST(List,resize){

}
TEST(List,assign){

}
TEST(List,splice){

    list<int> a;
    list<int> b;
    int Arr[] = { 11,12,13,14,15,16,17 };
    list<int> c(Arr,Arr+7);
    for(int i =1;i<11;i=i+2){
        a.push_back(i);
        b.push_back(i+1);
    }

    ASSERT_EQ(a.size(),list<int>::size_type(5));
    ASSERT_EQ(b.size(),list<int>::size_type(5));
    ASSERT_EQ(c.size(),list<int>::size_type(7));

    a.splice(a.begin(),b);
    c.splice(a.begin(),b);

    ASSERT_EQ(a.size(),list<int>::size_type(10));
    ASSERT_EQ(b.size(),list<int>::size_type(0));
    ASSERT_EQ(c.size(),list<int>::size_type(7));

    list<int> d;
    d.splice(d.begin(),c,c.begin());
    ASSERT_EQ(d.size(),list<int>::size_type(1));
    ASSERT_EQ(c.size(),list<int>::size_type(6));


    list<int> e;
    e.push_back(3);
    list<int> f;
    f.push_back(9);
    e.splice(e.begin(),f,f.begin(),f.end());

    ASSERT_EQ(e.size(),list<int>::size_type(2));
    ASSERT_EQ(f.size(),list<int>::size_type(0));


}

TEST(List,remove){

}

TEST(List,remove_if){

}
TEST(List,unique){

}
TEST(List,merge){
}
TEST(List,reverse){
    int Arr[] = { 1, 2, 3, 4};
    list<int> a;
    a.reverse();
    ASSERT_TRUE(a.empty());
    copy(Arr,Arr+4,front_inserter(a));// 4, 3, 2, 1
    a.reverse();// 1, 2, 3, 4
    ASSERT_TRUE(equal(Arr,Arr+4,a.begin()));
    a.reverse();// 4,3,2,1
    ASSERT_FALSE(equal(a.begin(),a.end(),Arr));
}
TEST(List,sort){
    int Arr[] = { 7,6,3,9,8,2,10};
    list<int> a(Arr,Arr+7);
    a.sort();
    ASSERT_FALSE(equal(a.begin(),a.end(),Arr));

    std::sort(Arr,Arr+7);
    ASSERT_TRUE(equal(a.begin(),a.end(),Arr));

    a.sort(std::greater<int>());
    ASSERT_FALSE(equal(a.begin(),a.end(),Arr));

    std::sort(Arr,Arr+7,std::greater<int>());
    ASSERT_TRUE(equal(a.begin(),a.end(),Arr));


}
TEST(List,Operator_equal){
}
TEST(List,Operator_less){
}
TEST(List,Operator_greator){
}
TEST(List,Operator_greatorequal){
}
TEST(List,Operator_lessequal){
}

int main(int argc,char* argv[]){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

