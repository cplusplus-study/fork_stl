#include <__tuple_mpl.hpp>
#include <type_traits>
#include <gtest/gtest.h>


TEST(TestTuplMpl, tuple_prefix){
    EXPECT_TRUE((std::is_same<std::tuple<int>, xusd::tuple_prefix<int, std::tuple<> >::type>::value));
    EXPECT_TRUE((std::is_same<std::tuple<int, long>, xusd::tuple_prefix<int, std::tuple<long> >::type>::value));
    EXPECT_TRUE((std::is_same<std::tuple<class CCC, class BBB, class AAA>, xusd::tuple_prefix<class CCC, std::tuple<class BBB, class AAA> >::type>::value));
}
TEST(TestTuplMpl, tuple_append){
    EXPECT_TRUE((std::is_same<std::tuple<int>, xusd::tuple_append<int, std::tuple<> >::type>::value));
    EXPECT_TRUE((std::is_same<std::tuple<long, int>, xusd::tuple_append<int, std::tuple<long> >::type>::value));
    EXPECT_TRUE((std::is_same<std::tuple<class CCC, class BBB, class AAA>, xusd::tuple_append<class AAA, std::tuple<class CCC, class BBB> >::type>::value));
}
TEST(TestTuplMpl, tuple_erase){
    EXPECT_TRUE((std::is_same<std::tuple<>, xusd::tuple_erase<int, std::tuple<> >::type>::value));
    EXPECT_TRUE((std::is_same<std::tuple<>, xusd::tuple_erase<int, std::tuple<int> >::type>::value));
    EXPECT_TRUE((std::is_same<std::tuple<char>, xusd::tuple_erase<int, std::tuple<char> >::type>::value));


    EXPECT_TRUE((std::is_same<std::tuple<char>, xusd::tuple_erase<int, std::tuple<int, char> >::type>::value));
    EXPECT_TRUE((std::is_same<std::tuple<char>, xusd::tuple_erase<int, std::tuple<char, int> >::type>::value));

    EXPECT_TRUE((std::is_same<std::tuple<int, int, char>, xusd::tuple_erase<int, std::tuple<int, int, int, char> >::type>::value));
}

TEST(TestTuplMpl, tuple_erase_all){
    EXPECT_TRUE((std::is_same<std::tuple<>, xusd::tuple_erase_all<int, std::tuple<> >::type>::value));
    EXPECT_TRUE((std::is_same<std::tuple<>, xusd::tuple_erase_all<int, std::tuple<int> >::type>::value));
    EXPECT_TRUE((std::is_same<std::tuple<char>, xusd::tuple_erase_all<int, std::tuple<char> >::type>::value));


    EXPECT_TRUE((std::is_same<std::tuple<char>, xusd::tuple_erase_all<int, std::tuple<int, char> >::type>::value));
    EXPECT_TRUE((std::is_same<std::tuple<char>, xusd::tuple_erase_all<int, std::tuple<char, int> >::type>::value));

    EXPECT_TRUE((std::is_same<std::tuple<char>, xusd::tuple_erase_all<int, std::tuple<int, int, int, char> >::type>::value));
}

TEST(TestTuplMpl, tuple_erase_idx){
    EXPECT_TRUE((std::is_same<std::tuple<>, xusd::tuple_erase_idx<0, std::tuple<int> >::type>::value));
    EXPECT_TRUE((std::is_same<std::tuple<char>, xusd::tuple_erase_idx<1, std::tuple<char, int> >::type>::value));
    EXPECT_TRUE((std::is_same<std::tuple<int>, xusd::tuple_erase_idx<0, std::tuple<char, int> >::type>::value));
}

TEST(TestTupleMpl, tuple_erase_dup){
    EXPECT_TRUE((std::is_same<std::tuple<>, xusd::tuple_erase_dup<std::tuple<> >::type>::value));
    EXPECT_TRUE((std::is_same<std::tuple<int>, xusd::tuple_erase_dup<std::tuple<int> >::type>::value));
    EXPECT_TRUE((std::is_same<std::tuple<int>, xusd::tuple_erase_dup<std::tuple<int, int> >::type>::value));
    EXPECT_TRUE((std::is_same<std::tuple<int, char>, xusd::tuple_erase_dup<std::tuple<int, int, char, int> >::type>::value));
}
TEST(TestTupleMpl, tuple_uniq){
    EXPECT_TRUE((std::is_same<std::tuple<>, xusd::tuple_uniq<std::tuple<> >::type>::value));
    EXPECT_TRUE((std::is_same<std::tuple<int>, xusd::tuple_uniq<std::tuple<int> >::type>::value));
    EXPECT_TRUE((std::is_same<std::tuple<int>, xusd::tuple_uniq<std::tuple<int, int> >::type>::value));
    EXPECT_TRUE((std::is_same<std::tuple<int, char>, xusd::tuple_uniq<std::tuple<int, int, char, int> >::type>::value));
}

TEST(TestTupleMpl, tuple_insert){
    EXPECT_TRUE((std::is_same<std::tuple<int>, xusd::tuple_insert<0,int, std::tuple<> >::type>::value));
}


int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
