
#include <chrono.hpp>
#include <gtest/gtest.h>
TEST(TestDuration, testCount){
    EXPECT_EQ(0, xusd::chrono::duration<int>(0).count());
    EXPECT_EQ(std::numeric_limits<int>::max(), xusd::chrono::duration<int>(std::numeric_limits<int>::max()).count());
    EXPECT_EQ(std::numeric_limits<int>::min(), xusd::chrono::duration<int>(std::numeric_limits<int>::min()).count());

    EXPECT_EQ(0, xusd::chrono::duration<int>::zero().count());
    EXPECT_EQ(std::numeric_limits<int>::max(), xusd::chrono::duration<int>::max().count());
    EXPECT_EQ(std::numeric_limits<int>::min(), xusd::chrono::duration<int>::min().count());
    
    EXPECT_EQ(xusd::chrono::milliseconds(1000).count(), xusd::chrono::duration_cast<xusd::chrono::milliseconds>(xusd::chrono::seconds(1)).count());

}

TEST(TestDuration, testCommonType){
    EXPECT_TRUE((std::is_same<xusd::chrono::duration<int, xusd::ratio<1,91>>, std::common_type<xusd::chrono::duration<int, xusd::ratio<3,7>>,xusd::chrono::duration<int, xusd::ratio<4,13>>>::type>::value));
    EXPECT_TRUE((std::is_same<xusd::chrono::duration<float, xusd::ratio<1,91>>, std::common_type<xusd::chrono::duration<long, xusd::ratio<3,7>>,xusd::chrono::duration<float, xusd::ratio<4,13>>>::type>::value));
    EXPECT_TRUE((std::is_same<xusd::chrono::duration<double, xusd::ratio<1,91>>, std::common_type<xusd::chrono::duration<double, xusd::ratio<3,7>>,xusd::chrono::duration<float, xusd::ratio<4,13>>>::type>::value));

    EXPECT_TRUE((std::is_same<xusd::chrono::duration<float, xusd::ratio<1,21>>, std::common_type<xusd::chrono::duration<long, xusd::ratio<1,7>>,xusd::chrono::duration<float, xusd::ratio<1,3>>>::type>::value));
    EXPECT_TRUE((std::is_same<xusd::chrono::duration<float, xusd::ratio<1,7>>, std::common_type<xusd::chrono::duration<long, xusd::ratio<4,7>>,xusd::chrono::duration<float, xusd::ratio<3,7>>>::type>::value));

    EXPECT_TRUE((std::is_same<xusd::chrono::duration<float, xusd::ratio<3,7>>, std::common_type<xusd::chrono::duration<long, xusd::ratio<3,7>>,xusd::chrono::duration<float, xusd::ratio<3,7>>>::type>::value));
}
TEST(TestDuration, testDuration){
    EXPECT_TRUE((xusd::chrono::seconds(1) == xusd::chrono::milliseconds(1000)));  
    EXPECT_TRUE((xusd::chrono::seconds(1) == xusd::chrono::microseconds(1000000)));  
    EXPECT_TRUE((xusd::chrono::seconds(1) == xusd::chrono::nanoseconds(1000000000)));  

    EXPECT_TRUE((xusd::chrono::milliseconds(999)< xusd::chrono::seconds(1)));
    EXPECT_TRUE((xusd::chrono::microseconds(999999)< xusd::chrono::seconds(1)));
    EXPECT_TRUE((xusd::chrono::nanoseconds(999999999)< xusd::chrono::seconds(1)));

    EXPECT_TRUE((xusd::chrono::milliseconds(999) + xusd::chrono::seconds(1) == xusd::chrono::milliseconds(1999)));
    EXPECT_TRUE((xusd::chrono::microseconds(999) + xusd::chrono::seconds(1) == xusd::chrono::microseconds(1000999)));
    EXPECT_TRUE((xusd::chrono::nanoseconds(999) + xusd::chrono::seconds(1) == xusd::chrono::nanoseconds(1000000999)));
}

int main(int argc, char* argv[]){

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
