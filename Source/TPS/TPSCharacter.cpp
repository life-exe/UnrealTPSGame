// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPSCharacter.h"

#if MY_VR_MACRO
#include "HeadMountedDisplayFunctionLibrary.h"
#endif

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "TPS/Components/TPSInventoryComponent.h"
#include "TimerManager.h"

//////////////////////////////////////////////////////////////////////////
// ATPSCharacter

ATPSCharacter::ATPSCharacter()
{
    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // set our turn rates for input
    BaseTurnRate = 45.f;
    BaseLookUpRate = 45.f;

    // Don't rotate when the controller rotates. Let that just affect the camera.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true;             // Character moves in the direction of input...
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);  // ...at this rotation rate
    GetCharacterMovement()->JumpZVelocity = 600.f;
    GetCharacterMovement()->AirControl = 0.2f;

    // Create a camera boom (pulls in towards the player if there is a collision)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f;        // The camera follows at this distance behind the character
    CameraBoom->bUsePawnControlRotation = true;  // Rotate the arm based on the controller

    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);  // Attach the camera to the end of the boom and let the
                                                                                 // boom adjust to match the controller orientation
    FollowCamera->bUsePawnControlRotation = false;                               // Camera does not rotate relative to arm

    // Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
    // are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

    InventoryComponent = CreateDefaultSubobject<UTPSInventoryComponent>("InventoryComponent");
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    // Set up gameplay key bindings
    check(PlayerInputComponent);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

    PlayerInputComponent->BindAxis("MoveForward", this, &ATPSCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ATPSCharacter::MoveRight);

    // We have 2 versions of the rotation bindings to handle different kinds of devices differently
    // "turn" handles devices that provide an absolute delta, such as a mouse.
    // "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("TurnRate", this, &ATPSCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("LookUpRate", this, &ATPSCharacter::LookUpAtRate);

    // handle touch devices
    PlayerInputComponent->BindTouch(IE_Pressed, this, &ATPSCharacter::TouchStarted);
    PlayerInputComponent->BindTouch(IE_Released, this, &ATPSCharacter::TouchStopped);

    // VR headset functionality
    PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ATPSCharacter::OnResetVR);
}

void ATPSCharacter::OnResetVR()
{
// If TPS is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in TPS.Build.cs is not
// automatically propagated and a linker error will result. You will need to either:
//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if
// supporting VR).
// or:
//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
#if MY_VR_MACRO
    UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
#endif
}

void ATPSCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
    Jump();
}

void ATPSCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
    StopJumping();
}

void ATPSCharacter::TurnAtRate(float Rate)
{
    // calculate delta for this frame from the rate information
    AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATPSCharacter::LookUpAtRate(float Rate)
{
    // calculate delta for this frame from the rate information
    AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATPSCharacter::MoveForward(float Value)
{
    if (Controller && Value != 0.0f)
    {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void ATPSCharacter::MoveRight(float Value)
{
    if (Controller && Value != 0.0f)
    {
        // find out which way is right
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get right vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        // add movement in that direction
        AddMovementInput(Direction, Value);
    }
}

void ATPSCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthData.MaxHealth > 0.0f);
    Health = HealthData.MaxHealth;

    OnTakeAnyDamage.AddDynamic(this, &ATPSCharacter::OnAnyDamageReceived);
}

float ATPSCharacter::GetHealthPercent() const
{
    return Health / HealthData.MaxHealth;
}

void ATPSCharacter::OnAnyDamageReceived(
    AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    const auto IsAlive = [&]() { return Health > 0.0f; };

    if (Damage <= 0.0f || !IsAlive()) return;

    Health = FMath::Clamp(Health - Damage, 0.0f, HealthData.MaxHealth);

    if (IsAlive())
    {
        GetWorldTimerManager().SetTimer(HealTimerHandle, this, &ATPSCharacter::OnHealing, HealthData.HealRate, true, -1.0f);
    }
    else
    {
        OnDeath();
    }
}

void ATPSCharacter::OnHealing()
{
    Health = FMath::Clamp(Health + HealthData.HealModifier, 0.0f, HealthData.MaxHealth);
    if (FMath::IsNearlyEqual(Health, HealthData.MaxHealth))
    {
        Health = HealthData.MaxHealth;
        GetWorldTimerManager().ClearTimer(HealTimerHandle);
    }
}

void ATPSCharacter::OnDeath()
{
    GetWorldTimerManager().ClearTimer(HealTimerHandle);

    check(GetCharacterMovement());
    check(GetCapsuleComponent());
    check(GetMesh());

    GetCharacterMovement()->DisableMovement();
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);
    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }

    SetLifeSpan(HealthData.LifeSpan);
}

void ATPSCharacter::TestClangFormat(AActor* Actor)
{
    for (int32 i = 0; i < 10; ++i)
    {
    }

    int32 value = 2;  // trail comment

    if (true)
    {
    }

    switch (value)
    {
        case 1: value++; break;
        case 2:
        {
        }
        break;

        default: break;
    }

    const TArray<int32> A = {10, 20, 30, 40};
    const int32 Sum1 = A[0] + A[1] + A[2] + A[3];

    // clang-format off
    const int32 Sum2 = A[0] + A[1] +
                       A[2] + A[3];
    AActor    *pointer{  nullptr};
    // clang-format on
}
