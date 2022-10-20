// My game copyright

#if WITH_AUTOMATION_TESTS

#include "TPS/Tests/TPSGameplayTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TPS/Tests/TestUtils.h"
#include "Kismet/GameplayStatics.h"
#include "TPS/Items/TPSInventoryItem.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "Tests/Utils/JsonUtils.h"
#include "TPS/Tests/Utils/InputRecordingUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryItemCanBeTakenOnJump, "TPSGame.Gameplay.InventoryItemCanBeTakenOnJump",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryItemCantBeTakenOnJumpIfTooHigh, "TPSGame.Gameplay.InventoryItemCantBeTakenOnJumpIfTooHigh",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAllItemsAreTakenOnMovement, "TPSGame.Gameplay.AllItemsAreTakenOnMovement",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_COMPLEX_AUTOMATION_TEST(FAllItemsAreTakenOnRecordingMovement, "TPSGame.Gameplay.AllItemsAreTakenOnRecordingMovement",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_COMPLEX_AUTOMATION_TEST(FMapsShouldBeLoaded, "TPSGame.Gameplay.MapsShouldBeLoaded",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

using namespace TPS::Test;

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FJumpLatentCommand, UInputComponent*, InputComponent);
bool FJumpLatentCommand::Update()
{
    JumpPressed(InputComponent);
    return true;
}

bool FInventoryItemCantBeTakenOnJumpIfTooHigh::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/InventoryTestLevel2");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Character exists", Character)) return false;

    TArray<AActor*> InventoryItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
    if (!TestEqual("Only one item exists", InventoryItems.Num(), 1)) return false;

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character->InputComponent));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            TArray<AActor*> InventoryItems;
            UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
            TestTrueExpr(InventoryItems.Num() == 1);
        },
        2.0f));

    return true;
}

bool FInventoryItemCanBeTakenOnJump::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/InventoryTestLevel1");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Character exists", Character)) return false;

    TArray<AActor*> InventoryItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
    if (!TestEqual("Only one item exists", InventoryItems.Num(), 1)) return false;

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character->InputComponent));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            TArray<AActor*> InventoryItems;
            UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
            TestTrueExpr(InventoryItems.Num() == 0);
        },
        2.0f));

    return true;
}

bool FAllItemsAreTakenOnMovement::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/InventoryTestLevel3");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Character exists", Character)) return false;

    TArray<AActor*> InventoryItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
    TestTrueExpr(InventoryItems.Num() == 7);

    const int32 MoveForwardIndex = GetAxisBindingIndexByName(Character->InputComponent, "MoveForward");
    TestTrueExpr(MoveForwardIndex != INDEX_NONE);

    const int32 MoveRightIndex = GetAxisBindingIndexByName(Character->InputComponent, "MoveRight");
    TestTrueExpr(MoveRightIndex != INDEX_NONE);

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FTPSUntilLatentCommand(                                                 //
        [=]() { Character->InputComponent->AxisBindings[MoveForwardIndex].AxisDelegate.Execute(1.0f); },  //
        []() {},                                                                                          //
        3.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character->InputComponent));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FTPSUntilLatentCommand(                                               //
        [=]() { Character->InputComponent->AxisBindings[MoveRightIndex].AxisDelegate.Execute(1.0f); },  //
        []() {},                                                                                        //
        2.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [=]()
        {
            TArray<AActor*> InventoryItems;
            UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
            TestTrueExpr(InventoryItems.Num() == 0);
            return true;
        }));

    return true;
}

class FSimulateMovementLatentCommand : public IAutomationLatentCommand
{
public:
    FSimulateMovementLatentCommand(UWorld* InWorld, UInputComponent* InInputComponent, const TArray<FBindingsData>& InBindingsData)
        : World(InWorld), InputComponent(InInputComponent), BindingsData(InBindingsData)
    {
    }

    virtual bool Update() override
    {
        if (!World || !InputComponent) return true;

        if (WorldStartTime == 0.0f)
        {
            WorldStartTime = World->TimeSeconds;
        }

        while (World->TimeSeconds - WorldStartTime >= BindingsData[Index].WorldTime)
        {
            for (int32 i = 0; i < InputComponent->AxisBindings.Num(); ++i)
            {
                const float AxisValue = BindingsData[Index].AxisValues[i].Value;
                InputComponent->AxisBindings[i].AxisDelegate.Execute(AxisValue);
            }

            if (Index > 0)
            {
                for (int32 i = 0; i < InputComponent->GetNumActionBindings(); ++i)
                {
                    const auto ActionValue = BindingsData[Index].ActionValues[i];
                    const auto PrevActionValue = BindingsData[Index - 1].ActionValues[i];
                    if (ActionValue.State && ActionValue.State != PrevActionValue.State)
                    {
                        InputComponent->GetActionBinding(i).ActionDelegate.Execute(ActionValue.Key);
                    }
                }
            }

            if (++Index >= BindingsData.Num()) return true;
        }

        return false;
    }

private:
    const UWorld* World;
    const UInputComponent* InputComponent;
    const TArray<FBindingsData> BindingsData;
    int32 Index{0};
    float WorldStartTime{0.0f};
};

void FAllItemsAreTakenOnRecordingMovement::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
    struct FTestData
    {
        FString TestName;
        FString MapPath;
        FString JsonName;
    };

    const TArray<FTestData> TestData =  //
        {
            {"MainMap", "/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap", "CharacterTestInputMainMap.json"},  //
            {"CustomMap", "/Game/ThirdPersonCPP/Maps/CustomMap", "CharacterTestInputCustomMap.json"},          //
        };

    for (const auto OneTestData : TestData)
    {
        OutBeautifiedNames.Add(OneTestData.TestName);
        OutTestCommands.Add(FString::Printf(TEXT("%s,%s"), *OneTestData.MapPath, *OneTestData.JsonName));
    }
}

bool FAllItemsAreTakenOnRecordingMovement::RunTest(const FString& Parameters)
{
    TArray<FString> ParsedParams;
    Parameters.ParseIntoArray(ParsedParams, TEXT(","));
    if (!TestTrue("Map name and JSON params should exist", ParsedParams.Num() == 2)) return false;

    const auto Level = LevelScope(ParsedParams[0]);

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Character exists", Character)) return false;

    TArray<AActor*> InventoryItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
    TestTrueExpr(InventoryItems.Num() == 5);

    const FString FileName = GetTestDataDir().Append(ParsedParams[1]);
    FInputData InputData;
    if (!JsonUtils::ReadInputData(FileName, InputData)) return false;
    if (!TestTrue("Input data is not empty", InputData.Bindings.Num() > 0)) return false;

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
    if (!TestNotNull("Player controller exists", PlayerController)) return false;

    Character->SetActorTransform(InputData.InitialTransform);
    PlayerController->SetControlRotation(InputData.InitialTransform.Rotator());

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FSimulateMovementLatentCommand(World, Character->InputComponent, InputData.Bindings));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [=]()
        {
            TArray<AActor*> InventoryItems;
            UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
            TestTrueExpr(InventoryItems.Num() == 0);
            return true;
        }));

    return true;
}

void FMapsShouldBeLoaded::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
    /*
    * =================================================
    * tuple usage example
    * =================================================

        const TTuple<FString, int32, bool, float> MyTuple1 = {"unreal", 5, true, 100.0f};
        const auto MyTuple2 = MakeTuple(FString("unity"), 7, false, 50.0f);

        auto str = MyTuple1.Get<0>();
        auto intgr = MyTuple1.Get<1>();
        auto bln = MyTuple1.Get<2>();
        auto flt = MyTuple1.Get<3>();
        UE_LOG(LogTemp, Display, TEXT("Tuple1: %s, %i, %i, %f"), *str, intgr, bln, flt);

        Tie(str, intgr, bln, flt) = MyTuple2;
        UE_LOG(LogTemp, Display, TEXT("Tuple2: %s, %i, %i, %f"), *str, intgr, bln, flt);

    * =================================================
    */

    const TArray<TTuple<FString, FString>> TestData =  //
        {
            {"MainMap", "/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap"},
            {"CustomMap", "/Game/ThirdPersonCPP/Maps/CustomMap"},
        };

    for (const auto OneTestData : TestData)
    {
        OutBeautifiedNames.Add(OneTestData.Key);
        OutTestCommands.Add(OneTestData.Value);
    }
}

bool FMapsShouldBeLoaded::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope(Parameters);
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.0f));
    return true;
}

#endif
