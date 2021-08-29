// My game copyright

#include "TPS/Science/ScienceFuncLib.h"

DEFINE_LOG_CATEGORY_STATIC(LogScience, All, All);

int32 UScienceFuncLib::Fibonacci(int32 Value)
{
    // check(Value >= 0);
    if (Value < 0)
    {
        UE_LOG(LogScience, Error, TEXT("Invalid input for Fibonacci: %i"), Value);
    }
    return Value <= 1 ? Value : Fibonacci(Value - 1) + Fibonacci(Value - 2);
}
