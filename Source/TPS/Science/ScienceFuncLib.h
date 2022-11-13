// My game copyright

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ScienceFuncLib.generated.h"

UCLASS()
class TPS_API UScienceFuncLib : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * Calculates Fibonacci number for the input number
     * https://en.wikipedia.org/wiki/Fibonacci_number
     *
     * \f$F_n = F_{n-1} + F_{n-2}, n > 2 \\
          F_1 = F_2 = 1, F_0 = 0\f$
     *
     * @param   Value  Number for which Fibonacci number should be calculated
     * @return         Fibonacci number or (-Value) for negative numbers
     */
    UFUNCTION(BlueprintPure, Category = Science)
    static int32 Fibonacci(int32 Value);

    /**
     * Calculates factorial for the input number
     * https://en.wikipedia.org/wiki/Factorial
     *
     * \f$ n! = n(n-1)(n-2)\cdots (2)(1) \\
        0! = 1 \f$
     *
     *
     * @param   Value  Number for which factorial should be calculated
     * @return         Factorial or (-1) for negative numbers
     */
    UFUNCTION(BlueprintPure, Category = Science)
    static int32 Factorial(int32 Value);
};
