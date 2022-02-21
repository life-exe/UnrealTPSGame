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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAllItemsAreTakenOnRecordingMovement, "TPSGame.Gameplay.AllItemsAreTakenOnRecordingMovement",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace TPS::Test;

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FJumpLatentCommand, ACharacter*, Character);
bool FJumpLatentCommand::Update()
{
    if (!Character) return true;

    const int32 ActionIndex = GetActionBindingIndexByName(Character->InputComponent, "Jump", EInputEvent::IE_Pressed);
    if (ActionIndex != INDEX_NONE)
    {
        const auto JumpActionBind = Character->InputComponent->GetActionBinding(ActionIndex);
        JumpActionBind.ActionDelegate.Execute(EKeys::SpaceBar);
    }
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
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character));
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
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character));
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
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character));
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

bool FAllItemsAreTakenOnRecordingMovement::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Character exists", Character)) return false;

    TArray<AActor*> InventoryItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
    TestTrueExpr(InventoryItems.Num() == 5);

    const FString FileName = GetTestDataDir().Append("CharacterTestInput.json");
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

#endif
