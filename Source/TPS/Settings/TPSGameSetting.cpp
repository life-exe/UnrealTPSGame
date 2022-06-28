// My game copyright

#include "Settings/TPSGameSetting.h"

DEFINE_LOG_CATEGORY_STATIC(LogTPSGameSetting, All, All);

void UTPSGameSetting::SetName(const FText& InName)
{
    Name = InName;
}

void UTPSGameSetting::SetOptions(const TArray<FSettingOption>& InOptions)
{
    Options = InOptions;
}

FSettingOption UTPSGameSetting::GetCurrentOption() const
{
    const int32 CurrentValue = GetCurrentValue();
    const auto Option = Options.FindByPredicate([&](const auto& Opt) { return CurrentValue == Opt.Value; });
    if (!Option)
    {
        UE_LOG(LogTPSGameSetting, Error, TEXT("Option doesn't exist"));
        return FSettingOption{};
    }
    return *Option;
}

FText UTPSGameSetting::GetName() const
{
    return Name;
}

void UTPSGameSetting::AddGetter(TFunction<int32()> Func)
{
    Getter = Func;
}

void UTPSGameSetting::AddSetter(TFunction<void(int32)> Func)
{
    Setter = Func;
}

void UTPSGameSetting::ApplyNextOption()
{
    const int32 CurrentIndex = GetCurrentIndex();
    if (CurrentIndex == INDEX_NONE) return;

    const int32 NextIndex = (CurrentIndex + 1) % Options.Num();
    SetCurrentValue(Options[NextIndex].Value);
}

void UTPSGameSetting::ApplyPrevOption()
{
    const int32 CurrentIndex = GetCurrentIndex();
    if (CurrentIndex == INDEX_NONE) return;

    const int32 PrevIndex = CurrentIndex == 0 ? Options.Num() - 1 : CurrentIndex - 1;
    SetCurrentValue(Options[PrevIndex].Value);
}

int32 UTPSGameSetting::GetCurrentValue() const
{
    if (!Getter)
    {
        UE_LOG(LogTPSGameSetting, Error, TEXT("Getter func is not set for %s"), *Name.ToString());
        return INDEX_NONE;
    }
    return Getter();
}

void UTPSGameSetting::SetCurrentValue(int32 Value)
{
    if (!Setter)
    {
        UE_LOG(LogTPSGameSetting, Error, TEXT("Setter func is not set for %s"), *Name.ToString());
        return;
    }
    Setter(Value);
}

int32 UTPSGameSetting::GetCurrentIndex() const
{
    const int32 CurrentValue = GetCurrentValue();
    return Options.IndexOfByPredicate([&](const auto& Opt) { return CurrentValue == Opt.Value; });
}
