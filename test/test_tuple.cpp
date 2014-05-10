#include<tuple>
#include <type_traits>
#include<gtest/gtest.h>

TEST(TestTuple, forward_as_tuple){
    auto t1 = std::forward_as_tuple(1);
    EXPECT_EQ(1, (std::tuple_size<decltype(t1)>::value));
    EXPECT_TRUE((std::is_same<std::tuple<int &&>,decltype(t1)>::value));

    int i = 0;
    auto t2 = std::forward_as_tuple(std::move(i));
    EXPECT_TRUE((std::is_same<std::tuple<int&&>,decltype(t2)>::value));
    auto ii = std::get<0>(t2);
    EXPECT_TRUE((std::is_same<int, decltype(ii)>::value));

    class A{
    public:
        A():i(7){ }
        ~A(){ }
        A(A&& o):i(o.i+1){EXPECT_TRUE(false); }
        A(const A&o):i(o.i){ }
        int get(){ return i; }
    private:
        int i;
    };


    A a;
    auto t3 = std::forward_as_tuple(std::move(a));
    EXPECT_TRUE((std::is_same<std::tuple<A&&>,decltype(t3)>::value));
    auto a2 = std::get<0>(t3);
    EXPECT_TRUE((std::is_same<A&,decltype(std::get<0>(t3))>::value));
    EXPECT_EQ(7, a2.get());
}

int main(int argc,char* argv[]){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
