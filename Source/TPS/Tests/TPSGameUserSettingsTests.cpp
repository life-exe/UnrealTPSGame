// My game copyright

#if WITH_AUTOMATION_TESTS

#include "TPS/Tests/TPSGameUserSettingsTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TPS/Tests/TestUtils.h"
#include "Settings/TPSGameUserSettings.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSettingsShouldExist, "TPSGame.GameUserSettings.SettingsShouldExist",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

using namespace TPS::Test;

bool FSettingsShouldExist::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap");

    TestTrueExpr(UTPSGameUserSettings::Get() != nullptr);
    TestTrueExpr(UTPSGameUserSettings::Get()->GetVideoSettings().Num() == 7);

    return true;
}

#endif
