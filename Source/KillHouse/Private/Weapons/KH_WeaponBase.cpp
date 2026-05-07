#include "Weapons/KH_WeaponBase.h"

#include "Kismet/GameplayStatics.h"

#include "Animations/KH_WeaponAnimBase.h"
#include "Animations/ReloadAnimNotify.h"

AKH_WeaponBase::AKH_WeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Configure Weapon Mesh
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetRootComponent());
	WeaponMesh->SetOnlyOwnerSee(true);
	WeaponMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	WeaponMesh->SetCollisionProfileName(FName("NoCollision"));

	// Configure Magazine Mesh
	MagazineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MagazineMesh"));
	MagazineMesh->SetupAttachment(WeaponMesh, FName("MagazineSocket"));
	MagazineMesh->SetOnlyOwnerSee(true);
	MagazineMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	MagazineMesh->SetCollisionProfileName(FName("NoCollision"));

	// Configure Muzzle point
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
	SetAnimInstance();
	CurrentAmmo = WeaponData->Stats.MagSize;

	// Configure animation Notifies
	UAnimMontage* ReloadMontage = WeaponData->WeaponReloadMontage;
	if (ReloadMontage)
	{
		const auto NotifyEvents = ReloadMontage->Notifies;
		for (FAnimNotifyEvent EventNotify : NotifyEvents)
		{
			if (const auto MyNotify = Cast<UReloadAnimNotify>(EventNotify.Notify))
			{
				MyNotify->OnReloadNotify.AddUObject(this, &AKH_WeaponBase::HandleReloadAnimNotify);
			}
		}
	}
	ReloadMontage = WeaponData->WeaponReloadEmptyMontage;
	if (ReloadMontage)
	{
		const auto NotifyEvents = ReloadMontage->Notifies;
		for (FAnimNotifyEvent EventNotify : NotifyEvents)
		{
			if (const auto MyNotify = Cast<UReloadAnimNotify>(EventNotify.Notify))
			{
				MyNotify->OnReloadNotify.AddUObject(this, &AKH_WeaponBase::HandleReloadAnimNotify);
			}
		}
	}
}
#pragma endregion

#pragma region Weapon Interface Implementation
void AKH_WeaponBase::SetCanFire_Implementation(bool Value)
{
	bCanFire = Value;
}

bool AKH_WeaponBase::GetCanFire_Implementation()
{
	return bCanFire;
}

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
	if (WeaponMesh == nullptr || WeaponData->WeaponAnimInstanceClass == nullptr)
		return;

	UAnimMontage* ReloadMontage = nullptr;
	if (CurrentAmmo > 0)
		ReloadMontage = WeaponData->WeaponReloadMontage;
	else
		ReloadMontage = WeaponData->WeaponReloadEmptyMontage;

	if (ReloadMontage == nullptr)
		return;
		
	WeaponMesh->GetAnimInstance()->Montage_Play(ReloadMontage);
}

void AKH_WeaponBase::ToggleLight_Implementation()
{
	// Turn off/on Light
}

void AKH_WeaponBase::ToggleLaser_Implementation()
{
	// Turn off/on Laser
}

void AKH_WeaponBase::StartADS_Implementation()
{
	bIsAiming = true;
}

void AKH_WeaponBase::StopADS_Implementation()
{
	bIsAiming = false;
}

int32 AKH_WeaponBase::GetAmmoCount_Implementation()
{
	return CurrentAmmo;
}

UAnimMontage* AKH_WeaponBase::GetDrawMontage_Implementation()
{
	return WeaponData->DrawMontage;
}

UAnimMontage* AKH_WeaponBase::GetHolsterMontage_Implementation()
{
	return WeaponData->HolsterMontage;
}

UAnimMontage* AKH_WeaponBase::GetIdleFireMontage_Implementation()
{
	return WeaponData->IdleFireMontage;
}

UAnimMontage* AKH_WeaponBase::GetADSFireMontage_Implementation()
{
	return WeaponData->ADSFireMontage;
}

UAnimMontage* AKH_WeaponBase::GetReloadMontage_Implementation()
{
	return WeaponData->ReloadMontage;
}

UAnimMontage* AKH_WeaponBase::GetReloadEmptyMontage_Implementation()
{
	return WeaponData->ReloadEmptyMontage;
}

FKH_WeaponStats AKH_WeaponBase::GetWeaponStats_Implementation()
{
	if (WeaponData == nullptr)
		return FKH_WeaponStats();

	return WeaponData->Stats;
}
#pragma endregion

#pragma region Weapon Data Logic
UAnimMontage* AKH_WeaponBase::GetWeaponFireMontage()
{
	return WeaponData->WeaponFireMontage;
}

UAnimMontage* AKH_WeaponBase::GetReloadMontage()
{
	return WeaponData->ReloadMontage;
}

void AKH_WeaponBase::SetAnimInstance()
{
	if (WeaponData->WeaponAnimInstanceClass == nullptr)
		return;

	WeaponMesh->SetAnimInstanceClass(WeaponData->WeaponAnimInstanceClass);
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
	FVector End = Start + MuzzlePoint->GetRightVector() * WeaponData->Stats.Range;
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility);

	FColor DebugLineColor = FColor::Red;
	if (Hit.GetActor())
	{
		// Apply damage via interface or damage system
		DebugLineColor = FColor::Green;
	}

	// Debug
	if (bDebugBullets)
	{
		DrawDebugLine(GetWorld(), Start, End, DebugLineColor, false, 1.0f);
	}

	// Effects
	if (WeaponData->MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponData->MuzzleFlash,
			MuzzlePoint->GetComponentTransform());
	}
	if (WeaponData->FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, WeaponData->FireSound,
			GetActorLocation());
	}

	// Play anim on weapon mesh
	if (WeaponMesh == nullptr || WeaponMesh->GetAnimInstance() == nullptr)
		return;

	UAnimMontage* FireMontage = GetWeaponFireMontage();
	if (FireMontage)
		WeaponMesh->GetAnimInstance()->Montage_Play(FireMontage);
}
#pragma endregion

#pragma region Animation Notifies
void AKH_WeaponBase::HandleReloadAnimNotify()
{
	CurrentAmmo = WeaponData->Stats.MagSize;
}
#pragma endregion