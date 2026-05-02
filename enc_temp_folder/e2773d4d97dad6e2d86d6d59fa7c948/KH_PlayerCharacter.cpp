#include "Characters/KH_PlayerCharacter.h"

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

	// Configure the first person weapon mesh
	FPSWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPS_Gun"));
	FPSWeaponMesh->SetupAttachment(FirstPersonMesh, FName("ik_hand_gun"));
	FPSWeaponMesh->SetOnlyOwnerSee(true);
	FPSWeaponMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FPSWeaponMesh->SetCollisionProfileName(FName("NoCollision"));

	// Configure size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(34.0f, 90.0f);	

	// Configure character movement
	GetCharacterMovement()->CrouchedHalfHeight = 45.0f;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

#pragma region UE Overrides
void AKH_PlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsAdjustingSpeed)
	{

	}
}

void AKH_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Configure character movement MOVE TO CONSTRUCTOR WHEN DONE WITH TWEAKING VALUES
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
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

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AKH_PlayerCharacter::HandleLookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AKH_PlayerCharacter::HandleLookInput);
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
	AimCharacter(LookAxisVector.X, LookAxisVector.Y);
}

void AKH_PlayerCharacter::MoveCharacter(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void AKH_PlayerCharacter::AimCharacter(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
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
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	bIsAdjustingSpeed = true;
	bIsSprinting = true;
}

void AKH_PlayerCharacter::HandleSprintEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	bIsAdjustingSpeed = true;
	bIsSprinting = false;
}

void AKH_PlayerCharacter::HandleCrouchStart()
{
	if (!GetCharacterMovement()->IsFalling())
	Crouch();
	bIsAdjustingSpeed = true;
}

void AKH_PlayerCharacter::HandleCrouchEnd()
{
	UnCrouch();
	bIsAdjustingSpeed = true;
}
#pragma endregion