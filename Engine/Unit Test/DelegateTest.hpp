#include "UnitTestPch.h"

#include "Engine/Delegates/Delegate.hpp"

// Define a class with methods to be tested
struct TestClass
{
    int Value;

    int Increment(int amount)
    {
        Value += amount;
        return Value;
    }
};

// Define a free function to be tested
int FreeFunction(int a, int b)
{
    return a + b;
}

// Test cases for the Delegate class
TEST(DelegateTest, FreeFunctionTest)
{
    Delegate<int(int, int)> delegateFreeFunc(&FreeFunction);
    EXPECT_EQ(delegateFreeFunc(2, 3), 5);
}

TEST(DelegateTest, MemberFunctionTest)
{
    TestClass obj{ 5 };
    Delegate<int(int)> delegateMemberFunc(&TestClass::Increment, &obj);
    EXPECT_EQ(delegateMemberFunc(3), 8); // obj.Value should now be 8
}

TEST(DelegateTest, LambdaTest)
{
    Delegate<int(int, int)> delegateLambda([](int a, int b) { return a * b; });
    EXPECT_EQ(delegateLambda(4, 2), 8);
}

TEST(DelegateTest, CopyConstructorTest)
{
    std::cout << "Test started" << std::endl;
    {
        std::cout << "Creating original delegate" << std::endl;
        auto delegateLambda = std::make_unique<Delegate<int(int, int)>>([](int a, int b) { return a * b; });
        std::cout << "Original delegate created" << std::endl;

        {
            std::cout << "Creating copied delegate" << std::endl;
            auto copiedDelegate = std::make_unique<Delegate<int(int, int)>>(*delegateLambda);
            std::cout << "Copied delegate created" << std::endl;

            EXPECT_EQ((*copiedDelegate)(4, 2), 8);
        }
        std::cout << "Copied delegate destroyed" << std::endl;
    }
    std::cout << "Original delegate destroyed" << std::endl;
    std::cout << "Test completed" << std::endl;
}