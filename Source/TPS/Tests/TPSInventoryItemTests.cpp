// My game copyright

#if WITH_AUTOMATION_TESTS

#include "TPS/Tests/TPSInventoryItemTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TPS/Items/TPSInventoryItem.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TPS/Tests/TestUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCppActorCantBeCreated, "TPSGame.Items.Inventory.CppActorCantBeCreated",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBlueprintShouldBeSetupCorrectly, "TPSGame.Items.Inventory.BlueprintShouldBeSetupCorrectly",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

namespace
{
constexpr char* InventoryItemBPName = "Blueprint'/Game/Inventory/BP_TPSInventoryItem.BP_TPSInventoryItem'";
}  // namespace

using namespace TPS::Test;

bool FCppActorCantBeCreated::RunTest(const FString& Parameters)
{
    const FString ExpectedWarnMsg =
        FString::Printf(TEXT("SpawnActor failed because class %s is abstract"), *ATPSInventoryItem::StaticClass()->GetName());
    AddExpectedError(ExpectedWarnMsg, EAutomationExpectedErrorFlags::Exact);

    LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    const FTransform InitialTransform{FVector{1000.0f}};
    const ATPSInventoryItem* InvItem = World->SpawnActor<ATPSInventoryItem>(ATPSInventoryItem::StaticClass(), InitialTransform);
    if (!TestNull("Inventory item exists", InvItem)) return false;

    return true;
}

bool FBlueprintShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    const FTransform InitialTransform{FVector{1000.0f}};
    const ATPSInventoryItem* InvItem = CreateBlueprint<ATPSInventoryItem>(World, InventoryItemBPName, InitialTransform);
    if (!TestNotNull("Inventory item exists", InvItem)) return false;

    const auto CollisionComp = InvItem->FindComponentByClass<USphereComponent>();
    if (!TestNotNull("Sphere component exists", CollisionComp)) return false;

    TestTrueExpr(CollisionComp->GetUnscaledSphereRadius() >= 30.0f);
    TestTrueExpr(CollisionComp->GetCollisionEnabled() == ECollisionEnabled::QueryOnly);
    TestTrueExpr(CollisionComp->GetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic) == ECollisionResponse::ECR_Overlap);
    TestTrueExpr(CollisionComp->GetGenerateOverlapEvents());
    TestTrueExpr(InvItem->GetRootComponent() == CollisionComp);

    ENUM_LOOP_START(ECollisionChannel, EElem)
    if (EElem != ECollisionChannel::ECC_OverlapAll_Deprecated)
    {
        TestTrueExpr(CollisionComp->GetCollisionResponseToChannel(EElem) == ECollisionResponse::ECR_Overlap);
    }
    ENUM_LOOP_END

    const auto TextRenderComp = InvItem->FindComponentByClass<UTextRenderComponent>();
    if (!TestNotNull("Text render component exists", TextRenderComp)) return false;

    const auto StaticMeshComp = InvItem->FindComponentByClass<UStaticMeshComponent>();
    if (!TestNotNull("Static mesh component exists", StaticMeshComp)) return false;

    TestTrueExpr(StaticMeshComp->GetCollisionEnabled() == ECollisionEnabled::NoCollision);

    return true;
}

#endif
