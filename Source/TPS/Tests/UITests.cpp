// My game copyright

#if WITH_AUTOMATION_TESTS

#include "TPS/Tests/UITests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TPS/Tests/TestUtils.h"
#include "UI/PauseMenu/PauseMenuWidget.h"
#include "Blueprint/UserWidget.h"
#include "UI/PauseMenu/VideoSettingsWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Settings/TPSGameUserSettings.h"
#include "UI/PauseMenu/SettingOptionWidget.h"
#include "Settings/TPSGameSetting.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPauseMenuShouldBeVisibleOnGamePaused, "TPSGame.UI.PauseMenuShouldBeVisibleOnGamePaused",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPauseMenuShouldBeCollapsedOnGameUnPaused, "TPSGame.UI.PauseMenuShouldBeCollapsedOnGameUnPaused",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAllVideoSettingsExist, "TPSGame.UI.AllVideoSettingsExist",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSettingCanBeApplied, "TPSGame.UI.SettingCanBeApplied",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAutoBenchmarkShouldWork, "TPSGame.UI.AutoBenchmarkShouldWork",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::StressFilter | EAutomationTestFlags::MediumPriority);

using namespace TPS::Test;

namespace
{
void NextSettingClick(int32 SettingIndex)
{
    const auto* VideoSettingsWidget = FindWidgetByClass<UVideoSettingsWidget>();
    const auto* VerticalBox = Cast<UVerticalBox>(FindWidgetByName(VideoSettingsWidget, "VideoSettingsContainer"));
    const auto* SettingOptionWidget = Cast<USettingOptionWidget>(VerticalBox->GetChildAt(SettingIndex));
    const auto* NextSettingButton = Cast<UButton>(FindWidgetByName(SettingOptionWidget, "NextSettingButton"));
    NextSettingButton->OnClicked.Broadcast();
}

void DoBenchmarkClick()
{
    const auto* VideoSettingsWidget = FindWidgetByClass<UVideoSettingsWidget>();
    const auto* BenchmarkButton = Cast<UButton>(FindWidgetByName(VideoSettingsWidget, "RunBenchmarkButton"));
    BenchmarkButton->OnClicked.Broadcast();
}
}  // namespace

bool FPauseMenuShouldBeVisibleOnGamePaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap");
    APlayerController* PC = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PC != nullptr);

    const UPauseMenuWidget* PauseMenuWidget = FindWidgetByClass<UPauseMenuWidget>();
    TestTrueExpr(PauseMenuWidget != nullptr);
    TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Collapsed);
    PausePressed(PC->InputComponent);
    TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Visible);

    return true;
}

bool FPauseMenuShouldBeCollapsedOnGameUnPaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap");
    APlayerController* PC = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PC != nullptr);

    const UPauseMenuWidget* PauseMenuWidget = FindWidgetByClass<UPauseMenuWidget>();
    TestTrueExpr(PauseMenuWidget != nullptr);
    TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Collapsed);
    PausePressed(PC->InputComponent);
    PausePressed(PC->InputComponent);
    TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Collapsed);

    return true;
}

bool FAllVideoSettingsExist::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap");

    const UVideoSettingsWidget* VideoSettingsWidget = FindWidgetByClass<UVideoSettingsWidget>();
    TestTrueExpr(VideoSettingsWidget != nullptr);

    const UButton* BenchmarkButton = Cast<UButton>(FindWidgetByName(VideoSettingsWidget, "RunBenchmarkButton"));
    TestTrueExpr(BenchmarkButton != nullptr);

    const UVerticalBox* VerticalBox = Cast<UVerticalBox>(FindWidgetByName(VideoSettingsWidget, "VideoSettingsContainer"));
    TestTrueExpr(VerticalBox != nullptr);

    const auto& VideoSettings = UTPSGameUserSettings::Get()->GetVideoSettings();
    TestTrueExpr(VerticalBox->GetChildrenCount() == VideoSettings.Num());
    TestTrueExpr(VerticalBox->GetChildrenCount() > 0);

    return true;
}

bool FSettingCanBeApplied::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap");
    APlayerController* PC = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PC != nullptr);
    PausePressed(PC->InputComponent);

    const auto& VideoSettings = UTPSGameUserSettings::Get()->GetVideoSettings();

    const int32 GIQualityBefore = UTPSGameUserSettings::Get()->GetGlobalIlluminationQuality();
    TestTrueExpr(GIQualityBefore == VideoSettings[2]->GetCurrentOption().Value);

    NextSettingClick(2);

    const int32 GIQualityAfter = UTPSGameUserSettings::Get()->GetGlobalIlluminationQuality();
    TestTrueExpr(GIQualityBefore != GIQualityAfter);
    TestTrueExpr(GIQualityAfter == VideoSettings[2]->GetCurrentOption().Value);

    UTPSGameUserSettings::Get()->SetGlobalIlluminationQuality(GIQualityBefore);

    return true;
}

bool FAutoBenchmarkShouldWork::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap");
    APlayerController* PC = GetTestGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PC != nullptr);
    PausePressed(PC->InputComponent);

    DoBenchmarkClick();

    TArray<int32> SavedSettingValues;
    const auto& VideoSettings = UTPSGameUserSettings::Get()->GetVideoSettings();
    for (const auto& Setting : VideoSettings)
    {
        SavedSettingValues.Add(Setting->GetCurrentOption().Value);
    }

    // random clicks

    for (int32 i = 0; i < VideoSettings.Num(); ++i)
    {
        NextSettingClick(i);
    }

    for (int32 i = 0; i < VideoSettings.Num(); ++i)
    {
        TestTrueExpr(SavedSettingValues[i] != VideoSettings[i]->GetCurrentOption().Value);
    }

    DoBenchmarkClick();

    for (int32 i = 0; i < VideoSettings.Num(); ++i)
    {
        TestTrueExpr(SavedSettingValues[i] == VideoSettings[i]->GetCurrentOption().Value);
    }

    return true;
}

#endif
