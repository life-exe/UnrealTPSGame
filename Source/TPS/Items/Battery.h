// My game copyright

#pragma once

#include "CoreMinimal.h"

namespace TPS
{
class TPS_API Battery
{
public:
    Battery() = default;
    Battery(float PercentIn);

    //! \todo Add parameter for charging
    void Charge();
    //! \todo Add parameter for uncharging
    void UnCharge();

    float GetPercent() const;
    FColor GetColor() const;
    FString ToString() const;

    bool operator>=(const Battery& rhs) const { return GetPercent() >= rhs.GetPercent(); }
    bool operator==(const Battery& rhs) const { return FMath::IsNearlyEqual(GetPercent(), rhs.GetPercent()); }

private:
    float Percent{1.0f};
    void SetPercent(float PercentIn);
};
}  // namespace TPS
