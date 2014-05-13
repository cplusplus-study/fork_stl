#include <vector>
#include <deque>
#include <iostream>
#include <algorithm.hpp>
#include <functional.hpp>
#include <gtest/gtest.h>

#define PRINT_SEQ(a) { \
    xusd::copy(a.begin(),a.end(),ostream_iterator<int>(std::cout, " ")); \
    std::cout<<std::endl;\
    }

using namespace xusd;

struct test_op{
    void operator()(int &a){ 
        a += 1;
        num = num +1;
    }
    int get_num(){
        return num;
    }
private:
    int num ;
};
TEST(Algorithm,FOR_EACH){
    int A[] = { 1,2,3,4,5,6,7 };
    std::vector<int> test(A,A+7);
    std::vector<int> tmp;


    //test原先的值
    std::cout<<"test = [";
    xusd::copy(test.begin(),test.end(),ostream_iterator<int>(std::cout," "));
    std::cout<<"]"<<std::endl;

    xusd::transform(A+0,A+7,xusd::back_inserter(tmp),bind2nd(plus<int>(),1));
    //test原先的值
    std::cout<<"tmp = [";
    xusd::copy(tmp.begin(),tmp.end(),ostream_iterator<int>(std::cout," "));
    std::cout<<"]"<<std::endl;

    ASSERT_EQ(for_each(test.begin(),test.end(),test_op()).get_num(),(int)test.size());

    //test改变后的值
    std::cout<<"test = [";
    xusd::copy(test.begin(),test.end(),ostream_iterator<int>(std::cout," "));
    std::cout<<"]"<<std::endl;

    EXPECT_TRUE(xusd::equal(test.begin(),test.end(),tmp.begin()));
}

TEST(Algorithm,Accumulate){
    int A[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9,10 };
    ASSERT_EQ(accumulate(A+0,A+10,0),55);
    ASSERT_EQ(accumulate(A+0,A+10,1),56);

    ASSERT_EQ(accumulate(A,A+10,0,xusd::plus<int>()),55);
    ASSERT_EQ(accumulate(A,A+10,0,xusd::minus<int>()),-55);
}

TEST(Algorithm,Inner_Product){
    int A1[] = { 1, 2, 3, 4, 5 };
    int A2[] = { 7, 9, 11, 13, 15 };


    ASSERT_EQ(inner_product(A1,A1+5,A2,0),0+1*7+2*9+3*11+4*13+5*15);
    ASSERT_EQ(inner_product(A1,A1+5,A2,0,plus<int>(),plus<int>()),0+1+7+2+9+3+11+4+13+5+15);
    ASSERT_EQ(inner_product(A1,A1+5,A2,0,plus<int>(),minus<int>()),0+1-7+2-9+3-11+4-13+5-15);
    ASSERT_EQ(inner_product(A1,A1+5,A2,0,minus<int>(),multiplies<int>()),0-1*7-2*9-3*11-4*13-5*15);
}

struct test_unique{
    template<typename T>
    bool operator()(T &a,T &b){
        std::cout<<"a= "<<a<<" b= "<<b<<std::endl;
        return (a-b) == -1;
    }
};
TEST(Algorithm,Unique){
    int A[] = { 1,2,2,2,3,3,4,5,7,7,8,9,10,10};
    int B[] = { 1,2,3,4,5,7,8,9,10};
    int C[] = {1,2,3,4,5,6,7,8,9,10};
    std::vector<int> a(A,A+14);
    //PRINT_SEQ(a);
    ASSERT_EQ(a.size(),std::vector<int>::size_type(14));

    std::vector<int>::iterator end = unique(a.begin(),a.end());
    ASSERT_EQ(a.size(),std::vector<int>::size_type(14));
    ASSERT_TRUE(xusd::equal(B,B+9,a.begin()));

    //PRINT_SEQ(a);
    a.erase(end,a.end());
    ASSERT_EQ(a.size(),std::vector<int>::size_type(9));
    ASSERT_TRUE(xusd::equal(B,B+9,a.begin()));
    //PRINT_SEQ(a);
    std::vector<int> c(C,C+10);
    ASSERT_EQ(c.size(),std::vector<int>::size_type(10));
    c.erase(unique(c.begin(),c.end(),test_unique()),c.end());
    ASSERT_EQ(c.size(),std::vector<int>::size_type(5));

    std::vector<int> d;
    xusd::unique_copy(C,C+10,xusd::back_inserter(d),test_unique());
    PRINT_SEQ(d);
    ASSERT_TRUE(xusd::equal(c.begin(),c.end(),d.begin()));

    //PRINT_SEQ(c);

}

TEST(Algorithm,Reverse){

    int A[] = {1,2,3,4,5,6,7,8,9,10};
    std::deque<int> a;
    a.assign(A,A+10);

    xusd::reverse(a.begin(),a.end());

    ASSERT_TRUE(xusd::equal(A,A+10,a.rbegin()));

    PRINT_SEQ(a);

    reverse_copy(A,A+10,ostream_iterator<int>(std::cout," "));
    std::cout<<std::endl;


}


TEST(Algorithm,Rotate){
    int A[] = {1,2,3,4,5};
    rotate(A,A+2,A+5);
}

struct test_js{
    template <typename T>
    bool operator()(T const&a){
        return a%2;
    }
};

TEST(Algorithm,Partial_sum){
    int A[] = { 1,2,3,4,5,6 };
    copy(A,A+6,ostream_iterator<int>(std::cout," "),test_js());
    std::cout<<std::endl;
}

int main(int argc,char* argv[]){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

