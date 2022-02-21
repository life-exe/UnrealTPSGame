// My game copyright

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPS/Tests/Utils/InputRecordingUtils.h"
#include "TPSInputRecordingComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TPS_API UTPSInputRecordingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTPSInputRecordingComponent();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UPROPERTY(EditAnywhere)
    FString FileName{"CharacterTestInput"};

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    FInputData InputData;

    FBindingsData MakeBindingsData() const;
    FString GenerateFileName() const;
};
