// My game copyright

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

class UButton;

/**
 *
 */
UCLASS()
class TPS_API UPauseMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* CloseMenuButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnCloseMenu();
};
