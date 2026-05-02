#pragma once

#include "Weapons/KH_WeaponInterface.h"
#include "Weapons/KH_WeaponData.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KH_WeaponBase.generated.h"

UCLASS()
class KILLHOUSE_API AKH_WeaponBase : public AActor, public IKH_WeaponInterface
{
	GENERATED_BODY()
	
public:	
	AKH_WeaponBase();

#pragma region Components
protected:
	//** Weapon Skeletal Mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* MuzzlePoint;
#pragma endregion

#pragma region UE Overrides
protected:
	//** Begin Play */
	virtual void BeginPlay() override;
#pragma endregion

#pragma region Weapon Interface Implementation
public:
	//** Handle Weapon Fire Logic */
	virtual void Fire_Implementation() override;
	//** Handle Weapon Reload Logic */
	virtual void Reload_Implementation() override;
	//** Handle Weapon Light Toggle Logic */
	virtual void ToggleLight_Implementation() override;
	//** Handle Weapon Laser Toggle Logic */
	virtual void ToggleLaser_Implementation() override;
	//** Access Weapon Stats within a Weapon Data Asset */
	virtual FKH_WeaponStats GetWeaponStats_Implementation() override;
#pragma endregion

#pragma region Weapon Data Logic
	//** Configure Mesh from Weapon Data Asset */
	void SetWeaponMesh();
	//** Get Fire Anim from Weapon Data Asset */
	UAnimMontage* GetFireMontage();
	//** Get Reload Anim from Weapon Data Asset */
	UAnimMontage* GetReloadMontage();
	//** Set Anim Instance from Weapon Data Asset */
	void SetAnimInstance();
	//** Get Anim Instance from Weapon Data Asset */
	UAnimInstance* GetAnimInstance();
	//** Get Muzzle Flash from Weapon Data Asset */
	UParticleSystem* GetMuzzleFlash();
	//** Get Fire Sound from Weapon Data Asset */
	USoundBase* GetFireSound();
#pragma endregion

#pragma region Weapon Logic
protected:
	void HandleFire();

	FTimerHandle FireRateTimer;
	FTimerHandle ReloadTimer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Data")
	UKH_WeaponData* WeaponData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Data")
	int32 CurrentAmmo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Data")
	bool bIsReloading;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Data")
	bool bCanFire = true;
#pragma endregion
};
