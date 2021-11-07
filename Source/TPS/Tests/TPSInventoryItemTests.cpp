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
#include "Kismet/GameplayStatics.h"
#include "TPS/TPSCharacter.h"
#include "TPS/Components/TPSInventoryComponent.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCppActorCantBeCreated, "TPSGame.Items.Inventory.CppActorCantBeCreated",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBlueprintShouldBeSetupCorrectly, "TPSGame.Items.Inventory.BlueprintShouldBeSetupCorrectly",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryDataShouldBeSetupCorrectly, "TPSGame.Items.Inventory.InventoryDataShouldBeSetupCorrectly",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryCanBeTaken, "TPSGame.Items.Inventory.InventoryCanBeTaken",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

namespace
{
constexpr char* InventoryItemBPName = "Blueprint'/Game/Inventory/BP_TPSInventoryItem.BP_TPSInventoryItem'";
constexpr char* InventoryItemBPTestName = "Blueprint'/Game/Tests/BP_Test_TPSInventoryItem.BP_Test_TPSInventoryItem'";

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

bool FInventoryDataShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    const FTransform InitialTransform{FVector{1000.0f}};
    ATPSInventoryItem* InvItem = CreateBlueprint<ATPSInventoryItem>(World, InventoryItemBPTestName, InitialTransform);
    if (!TestNotNull("Inventory item exists", InvItem)) return false;

    /*
    * Example of using CallFuncByNameWithParams
    *
    FVector Vector{120.0f, 130.0f, 140.0f};
    CallFuncByNameWithParams(InvItem, "SetTestData",
        {
            FString::FromInt(123),                                                    //
            FString::SanitizeFloat(345.45f),                                          //
            FString("\"My cool string\""),                                            //
            FString("Yes"),                                                           //
            FString::Printf(TEXT("(X=%f,Y=%f,Z=%f)"), Vector.X, Vector.Y, Vector.Z),  //
        });
    */

    const FInventoryData InvData{EInventoryItemType::CYLINDER, 13};
    const FLinearColor Color = FLinearColor::Yellow;
    CallFuncByNameWithParams(InvItem, "SetInventoryData", {InvData.ToString(), Color.ToString()});

    const auto TextRenderComp = InvItem->FindComponentByClass<UTextRenderComponent>();
    if (!TestNotNull("Text render component exists", TextRenderComp)) return false;

    TestTrueExpr(TextRenderComp->Text.ToString().Equals(FString::FromInt(InvData.Score)));
    TestTrueExpr(TextRenderComp->TextRenderColor == Color.ToFColor(true));

    const auto StaticMeshComp = InvItem->FindComponentByClass<UStaticMeshComponent>();
    if (!TestNotNull("Static mesh component exists", StaticMeshComp)) return false;

    const auto Material = StaticMeshComp->GetMaterial(0);
    if (!TestNotNull("Material exists", Material)) return false;

    FLinearColor MaterialColor;
    Material->GetVectorParameterValue(FHashedMaterialParameterInfo{"Color"}, MaterialColor);
    TestTrueExpr(MaterialColor == Color);

    return true;
}

bool FInventoryCanBeTaken::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/EmptyTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    const FTransform InitialTransform{FVector{1000.0f}};
    ATPSInventoryItem* InvItem = CreateBlueprint<ATPSInventoryItem>(World, InventoryItemBPTestName, InitialTransform);
    if (!TestNotNull("Inventory item exists", InvItem)) return false;

    const FInventoryData InvData{EInventoryItemType::CYLINDER, 13};
    const FLinearColor Color = FLinearColor::Yellow;
    CallFuncByNameWithParams(InvItem, "SetInventoryData", {InvData.ToString(), Color.ToString()});

    TArray<AActor*> Pawns;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSCharacter::StaticClass(), Pawns);
    if (!TestTrueExpr(Pawns.Num() == 1)) return false;

    const auto Character = Cast<ATPSCharacter>(Pawns[0]);
    if (!TestNotNull("TPSCharacter exists", Character)) return false;

    const auto InvComp = Character->FindComponentByClass<UTPSInventoryComponent>();
    if (!TestNotNull("InvComp exists", InvComp)) return false;
    TestTrueExpr(InvComp->GetInventoryAmountByType(InvData.Type) == 0);

    // Character takes inventory item
    Character->SetActorLocation(InitialTransform.GetLocation());

    TestTrueExpr(InvComp->GetInventoryAmountByType(InvData.Type) == InvData.Score);
    TestTrueExpr(!IsValid(InvItem));

    TArray<AActor*> InvItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InvItems);
    TestTrueExpr(InvItems.Num() == 0);

    return true;
}

#endif
