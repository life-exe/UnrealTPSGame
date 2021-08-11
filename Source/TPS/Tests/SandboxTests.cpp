// My game copyright

#include "TPS/Tests/SandboxTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathMaxInt, "TPSGame.Math.MaxInt",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathSqrt, "TPSGame.Math.Sqrt",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathSin, "TPSGame.Math.Sin",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FMathMaxInt::RunTest(const FString& Parameters)
{
    AddInfo("Max [int] func testing");

    /*if (1 != 2)
    {
        AddWarning("1 not equal 2");
        return true;
    }*/

    TestTrue("2 different positive numbers", FMath::Max(13, 25) == 25);
    TestEqual("2 equal positive numbers", FMath::Max(25, 25), 25);
    TestTrueExpr(FMath::Max(0, 123) == 123);
    TestTrue("2 zeroes", FMath::Max(0, 0) == 0);
    TestTrue("Negative number and zero", FMath::Max(-2345, 0) == 0);
    TestTrue("2 different negative numbers", FMath::Max(-45, -67) == -45);
    TestTrue("2 equal negative numbers", FMath::Max(-9, -9) == -9);
    TestTrue("Positive vs. negative number", FMath::Max(-78, 34) == 34);

    return true;
}

bool FMathSqrt::RunTest(const FString& Parameters)
{
    // sqrt(3) = 1.73205...

    AddInfo("Sqrt func testing");

    TestEqual("Sqrt(4) [0]", FMath::Sqrt(4.0f), 2.0f);
    // TestEqual("Sqrt(3) [1]", FMath::Sqrt(3.0f), 1.7f);
    TestEqual("Sqrt(3) [2]", FMath::Sqrt(3.0f), 1.7f, 0.1f);
    TestEqual("Sqrt(3) [3]", FMath::Sqrt(3.0f), 1.73f, 0.01f);
    TestEqual("Sqrt(3) [4]", FMath::Sqrt(3.0f), 1.73205f, 1.e-5f);

    return true;
}

// Homework ------> testing FMath::Sin()
bool FMathSin::RunTest(const FString& Parameters)
{
    AddInfo("Sin func testing");

    return true;
}
// <------ Homework
