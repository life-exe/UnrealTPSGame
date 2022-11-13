// My game copyright

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPS/TPSTypes.h"
#include "TPSInventoryComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TPS_API UTPSInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTPSInventoryComponent();

    /**
     * Trying to add inventory to Inventory TMap
     * Score of the Data object can't be < 0
     * Final score might be less than the corresponding limit
     * @param   Data  inventory data to add
     */
    bool TryToAddItem(const FInventoryData& Data);

    UFUNCTION(BlueprintCallable)
    int32 GetInventoryAmountByType(EInventoryItemType Type) const;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TMap<EInventoryItemType, int32> InventoryLimits;

private:
    TMap<EInventoryItemType, int32> Inventory;
};
