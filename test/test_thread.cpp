#include <thread.hpp>
#include <bind.hpp>
#include <gtest/gtest.h>

TEST(TestThread, test__thread_execute){
    int x = 0;
    auto f1 = std::bind(std::bind(std::bind([&](int a, int b){ x = a+b; return x; }, 1, 2)));
    EXPECT_EQ(f1(), 3);
    EXPECT_EQ(x, 3);
    typedef std::tuple<decltype(f1)> mytuple;
    mytuple f2(f1);
    xusd::__thread_execute(f2, xusd::seq<0>());
}

int g_test_thread_run_count = 0;
int thread_fun(){
    return ++g_test_thread_run_count;
}

TEST(TestThread, testRun){
    xusd::thread a(&thread_fun);
    ASSERT_TRUE(a.joinable());
    a.join();
    ASSERT_EQ(g_test_thread_run_count, 1);
    ASSERT_FALSE(a.joinable());

    EXPECT_EQ(xusd::thread::hardware_concurrency(),8);// my computer is 8
}

TEST(TestThread, testSleepFor){
    auto start = xusd::chrono::system_clock::now();
    xusd::this_thread::sleep_for(xusd::chrono::seconds(1));
    auto end = xusd::chrono::system_clock::now();
    EXPECT_TRUE((end - start >= xusd::chrono::seconds(1)));
    EXPECT_TRUE((end - start <= xusd::chrono::seconds(2))); // warrning!!!
}



int main(int argc, char* argv[]){

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
