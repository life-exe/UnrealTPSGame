// My game copyright

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TPS/Tests/TestUtils.h"
#include "Weapon/TPSTurret.h"

BEGIN_DEFINE_SPEC(FTurret, "TPSGame.Turret",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)
UWorld* World;
ATPSTurret* Turret;
const FTransform InitialTransform{FVector{0.0f, 286.0f, 110.0f}};
END_DEFINE_SPEC(FTurret)

using namespace TPS::Test;

namespace
{
constexpr char* MapName = "/Game/Tests/EmptyTestLevel";
constexpr char* TurretBPName = "Blueprint'/Game/Weapon/BP_TPSTurret.BP_TPSTurret'";
constexpr char* TurretBPTestName = "Blueprint'/Game/Tests/BP_Test_TPSTurret.BP_Test_TPSTurret'";

}  // namespace

void FTurret::Define()
{
    Describe("Creational",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    AutomationOpenMap(MapName);
                    World = GetTestGameWorld();
                    TestNotNull("World exists", World);
                });

            It("Cpp instance can't be created",
                [this]()
                {
                    const FString ExpectedWarnMsg =
                        FString::Printf(TEXT("SpawnActor failed because class %s is abstract"), *ATPSTurret::StaticClass()->GetName());
                    AddExpectedError(ExpectedWarnMsg, EAutomationExpectedErrorFlags::Exact);

                    Turret = World->SpawnActor<ATPSTurret>(ATPSTurret::StaticClass(), InitialTransform);
                    TestNull("Turret doesn't exist", Turret);
                });

            It("Blueprint instance can be created",
                [this]()
                {
                    Turret = CreateBlueprint<ATPSTurret>(World, TurretBPName, InitialTransform);
                    TestNotNull("Turret exists", Turret);
                });

            AfterEach([this]() { SpecCloseLevel(World); });
        });

    Describe("Defaults",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    AutomationOpenMap(MapName);
                    World = GetTestGameWorld();
                    TestNotNull("World exists", World);

                    Turret = CreateBlueprint<ATPSTurret>(World, TurretBPTestName, InitialTransform);
                    TestNotNull("Turret exists", Turret);
                });

            const TArray<TTuple<int32, float>> TestData{{45, 2.0f}, {15, 3.0f}, {5, 5.0f}};
            for (const auto& Data : TestData)
            {
                const auto TestName = FString::Printf(TEXT("Ammo: %i and freq: %.0f should be set up correctly"), Data.Key, Data.Value);
                It(TestName,
                    [this, Data]()
                    {
                        const auto [Ammo, Freq] = Data;
                        CallFuncByNameWithParams(Turret, "SetTurretData", {FString::FromInt(Ammo), FString::SanitizeFloat(Freq)});

                        const int32 AmmoCount = GetPropertyValueByName<ATPSTurret, int32>(Turret, "AmmoCount");
                        TestTrueExpr(AmmoCount == Ammo);

                        const float FireFrequency = GetPropertyValueByName<ATPSTurret, float>(Turret, "FireFrequency");
                        TestTrueExpr(FireFrequency == Freq);
                    });
            }

            AfterEach([this]() { SpecCloseLevel(World); });
        });

    Describe("Ammo",
        [this]()
        {
            const int32 InitialAmmoCount = 4;
            const float FireFreq = 1.5f;
            LatentBeforeEach(
                [this, InitialAmmoCount, FireFreq](const FDoneDelegate& TestDone)
                {
                    AutomationOpenMap(MapName);
                    World = GetTestGameWorld();
                    TestNotNull("World exists", World);

                    Turret = CreateBlueprint<ATPSTurret>(World, TurretBPTestName, InitialTransform);
                    TestNotNull("Turret exists", Turret);
                    CallFuncByNameWithParams(
                        Turret, "SetTurretData", {FString::FromInt(InitialAmmoCount), FString::SanitizeFloat(FireFreq)});
                    TestDone.Execute();
                });

            LatentIt(FString::Printf(TEXT("Should be empty after %i sec"), FMath::RoundToInt(InitialAmmoCount * FireFreq)),
                EAsyncExecution::ThreadPool,
                [this, InitialAmmoCount, FireFreq](const FDoneDelegate& TestDone)
                {
                    AsyncTask(ENamedThreads::GameThread,
                        [&]()
                        {
                            const int32 AmmoCount = GetPropertyValueByName<ATPSTurret, int32>(Turret, "AmmoCount");
                            TestTrueExpr(AmmoCount == InitialAmmoCount);

                            const FTimerHandle FireTimerHandle =
                                GetPropertyValueByName<ATPSTurret, FTimerHandle>(Turret, "FireTimerHandle");
                            TestTrueExpr(World->GetTimerManager().IsTimerActive(FireTimerHandle));
                        });

                    const float SynchDelta = 0.5f;
                    FPlatformProcess::Sleep(InitialAmmoCount * FireFreq + SynchDelta);

                    AsyncTask(ENamedThreads::GameThread,
                        [&]()
                        {
                            const int32 AmmoCount = GetPropertyValueByName<ATPSTurret, int32>(Turret, "AmmoCount");
                            TestTrueExpr(AmmoCount == 0);
                            const FTimerHandle FireTimerHandle =
                                GetPropertyValueByName<ATPSTurret, FTimerHandle>(Turret, "FireTimerHandle");
                            TestTrueExpr(!World->GetTimerManager().IsTimerActive(FireTimerHandle));
                        });

                    TestDone.Execute();
                });

            LatentAfterEach(
                [this](const FDoneDelegate& TestDone)
                {
                    SpecCloseLevel(World);
                    TestDone.Execute();
                });
        });
}

#endif
