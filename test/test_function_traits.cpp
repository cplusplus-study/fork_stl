#include <base/__function_traits.hpp>
#include <gtest/gtest.h>


TEST(TestFunctionTraits, ReturnType){
    ASSERT_TRUE((std::is_same<xusd::base::function_traits<void()>::return_type, void>::value));
    ASSERT_TRUE((std::is_same<xusd::base::function_traits<int()>::return_type,int>::value));
    ASSERT_TRUE((std::is_same<xusd::base::function_traits<void(int)>::return_type,void>::value));
    ASSERT_TRUE((std::is_same<xusd::base::function_traits<int(int, long, char)>::return_type,int>::value));
    ASSERT_TRUE((std::is_same<xusd::base::function_traits<class AAA(class AAA,class BBB, class CCC)>::return_type,class AAA>::value));



    ASSERT_TRUE((std::is_same<xusd::base::function_traits<int&()>::return_type,int&>::value));
    ASSERT_TRUE((std::is_same<xusd::base::function_traits<int const &()>::return_type,int const&>::value));
    ASSERT_TRUE((std::is_same<xusd::base::function_traits<int&&()>::return_type,int&&>::value));
    ASSERT_TRUE((std::is_same<xusd::base::function_traits<int*()>::return_type,int*>::value));
    ASSERT_TRUE((std::is_same<xusd::base::function_traits<int const *()>::return_type,int const*>::value));
    ASSERT_TRUE((std::is_same<xusd::base::function_traits<int* const()>::return_type,int* const>::value));

    ASSERT_TRUE((std::is_same<xusd::base::function_traits<int (&())[]>::return_type,int(&)[]>::value));
    ASSERT_TRUE((std::is_same<xusd::base::function_traits<int (&())[10]>::return_type,int(&)[10]>::value));
}

TEST(TestFunctionTraits, Arity){
    ASSERT_EQ((xusd::base::function_traits<void()>::arity), 0);
    ASSERT_EQ((xusd::base::function_traits<int()>::arity), 0);
    ASSERT_EQ((xusd::base::function_traits<void(int)>::arity), 1);
    ASSERT_EQ((xusd::base::function_traits<int(int, long, char)>::arity), 3);
    ASSERT_EQ((xusd::base::function_traits<class AAA(class AAA,class BBB, class CCC)>::arity), 3);
}
TEST(TestFunctionTraits, ArgN){
    ASSERT_TRUE((std::is_same<xusd::base::function_traits<void(int)>::arg<0>::type,int>::value));
    ASSERT_TRUE((std::is_same<xusd::base::function_traits<int(int, long, char)>::arg<0>::type,int>::value));
    ASSERT_TRUE((std::is_same<xusd::base::function_traits<int(int, long, char)>::arg<1>::type, long>::value));
    ASSERT_TRUE((std::is_same<xusd::base::function_traits<int(int, long, char)>::arg<2>::type,char>::value));
    ASSERT_TRUE((std::is_same<xusd::base::function_traits<class AAA(class AAA,class BBB, class CCC)>::arg<0>::type,class AAA>::value));
    ASSERT_TRUE((std::is_same<xusd::base::function_traits<class AAA(class AAA,class BBB, class CCC)>::arg<1>::type,class BBB>::value));
    ASSERT_TRUE((std::is_same<xusd::base::function_traits<class AAA(class AAA,class BBB, class CCC)>::arg<2>::type,class CCC>::value));
}


class AAA{
public:
    int mem1;
    int fun1(int);
    int const fun2(int, char)const;
    int& fun3(int, char, long)volatile;
    int* fun4(int, char, long, double)const volatile;
};
TEST(TestFunctionTraits, MembersReturnType){

    ASSERT_TRUE((std::is_same<xusd::base::function_traits<decltype(&AAA::fun1)>::return_type, int>::value));
    ASSERT_TRUE((std::is_same<xusd::base::function_traits<decltype(&AAA::fun2)>::return_type, int const>::value));
    ASSERT_TRUE((std::is_same<xusd::base::function_traits<decltype(&AAA::fun3)>::return_type, int&>::value));
    ASSERT_TRUE((std::is_same<xusd::base::function_traits<decltype(&AAA::fun4)>::return_type, int*>::value));

    ASSERT_TRUE((std::is_same<xusd::base::function_traits<decltype(&AAA::mem1)>::return_type, int>::value));

}

int&& fun222();
TEST(TestFunctionTraits, MembersFunctionArity){

    ASSERT_EQ((xusd::base::function_traits<decltype(&AAA::fun1)>::arity), 2);
    ASSERT_EQ((xusd::base::function_traits<decltype(&AAA::fun2)>::arity), 3);
    ASSERT_EQ((xusd::base::function_traits<decltype(&AAA::fun3)>::arity), 4);
    ASSERT_EQ((xusd::base::function_traits<decltype(&AAA::fun4)>::arity), 5);
    ASSERT_EQ((xusd::base::function_traits<decltype(&AAA::mem1)>::arity), 1);

    int i = 0;
    int &j = i;
    ASSERT_TRUE((std::is_same<decltype(i+0), int>::value));
    ASSERT_TRUE((std::is_same<decltype(fun222()), int&&>::value));
    
}

int main(int argc, char* argv[]){

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
