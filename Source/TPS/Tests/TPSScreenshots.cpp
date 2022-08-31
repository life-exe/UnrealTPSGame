// My game copyright

#if WITH_AUTOMATION_TESTS

#include "TPS/Tests/TPSScreenshots.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TPS/Tests/TestUtils.h"
#include "Camera/CameraActor.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRenderingShouldBeCorrect, "TPSGame.Screenshots.RenderingShouldBeCorrect",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMainPlayerHUDShouldBeRendered, "TPSGame.Screenshots.MainPlayerHUDShouldBeRendered",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthWidgetShouldBeRenderedCorrectlyAfterDamage,
    "TPSGame.Screenshots.HealthWidgetShouldBeRenderedCorrectlyAfterDamage",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSettingsWidgetBeRenderedWhenGamePaused, "TPSGame.Screenshots.SettingsWidgetBeRenderedWhenGamePaused",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace TPS::Test;

bool FRenderingShouldBeCorrect::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap");
    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    const FTransform Transform{FVector{420.0f, -770.0f, 310.0f}};
    ACameraActor* Camera = World->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), Transform);
    if (!TestNotNull("Camera exists", Camera)) return false;

    APlayerController* PC = World->GetFirstPlayerController();
    if (!TestNotNull("Player controller exists", PC)) return false;

    PC->SetViewTarget(Camera);

    ADD_LATENT_AUTOMATION_COMMAND(FTakeGameScreenshotLatentCommand("rendering_check_screenshot"));
    return true;
}

bool FMainPlayerHUDShouldBeRendered::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap");
    ADD_LATENT_AUTOMATION_COMMAND(FTakeUIScreenshotLatentCommand("main_player_hud_screenshot"));
    return true;
}

bool FHealthWidgetShouldBeRenderedCorrectlyAfterDamage::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    APlayerController* PC = World->GetFirstPlayerController();
    if (!TestNotNull("Player controller exists", PC)) return false;
    if (!TestNotNull("Pawn exists", PC->GetPawn())) return false;

    const float DamageAmount = 40.0f;
    PC->GetPawn()->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);

    ADD_LATENT_AUTOMATION_COMMAND(FTakeUIScreenshotLatentCommand("health_widget_screenshot"));
    return true;
}

bool FSettingsWidgetBeRenderedWhenGamePaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    APlayerController* PC = World->GetFirstPlayerController();
    if (!TestNotNull("Player controller exists", PC)) return false;

    PausePressed(PC->InputComponent);

    ADD_LATENT_AUTOMATION_COMMAND(FTakeUIScreenshotLatentCommand("settings_widget_screenshot"));
    return true;
}

#endif
