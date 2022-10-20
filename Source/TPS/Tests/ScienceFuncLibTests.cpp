// My game copyright

#if WITH_AUTOMATION_TESTS

#include "TPS/Tests/ScienceFuncLibTests.h"
#include "TPS/Tests/TestUtils.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TPS/Science/ScienceFuncLib.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciSimple, "TPSGame.Science.Fibonacci.Simple",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciStress, "TPSGame.Science.Fibonacci.Stress",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::StressFilter | EAutomationTestFlags::LowPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciLogHasErrors, "TPSGame.Science.Fibonacci.LogHasErrors",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

DEFINE_SPEC(FFactorial, "TPSGame.Science.Factorial",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority)

bool FFibonacciSimple::RunTest(const FString& Parameters)
{
    AddInfo("Fibonacci simple testing");

    // 0 1 1 2 3 5 8 ...

    TestTrueExpr(UScienceFuncLib::Fibonacci(0) == 0);
    TestTrueExpr(UScienceFuncLib::Fibonacci(1) == 1);
    TestTrueExpr(UScienceFuncLib::Fibonacci(2) == 1);
    TestTrueExpr(UScienceFuncLib::Fibonacci(3) == 2);

    const TArray<TPS::Test::TestPayload<int32, int32>> TestData{{0, 0}, {1, 1}, {2, 1}, {3, 2}, {4, 3}, {5, 5}};
    for (const auto Data : TestData)
    {
        // TestTrueExpr(UScienceFuncLib::Fibonacci(Data.TestValue) == Data.ExpectedValue);
        const FString InfoString = FString::Printf(TEXT("test value: %i, expected value: %i"), Data.TestValue, Data.ExpectedValue);
        TestEqual(InfoString, UScienceFuncLib::Fibonacci(Data.TestValue), Data.ExpectedValue);
    }

    return true;
}

bool FFibonacciStress::RunTest(const FString& Parameters)
{
    AddInfo("Fibonacci stress testing");

    /* for (int32 i = 2; i < 40; ++i)
    {
        TestTrueExpr(UScienceFuncLib::Fibonacci(i) ==  //
                     UScienceFuncLib::Fibonacci(i - 1) + UScienceFuncLib::Fibonacci(i - 2));
    }*/

    int32 PrevPrevValue = 0;
    int32 PrevValue = 1;

    for (int32 i = 2; i < 40; ++i)
    {
        const int32 CurrentValue = UScienceFuncLib::Fibonacci(i);
        TestTrueExpr(CurrentValue == PrevPrevValue + PrevValue);

        PrevPrevValue = PrevValue;
        PrevValue = CurrentValue;
    }

    return true;
}

bool FFibonacciLogHasErrors::RunTest(const FString& Parameters)
{
    AddInfo("Fibonacci negative number on input produces error");

    AddExpectedError("Invalid input for Fibonacci", EAutomationExpectedErrorFlags::Contains);
    UScienceFuncLib::Fibonacci(-10);

    return true;
}

void FFactorial::Define()
{
    Describe("Corner cases",
        [this]()
        {
            It("Factorial of 0 should be equal 1", [this]() { TestTrueExpr(UScienceFuncLib::Factorial(0) == 1); });
            It("Factorial of 1 should be equal 1", [this]() { TestTrueExpr(UScienceFuncLib::Factorial(1) == 1); });
            It("Factorial of negative number is undefined and func should return -1",
                [this]() { TestTrueExpr(UScienceFuncLib::Factorial(-23) == -1); });
        });
    Describe("Normal cases",
        [this]()
        {
            It("Factorial of 2 should be equal 2", [this]() { TestTrueExpr(UScienceFuncLib::Factorial(2) == 2); });
            It("Factorial of 3 should be equal 6", [this]() { TestTrueExpr(UScienceFuncLib::Factorial(3) == 6); });
            It("Factorial of 4 should be equal 24", [this]() { TestTrueExpr(UScienceFuncLib::Factorial(4) == 24); });

            using namespace TPS::Test;
            const TArray<TestPayload<int32, int32>> TestData{{5, 120}, {6, 720}, {7, 5040}, {8, 40320}};
            for (const auto& Data : TestData)
            {
                It(FString::Printf(TEXT("Factorial of %i should be equal %i"), Data.TestValue, Data.ExpectedValue),
                    [this, Data]() { TestTrueExpr(UScienceFuncLib::Factorial(Data.TestValue) == Data.ExpectedValue); });
            }
        });
}

#endif