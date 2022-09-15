// My game copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPSTurret.generated.h"

class UStaticMeshComponent;
class ATPSProjectile;

UCLASS(Abstract)
class TPS_API ATPSTurret : public AActor
{
    GENERATED_BODY()

public:
    ATPSTurret();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* TurretMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    int32 AmmoCount{10};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Units = s))
    float FireFrequency{1.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<ATPSProjectile> ProjectileClass;

private:
    UPROPERTY()
    FTimerHandle FireTimerHandle;

    void OnFire();
};
