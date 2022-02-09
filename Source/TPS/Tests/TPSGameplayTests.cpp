// My game copyright

#if WITH_AUTOMATION_TESTS

#include "TPS/Tests/TPSGameplayTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TPS/Tests/TestUtils.h"
#include "Kismet/GameplayStatics.h"
#include "TPS/Items/TPSInventoryItem.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryItemCanBeTakenOnJump, "TPSGame.Gameplay.InventoryItemCanBeTakenOnJump",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryItemCantBeTakenOnJumpIfTooHigh, "TPSGame.Gameplay.InventoryItemCantBeTakenOnJumpIfTooHigh",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAllItemsAreTakenOnMovement, "TPSGame.Gameplay.AllItemsAreTakenOnMovement",
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

#endif
