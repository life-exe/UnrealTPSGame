// My game copyright

#include "Weapon/TPSTurret.h"
#include "Weapon/TPSProjectile.h"
#include "Components/StaticMeshComponent.h"

ATPSTurret::ATPSTurret()
{
    PrimaryActorTick.bCanEverTick = false;

    TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>("TurretMesh");
    check(TurretMesh);
    SetRootComponent(TurretMesh);
}

void ATPSTurret::BeginPlay()
{
    Super::BeginPlay();

    check(AmmoCount > 0);
    check(FireFrequency > 0.0f);

    const float FirstDelay = FireFrequency;
    GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATPSTurret::OnFire, FireFrequency, true, FirstDelay);
}

void ATPSTurret::OnFire()
{
    if (--AmmoCount == 0)
    {
        GetWorldTimerManager().ClearTimer(FireTimerHandle);
    }

    if (GetWorld())
    {
        const auto SocketTransform = TurretMesh->GetSocketTransform("Muzzle");
        auto* ProjectileObj = GetWorld()->SpawnActorDeferred<ATPSProjectile>(ProjectileClass, SocketTransform);
        if (ProjectileObj)
        {
            ProjectileObj->SetShotDirection(SocketTransform.GetRotation().GetForwardVector());
            ProjectileObj->FinishSpawning(SocketTransform);
        }
    }
}
