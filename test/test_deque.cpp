#include <gtest/gtest.h>
#include <deque.hpp>
#include <algorithm.hpp>
#include <iostream>
using namespace xusd;

#define PRINT_DEQUE(a) { \
    copy(a.begin(),a.end(),ostream_iterator<int>(std::cout, " ")); \
    std::cout<<std::endl;\
    }

TEST(Deque,Constructor){
    deque<int> a;
    ASSERT_TRUE(a.empty());
    ASSERT_EQ(a.size(),deque<int>::size_type(0));
    ASSERT_EQ(a.capacity(),deque<int>::size_type(0));

    int Arr[] = { 1, 2, 3, 4, 5, 6, 7 };
    deque<int> b(Arr,Arr+7);
    ASSERT_FALSE(b.empty());
    ASSERT_EQ(b.size(),deque<int>::size_type(7));
    ASSERT_EQ(b.capacity(),deque<int>::size_type(8));

    EXPECT_TRUE(equal(b.begin(),b.end(),Arr));

    copy(b.begin(),b.end(),ostream_iterator<int>(std::cout, " "));
    std::cout<<std::endl;

}

TEST(Deque,Insert){
    deque<int> a;
    typedef deque<int>::iterator iterator;
    ASSERT_EQ(a.capacity(),deque<int>::size_type(0));
    iterator begin = a.begin();
    iterator ret = a.insert(begin,1);
    //ASSERT_EQ(a.capacity(),deque<int>::size_type(7));
    PRINT_DEQUE(a);
    begin = a.begin();
    ret = a.insert(begin,0);
    PRINT_DEQUE(a);
    iterator end = a.end();
    ret = a.insert(end,2);
    PRINT_DEQUE(a);

    for(int i = 3;i < 10;i = i + 2){
        a.push_back(i);
    }
    PRINT_DEQUE(a);

    a.insert(a.begin()+4,4);
    PRINT_DEQUE(a);
    a.insert(a.begin()+6,6);
    PRINT_DEQUE(a);
    a.insert(a.begin()+8,8);
    PRINT_DEQUE(a);
    a.insert(a.begin()+10,10);
    PRINT_DEQUE(a);


}

int main(int argc,char* argv[]){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

