// My game copyright

#include "UI/PauseMenu/PauseMenuWidget.h"
#include "Components/Button.h"
#include "TPSPlayerController.h"

void UPauseMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(CloseMenuButton);
    CloseMenuButton->OnClicked.AddDynamic(this, &ThisClass::OnCloseMenu);
}

void UPauseMenuWidget::OnCloseMenu()
{
    if (!GetWorld()) return;

    if (auto* PC = Cast<ATPSPlayerController>(GetWorld()->GetFirstPlayerController()))
    {
        PC->ToggleGamePause();
    }
}
