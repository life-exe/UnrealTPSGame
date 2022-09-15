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
     * @param   Value  Number for which Fibonacci number should be calculated
     * @return         Fibonacci number or (-Value) for negative numbers
     */
    UFUNCTION(BlueprintPure, Category = Science)
    static int32 Fibonacci(int32 Value);

    /**
     * Calculates factorial for the input number
     * https://en.wikipedia.org/wiki/Factorial
     *
     * @param   Value  Number for which factorial should be calculated
     * @return         Factorial or (-1) for negative numbers
     */
    UFUNCTION(BlueprintPure, Category = Science)
    static int32 Factorial(int32 Value);
};
