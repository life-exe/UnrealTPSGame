// My game copyright

#if WITH_AUTOMATION_TESTS

#include "TPS/Tests/TPSPlayerControllerTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "GameFramework/PlayerController.h"
#include "TPS/Tests/TestUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameCanBePaused, "TPSGame.PlayerController.GameCanBePaused",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameCanBeUnPaused, "TPSGame.PlayerController.GameCanBeUnPaused",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

using namespace TPS::Test;

bool FGameCanBePaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap");
    APlayerController* PC = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PC != nullptr);

    TestTrueExpr(!PC->IsPaused());
    TestTrueExpr(!PC->bShowMouseCursor);
    PausePressed(PC->InputComponent);
    TestTrueExpr(PC->IsPaused());
    TestTrueExpr(PC->bShowMouseCursor);

    return true;
}

bool FGameCanBeUnPaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap");
    APlayerController* PC = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PC != nullptr);

    TestTrueExpr(!PC->IsPaused());
    TestTrueExpr(!PC->bShowMouseCursor);
    PausePressed(PC->InputComponent);
    PausePressed(PC->InputComponent);
    TestTrueExpr(!PC->IsPaused());
    TestTrueExpr(!PC->bShowMouseCursor);

    return true;
}

#endif
