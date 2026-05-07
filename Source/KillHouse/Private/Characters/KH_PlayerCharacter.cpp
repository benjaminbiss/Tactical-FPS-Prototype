#include "Characters/KH_PlayerCharacter.h"

#include "Animations/WeaponReadyAnimNotify.h"
#include "Weapons/KH_WeaponBase.h"

#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

AKH_PlayerCharacter::AKH_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// Configure the the Character's main mesh to not be seen by the owning player, and to be a world space representation (not rendered in the first person view)
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->SetRelativeLocation(FVector(0, 0.0f, -90.0f));
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	// Configure the weapon mesh
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	WeaponMesh->SetupAttachment(GetMesh(), FName("HandGrip_R"));
	WeaponMesh->SetOwnerNoSee(true);
	WeaponMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;
	WeaponMesh->SetCollisionProfileName(FName("NoCollision"));

	// Configure the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FirstPersonCameraComponent->SetupAttachment(GetMesh(), FName("head"));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 90.0f;
	FirstPersonCameraComponent->FirstPersonScale = 1.0f;

	// Configure the first person arms mesh to be seen only by the owning player
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arms"));
	FirstPersonMesh->SetupAttachment(FirstPersonCameraComponent);
	FirstPersonMesh->SetRelativeLocation(FVector(-0.663123f, -0.0f, -162.575101f));
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Configure size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(34.0f, 90.0f);	

	// Configure character movement
	GetCharacterMovement()->SetCrouchedHalfHeight(45.0f);
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

#pragma region UE Overrides
void AKH_PlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!TargetCameraRecoilOffset.IsNearlyZero())
	{
		const float CameraSmoothSpeed = 0.25f;
		float YawDelta = TargetCameraRecoilOffset.X * CameraSmoothSpeed;
		float PitchDelta = TargetCameraRecoilOffset.Z * CameraSmoothSpeed;
		AddControllerYawInput((FMath::FRand() * 2 - 1.0f) * YawDelta); // -1,1 range * recoil horizontal
		AddControllerPitchInput(FMath::FRand() * PitchDelta);
		TargetCameraRecoilOffset -= FVector(YawDelta, 0, PitchDelta);
	}
}

void AKH_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Configure character movement MOVE TO CONSTRUCTOR WHEN DONE WITH TWEAKING VALUES
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;

	// Configure Weapon
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.Instigator = GetInstigator();
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Weapon = GetWorld()->SpawnActor<AKH_WeaponBase>(WeaponToSpawn, this->GetActorTransform(), Params);
	if (Weapon)
	{
		Weapon->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("ik_hand_gun"));
		WeaponInterface = Weapon;
		WeaponInterface->Execute_SetCanFire(WeaponInterface.GetObject(), false);

		// Configure Animations
		InitAnimations();
		FirstPersonMesh->GetAnimInstance()->Montage_Play(WeaponInterface->Execute_GetDrawMontage(WeaponInterface.GetObject()));		
	}

}

void AKH_PlayerCharacter::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

}

void AKH_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AKH_PlayerCharacter::HandleJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AKH_PlayerCharacter::HandleJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AKH_PlayerCharacter::HandleMoveInput);

		// Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AKH_PlayerCharacter::HandleSprintStart);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AKH_PlayerCharacter::HandleSprintEnd);

		// Crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AKH_PlayerCharacter::HandleCrouchStart);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AKH_PlayerCharacter::HandleCrouchEnd);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AKH_PlayerCharacter::HandleLookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AKH_PlayerCharacter::HandleLookInput);

		// Aiming
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AKH_PlayerCharacter::HandleAimStart);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AKH_PlayerCharacter::HandleAimEnd);

		// Shooting
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AKH_PlayerCharacter::HandleShootStart);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &AKH_PlayerCharacter::HandleShootEnd);

		// Reload
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AKH_PlayerCharacter::HandleReload);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}
#pragma endregion

#pragma region Input Handling
void AKH_PlayerCharacter::HandleMoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	MoveCharacter(MovementVector.X, MovementVector.Y);
}

void AKH_PlayerCharacter::HandleLookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	LookCharacter(LookAxisVector.X, LookAxisVector.Y);
}

void AKH_PlayerCharacter::MoveCharacter(float Right, float Forward)
{
	if (GetController())
	{
		// Handle Sprint Logic
		if (bCanSprint && Forward > 0)
		{
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
			bIsSprinting = true;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = 400.0f;
			bIsSprinting = false;
		}
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void AKH_PlayerCharacter::LookCharacter(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AKH_PlayerCharacter::HandleAimStart()
{
	bIsAiming = true;
	WeaponInterface->Execute_StartADS(WeaponInterface.GetObject());
}

void AKH_PlayerCharacter::HandleAimEnd()
{
	bIsAiming = false;
	WeaponInterface->Execute_StopADS(WeaponInterface.GetObject());
}

void AKH_PlayerCharacter::HandleShootStart()
{
	if (WeaponInterface == nullptr)
		return;	

	int32 AmmoCount = WeaponInterface->Execute_GetAmmoCount(WeaponInterface.GetObject());
	bool bCanFire = WeaponInterface->Execute_GetCanFire(WeaponInterface.GetObject());
	if (bCanFire && AmmoCount > 0)
	{
		WeaponInterface->Execute_Fire(WeaponInterface.GetObject());

		FKH_WeaponStats stats = WeaponInterface->Execute_GetWeaponStats(WeaponInterface.GetObject());
		FVector recoil = bIsAiming ? stats.ADSShotImpulseVector : stats.IdleShotImpulseVector;
		TargetCameraRecoilOffset += recoil;

		UAnimMontage* FireMontage = bIsAiming ? WeaponInterface->Execute_GetADSFireMontage(WeaponInterface.GetObject()) : WeaponInterface->Execute_GetIdleFireMontage(WeaponInterface.GetObject());
		if (FireMontage)
		{
			FirstPersonMesh->GetAnimInstance()->Montage_Play(FireMontage);
			// play on character world mesh here too
		}
	}
}

void AKH_PlayerCharacter::HandleShootEnd()
{

}

void AKH_PlayerCharacter::HandleReload()
{
	int32 AmmoCount = WeaponInterface->Execute_GetAmmoCount(WeaponInterface.GetObject());
	UAnimMontage* ReloadMontage = AmmoCount > 0 ? WeaponInterface->Execute_GetReloadMontage(WeaponInterface.GetObject()) : WeaponInterface->Execute_GetReloadEmptyMontage(WeaponInterface.GetObject());

	if (ReloadMontage)
	{
		WeaponInterface->Execute_SetCanFire(WeaponInterface.GetObject(), false);
		WeaponInterface->Execute_Reload(WeaponInterface.GetObject());
		FirstPersonMesh->GetAnimInstance()->Montage_Play(ReloadMontage);
	}
}

void AKH_PlayerCharacter::HandleJumpStart()
{
	Jump(); // ACharacter
}

void AKH_PlayerCharacter::HandleJumpEnd()
{
	StopJumping(); // ACharacter
}

void AKH_PlayerCharacter::HandleSprintStart()
{
	bCanSprint = true;
}

void AKH_PlayerCharacter::HandleSprintEnd()
{
	bCanSprint = false;
}

void AKH_PlayerCharacter::HandleCrouchStart()
{
	if (!GetCharacterMovement()->IsFalling())
	Crouch();
}

void AKH_PlayerCharacter::HandleCrouchEnd()
{
	UnCrouch();
}
#pragma endregion

#pragma region Movement Variables Getters
float AKH_PlayerCharacter::GetADSInTime()
{
	if (WeaponInterface == nullptr)
		return 0.0f;

	FKH_WeaponStats stats = WeaponInterface->Execute_GetWeaponStats(WeaponInterface.GetObject());
	return stats.ADSTimeIn;
}
float AKH_PlayerCharacter::GetADSOutTime()
{
	if (WeaponInterface == nullptr)
		return 0.0f;

	FKH_WeaponStats stats = WeaponInterface->Execute_GetWeaponStats(WeaponInterface.GetObject());
	return stats.ADSTimeOut;
}
#pragma endregion

#pragma region Animation Notifies
void AKH_PlayerCharacter::InitAnimations()
{
	BindWeaponReadyAnimation(WeaponInterface->Execute_GetDrawMontage(WeaponInterface.GetObject()));
	BindWeaponReadyAnimation(WeaponInterface->Execute_GetReloadMontage(WeaponInterface.GetObject()));
	BindWeaponReadyAnimation(WeaponInterface->Execute_GetReloadEmptyMontage(WeaponInterface.GetObject()));
}
void AKH_PlayerCharacter::BindWeaponReadyAnimation(UAnimMontage* Montage)
{
	if (Montage)
	{
		const auto NotifyEvents = Montage->Notifies;
		for (FAnimNotifyEvent EventNotify : NotifyEvents)
		{
			if (const auto MyNotify = Cast<UWeaponReadyAnimNotify>(EventNotify.Notify))
			{
				MyNotify->OnWeaponReadyNotify.AddUObject(this, &AKH_PlayerCharacter::HandleWeaponReadyAnimNotify);
			}
		}
	}
}

void AKH_PlayerCharacter::HandleWeaponReadyAnimNotify()
{
	UE_LOG(LogTemp, Warning, TEXT("WeaponReady"));
	if (WeaponInterface)
		WeaponInterface->Execute_SetCanFire(WeaponInterface.GetObject(), true);
}
#pragma endregion