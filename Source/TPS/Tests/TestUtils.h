#pragma once

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Engine/Blueprint.h"
#include "Tests/AutomationCommon.h"

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
    for (int32 Index = 0; Index < StaticEnum<TYPE>()->NumEnums() - 1; ++Index) \
    {                                                                          \
        const auto EnumElem = static_cast<TYPE>(Index);
#define ENUM_LOOP_END }

template <typename T>
T* CreateBlueprint(UWorld* World, const FString& Name, const FTransform& Transform = FTransform::Identity)
{
    const UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *Name);
    return (World && Blueprint) ? World->SpawnActor<T>(Blueprint->GeneratedClass, Transform) : nullptr;
}

template <typename T>
T* CreateBlueprintDeferred(UWorld* World, const FString& Name, const FTransform& Transform = FTransform::Identity)
{
    const UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *Name);
    return (World && Blueprint) ? World->SpawnActorDeferred<T>(Blueprint->GeneratedClass, Transform) : nullptr;
}

class LevelScope
{
public:
    LevelScope(const FString& MapName) { AutomationOpenMap(MapName); }
    ~LevelScope() { ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand); }
};

UWorld* GetTestGameWorld();

void CallFuncByNameWithParams(UObject* Object, const FString& FuncName, const TArray<FString>& Params);

class FTPSUntilLatentCommand : public IAutomationLatentCommand
{
public:
    FTPSUntilLatentCommand(TFunction<void()> InCallback, TFunction<void()> InTimeoutCallback, float InTimeout = 5.0f);
    virtual bool Update() override;

private:
    TFunction<void()> Callback;
    TFunction<void()> TimeoutCallback;
    float Timeout;
};

int32 GetActionBindingIndexByName(UInputComponent* InputComp, const FString& ActionName, EInputEvent InputEvent);
int32 GetAxisBindingIndexByName(UInputComponent* InputComp, const FString& AxisName);

}  // namespace Test
}  // namespace TPS

#endif
