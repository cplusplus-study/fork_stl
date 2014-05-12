#include <ratio.hpp>
#include <gtest/gtest.h>

TEST(TestRatio, TestRatirNum){
    
    EXPECT_EQ(0, (xusd::ratio<0>::num));
    EXPECT_EQ(1, (xusd::ratio<1>::num));
    EXPECT_EQ(100, (xusd::ratio<100>::num));


    EXPECT_EQ(0, (xusd::ratio<0,2>::num));
    EXPECT_EQ(1, (xusd::ratio<1,2>::num));
    EXPECT_EQ(50, (xusd::ratio<100,2>::num));

    EXPECT_EQ(-0, (xusd::ratio<0,-2>::num));
    EXPECT_EQ(-1, (xusd::ratio<1,-2>::num));
    EXPECT_EQ(-50, (xusd::ratio<100,-2>::num));

    EXPECT_EQ(-0, (xusd::ratio<-0,2>::num));
    EXPECT_EQ(-1, (xusd::ratio<-1,2>::num));
    EXPECT_EQ(-50, (xusd::ratio<-100,2>::num));
}

TEST(TestRatio, TestRatioDen){
    
    EXPECT_EQ(1, (xusd::ratio<0>::den));
    EXPECT_EQ(1, (xusd::ratio<1>::den));
    EXPECT_EQ(1, (xusd::ratio<100>::den));


    EXPECT_EQ(1, (xusd::ratio<0,2>::den));
    EXPECT_EQ(2, (xusd::ratio<1,2>::den));
    EXPECT_EQ(1, (xusd::ratio<100,2>::den));

    EXPECT_EQ(1, (xusd::ratio<0,-2>::den));
    EXPECT_EQ(2, (xusd::ratio<1,-2>::den));
    EXPECT_EQ(1, (xusd::ratio<100,-2>::den));

    EXPECT_EQ(1, (xusd::ratio<-0,2>::den));
    EXPECT_EQ(2, (xusd::ratio<-1,2>::den));
    EXPECT_EQ(1, (xusd::ratio<-100,2>::den));
}

TEST(TestRatio, TestRatioAdd){
    
    EXPECT_TRUE((std::is_same<xusd::ratio_add<xusd::ratio<1,2>, xusd::ratio<1,3> >::type, xusd::ratio<5,6> >::value));
    EXPECT_TRUE((std::is_same<xusd::ratio_add<xusd::ratio<1,2>, xusd::ratio<0> >::type, xusd::ratio<1,2> >::value));
    EXPECT_TRUE((std::is_same<xusd::ratio_add<xusd::ratio<-1,2>, xusd::ratio<0> >::type, xusd::ratio<-1,2> >::value));
    EXPECT_TRUE((std::is_same<xusd::ratio_add<xusd::ratio<-1,2>, xusd::ratio<0> >::type, xusd::ratio<-1,2> >::value));
    EXPECT_TRUE((std::is_same<xusd::ratio_add<xusd::ratio<1,-2>, xusd::ratio<0> >::type, xusd::ratio<-1,2> >::value));

    EXPECT_TRUE((std::is_same<xusd::ratio_add<xusd::ratio<1,100>, xusd::ratio<1,3> >::type, xusd::ratio<103,300> >::value));
}

TEST(TestRatio, TestRatioSub){
    
    EXPECT_TRUE((std::is_same<xusd::ratio_subtract<xusd::ratio<1,2>, xusd::ratio<1,3> >::type, xusd::ratio<1,6> >::value));
    EXPECT_TRUE((std::is_same<xusd::ratio_subtract<xusd::ratio<1,2>, xusd::ratio<0> >::type, xusd::ratio<1,2> >::value));
    EXPECT_TRUE((std::is_same<xusd::ratio_subtract<xusd::ratio<-1,2>, xusd::ratio<0> >::type, xusd::ratio<-1,2> >::value));
    EXPECT_TRUE((std::is_same<xusd::ratio_subtract<xusd::ratio<-1,2>, xusd::ratio<0> >::type, xusd::ratio<-1,2> >::value));
    EXPECT_TRUE((std::is_same<xusd::ratio_subtract<xusd::ratio<1,-2>, xusd::ratio<0> >::type, xusd::ratio<-1,2> >::value));
    EXPECT_TRUE((std::is_same<xusd::ratio_subtract<xusd::ratio<1,3>, xusd::ratio<1,100> >::type, xusd::ratio<97,300> >::value));
    EXPECT_TRUE((std::is_same<xusd::ratio_subtract<xusd::ratio<1,100>, xusd::ratio<1,3> >::type, xusd::ratio<-97,300> >::value));
}

TEST(TestRatio, TestRatioMultiply){
    
    EXPECT_TRUE((std::is_same<xusd::ratio_multiply<xusd::ratio<1,2>, xusd::ratio<1,3> >::type, xusd::ratio<1,6> >::value));
    EXPECT_TRUE((std::is_same<xusd::ratio_multiply<xusd::ratio<1,2>, xusd::ratio<0> >::type, xusd::ratio<0> >::value));
    EXPECT_TRUE((std::is_same<xusd::ratio_multiply<xusd::ratio<-1,2>, xusd::ratio<0> >::type, xusd::ratio<0> >::value));
    EXPECT_TRUE((std::is_same<xusd::ratio_multiply<xusd::ratio<-1,2>, xusd::ratio<0> >::type, xusd::ratio<0> >::value));
    EXPECT_TRUE((std::is_same<xusd::ratio_multiply<xusd::ratio<1,-2>, xusd::ratio<0> >::type, xusd::ratio<0> >::value));
    EXPECT_TRUE((std::is_same<xusd::ratio_multiply<xusd::ratio<1,3>, xusd::ratio<1,100> >::type, xusd::ratio<1,300> >::value));
    EXPECT_TRUE((std::is_same<xusd::ratio_multiply<xusd::ratio<1,100>, xusd::ratio<1,3> >::type, xusd::ratio<1,300> >::value));
}

TEST(TestRatio, TestRatioDivide){
    
    EXPECT_TRUE((std::is_same<xusd::ratio_divide<xusd::ratio<1,2>, xusd::ratio<1,3> >::type, xusd::ratio<3,2> >::value));
    EXPECT_TRUE((std::is_same<xusd::ratio_divide<xusd::ratio<0>, xusd::ratio<1,2> >::type, xusd::ratio<0> >::value));
    EXPECT_TRUE((std::is_same<xusd::ratio_divide<xusd::ratio<1,3>, xusd::ratio<1,100> >::type, xusd::ratio<100,3> >::value));
    EXPECT_TRUE((std::is_same<xusd::ratio_divide<xusd::ratio<1,100>, xusd::ratio<1,3> >::type, xusd::ratio<3,100> >::value));
}

TEST(TestRatio, TestRatioEQ){
    
    EXPECT_TRUE((xusd::ratio_equal<xusd::ratio<0>, xusd::ratio<0,2> >::value));
    EXPECT_TRUE((xusd::ratio_equal<xusd::ratio<1,2>, xusd::ratio<1,2> >::value));
    EXPECT_TRUE((xusd::ratio_equal<xusd::ratio<1,2>, xusd::ratio<3,6> >::value));
}
TEST(TestRatio, TestRatioNE){
    
    EXPECT_FALSE((xusd::ratio_not_equal<xusd::ratio<0>, xusd::ratio<0,2> >::value));
    EXPECT_FALSE((xusd::ratio_not_equal<xusd::ratio<1,2>, xusd::ratio<1,2> >::value));
    EXPECT_FALSE((xusd::ratio_not_equal<xusd::ratio<1,2>, xusd::ratio<3,6> >::value));
}

TEST(TestRatio, TestRatioLT){
    
    EXPECT_TRUE((xusd::ratio_less<xusd::ratio<0>,xusd::ratio<1>>::value));
    EXPECT_TRUE((xusd::ratio_less<xusd::ratio<1, 3>,xusd::ratio<1, 2>>::value));
    EXPECT_TRUE((xusd::ratio_less<xusd::ratio<1, -2>,xusd::ratio<1, -3>>::value));
}
TEST(TestRatio, TestRatioLE){
    
    EXPECT_TRUE((xusd::ratio_less_equal<xusd::ratio<0>,xusd::ratio<0>>::value));
    EXPECT_TRUE((xusd::ratio_less_equal<xusd::ratio<0>,xusd::ratio<1>>::value));
    EXPECT_TRUE((xusd::ratio_less_equal<xusd::ratio<1, 3>,xusd::ratio<1, 2>>::value));
    EXPECT_TRUE((xusd::ratio_less_equal<xusd::ratio<1, -2>,xusd::ratio<1, -3>>::value));
}
TEST(TestRatio, TestRatioGT){
    
    EXPECT_FALSE((xusd::ratio_greater<xusd::ratio<0>,xusd::ratio<1>>::value));
    EXPECT_FALSE((xusd::ratio_greater<xusd::ratio<1, 3>,xusd::ratio<1, 2>>::value));
    EXPECT_FALSE((xusd::ratio_greater<xusd::ratio<1, -2>,xusd::ratio<1, -3>>::value));
}
TEST(TestRatio, TestRatioGE){
    
    EXPECT_TRUE((xusd::ratio_greater_equal<xusd::ratio<0>,xusd::ratio<0>>::value));
    EXPECT_FALSE((xusd::ratio_greater_equal<xusd::ratio<0>,xusd::ratio<1>>::value));
    EXPECT_FALSE((xusd::ratio_greater_equal<xusd::ratio<1, 3>,xusd::ratio<1, 2>>::value));
    EXPECT_FALSE((xusd::ratio_greater_equal<xusd::ratio<1, -2>,xusd::ratio<1, -3>>::value));
}

int main(int argc, char* argv[]){

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
