#pragma once

#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

#include "CoreMinimal.h"

namespace TPS
{
namespace Test
{
template <typename Type1, typename Type2>
struct TestPayload
{
    Type1 TestValue;
    Type2 ExpectedValue;
    float Tolerance = KINDA_SMALL_NUMBER;
};

#define ENUM_LOOP_START(TYPE, EnumElem)                                        \
    for (int32 index = 0; index < StaticEnum<TYPE>()->NumEnums() - 1; ++index) \
    {                                                                          \
        const auto EnumElem = static_cast<TYPE>(index);
#define ENUM_LOOP_END }
}  // namespace Test
}  // namespace TPS

#endif
