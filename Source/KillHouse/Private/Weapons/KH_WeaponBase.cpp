#include "Weapons/KH_WeaponBase.h"

#include "Kismet/GameplayStatics.h"

AKH_WeaponBase::AKH_WeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Configure WeaponMesh
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPS_Gun"));
	WeaponMesh->SetupAttachment(GetRootComponent());
	WeaponMesh->SetOnlyOwnerSee(true);
	WeaponMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	WeaponMesh->SetCollisionProfileName(FName("NoCollision"));

	// Configure Muzzlepoint
	MuzzlePoint = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzlePoint"));
	MuzzlePoint->SetupAttachment(WeaponMesh);
}

#pragma region UE Overrides
void AKH_WeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (WeaponData == nullptr)
	{
		this->Destroy();
		UE_LOG(LogTemp, Warning, TEXT("AKH_WeaponBase::BeginPlay || WeaponData is nullptr, Destroying Actor."));
		return;
	}

	// Configure weapon from weapon data
	SetWeaponMesh();
	SetAnimInstance();
}
#pragma endregion

#pragma region Weapon Interface Implementation
void AKH_WeaponBase::Fire_Implementation()
{
	if (!bCanFire || bIsReloading || CurrentAmmo <= 0) return;

	HandleFire();

	bCanFire = false;
	GetWorldTimerManager().SetTimer(FireRateTimer, [this]()
	{
		bCanFire = true;
	}, 60.f / WeaponData->Stats.FireRate, false);
}

void AKH_WeaponBase::Reload_Implementation()
{
	UAnimMontage* ReloadMontage = WeaponData->ReloadMontage;
	if (ReloadMontage == nullptr)
		return;
	if (WeaponMesh == nullptr)
		return;
	UAnimInstance* AnimInstance = WeaponMesh->GetAnimInstance();
	if (AnimInstance == nullptr)
		return;

	AnimInstance->PlaySlotAnimationAsDynamicMontage(ReloadMontage, "Action", 0.25f, 0.25f, 1.0f, 1, -1.0f, 0.0f);
}

void AKH_WeaponBase::ToggleLight_Implementation()
{
	// Turn off/on Light
}

void AKH_WeaponBase::ToggleLaser_Implementation()
{
	// Turn off/on Laser
}

FKH_WeaponStats AKH_WeaponBase::GetWeaponStats_Implementation()
{
	if (WeaponData == nullptr)
		return FKH_WeaponStats();

	return WeaponData->Stats;
}
#pragma endregion

#pragma region Weapon Data Logic
void AKH_WeaponBase::SetWeaponMesh()
{
	if (WeaponData->WeaponMesh == nullptr)
		return;

	WeaponMesh->SetSkeletalMesh(WeaponData->WeaponMesh);
}

UAnimMontage* AKH_WeaponBase::GetFireMontage()
{
	return WeaponData->FireMontage;
}

UAnimMontage* AKH_WeaponBase::GetReloadMontage()
{
	return WeaponData->ReloadMontage;
}

void AKH_WeaponBase::SetAnimInstance()
{
	if (WeaponData->AnimInstanceClass == nullptr)
		return;

	WeaponMesh->SetAnimInstanceClass(WeaponData->AnimInstanceClass);
}

UAnimInstance* AKH_WeaponBase::GetAnimInstance()
{
	return WeaponMesh->GetAnimInstance();
}

UParticleSystem* AKH_WeaponBase::GetMuzzleFlash()
{
	return WeaponData->MuzzleFlash;
}

USoundBase* AKH_WeaponBase::GetFireSound()
{
	return WeaponData->FireSound;
}
#pragma endregion

#pragma region Weapon Logic
void AKH_WeaponBase::HandleFire()
{
	CurrentAmmo--;

	// Linetrace
	FHitResult Hit;
	FVector Start = MuzzlePoint->GetComponentLocation();
	FVector End = Start + MuzzlePoint->GetForwardVector() * WeaponData->Stats.Range;
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility);

	if (Hit.GetActor())
	{
		// Apply damage via interface or damage system
	}

	// Effects
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponData->MuzzleFlash,
		MuzzlePoint->GetComponentTransform());
	UGameplayStatics::PlaySoundAtLocation(this, WeaponData->FireSound,
		GetActorLocation());

	// Play anim on weapon mesh
	UAnimMontage* FireMontage = WeaponData->FireMontage;
	if (FireMontage == nullptr)
		return;
	if (WeaponMesh == nullptr)
		return;
	UAnimInstance* AnimInstance = WeaponMesh->GetAnimInstance();
	if (AnimInstance == nullptr)
		return;

	AnimInstance->PlaySlotAnimationAsDynamicMontage(FireMontage, "Action", 0.25f, 0.25f, 1.0f, 1, -1.0f, 0.0f);
}
#pragma endregion
