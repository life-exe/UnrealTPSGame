// My game copyright

#pragma once

#include "CoreMinimal.h"
#include "TPSGameSetting.generated.h"

struct FSettingOption
{
    FText Name;   ///< Name of the option
    int32 Value;  ///< Value of the option
};

UCLASS()
class UTPSGameSetting : public UObject
{
    GENERATED_BODY()

public:
    void SetName(const FText& Name);

    //! Sets an array of options
    void SetOptions(const TArray<FSettingOption>& Options);

    FSettingOption GetCurrentOption() const;
    FText GetName() const;
    //! \attention Func might be set before object using
    void AddGetter(TFunction<int32()> Func);
    //! \attention Func might be set before object using
    void AddSetter(TFunction<void(int32)> Func);

    void ApplyNextOption();
    void ApplyPrevOption();

private:
    FText Name;
    TArray<FSettingOption> Options;
    TFunction<int32()> Getter;
    TFunction<void(int32)> Setter;

    int32 GetCurrentValue() const;
    void SetCurrentValue(int32 Value);
    FORCENOINLINE int32 GetCurrentIndex() const;
};