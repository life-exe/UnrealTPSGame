// My game copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TPSPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamePauseDelegate, bool, IsPaused);

/**
 *
 */
UCLASS()
class TPS_API ATPSPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    void ToggleGamePause();

    UPROPERTY(BlueprintAssignable)
    FOnGamePauseDelegate OnGamePause;

protected:
    virtual void SetupInputComponent() override;

private:
    bool bInPause{false};
};
