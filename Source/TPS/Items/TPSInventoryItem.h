// My game copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPS/TPSTypes.h"
#include "TPSInventoryItem.generated.h"

class USphereComponent;

UCLASS(Abstract)
class TPS_API ATPSInventoryItem : public AActor
{
    GENERATED_BODY()

public:
    ATPSInventoryItem();

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
    UPROPERTY(VisibleAnywhere)
    USphereComponent* CollisionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FInventoryData InventoryData;
};
