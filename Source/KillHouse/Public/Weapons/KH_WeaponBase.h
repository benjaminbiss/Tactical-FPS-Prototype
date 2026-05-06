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
	//** Set Weapon Can Fire Logic */
	virtual void SetCanFire_Implementation(bool Value) override;
	//** Get Weapon Can Fire Logic */
	virtual bool GetCanFire_Implementation() override;
	//** Handle Weapon Fire Logic */
	virtual void Fire_Implementation() override;
	//** Handle Weapon Reload Logic */
	virtual void Reload_Implementation() override;
	//** Handle Weapon Light Toggle Logic */
	virtual void ToggleLight_Implementation() override;
	//** Handle Weapon Laser Toggle Logic */
	virtual void ToggleLaser_Implementation() override;
	//** Start Weapon ADS */
	virtual void StartADS_Implementation() override;
	//** Stop Weapon ADS */
	virtual void StopADS_Implementation() override;
	//** Get Weapons current ammo count */
	virtual int32 GetAmmoCount_Implementation() override;
	//** Get Caracter Draw Anim from Weapon Data Asset */
	virtual UAnimMontage* GetDrawMontage_Implementation() override;
	//** Get Caracter Hoslter Anim from Weapon Data Asset */
	virtual UAnimMontage* GetHolsterMontage_Implementation() override;
	//** Get Caracter Idle Fire Anim from Weapon Data Asset */
	virtual UAnimMontage* GetIdleFireMontage_Implementation() override;
	//** Get Caracter ADS Fire Anim from Weapon Data Asset */
	virtual UAnimMontage* GetADSFireMontage_Implementation() override;
	//** Get Caracter Reload Anim from Weapon Data Asset */
	virtual UAnimMontage* GetReloadMontage_Implementation() override;
	//** Get Caracter Reload Empty Anim from Weapon Data Asset */
	virtual UAnimMontage* GetReloadEmptyMontage_Implementation() override;
	//** Access Weapon Stats within a Weapon Data Asset */
	virtual FKH_WeaponStats GetWeaponStats_Implementation() override;
#pragma endregion

#pragma region Weapon Data Logic
	//** Configure Mesh from Weapon Data Asset */
	void SetWeaponMesh();
	//** Get Fire Anim from Weapon Data Asset */
	UAnimMontage* GetWeaponFireMontage();
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
	bool bIsAiming = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Data")
	bool bIsReloading = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Data")
	bool bCanFire = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Data")
	bool bDebugBullets = true;
#pragma endregion

#pragma region Animation Notifies
	/** Handles fps skeleton anim notify */
	UFUNCTION()
	void HandleAnimiationNotifies(FName NotifyName, const FBranchingPointNotifyPayload& Payload);
	/** Handles reload anim notify */
	UFUNCTION()
	void HandleReloadAnimNotify();
#pragma endregion
};
