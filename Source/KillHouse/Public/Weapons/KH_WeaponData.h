#pragma once

#include "Weapons/FKH_WeaponStats.h"

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "KH_WeaponData.generated.h"

UCLASS()
class KILLHOUSE_API UKH_WeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditDefaultsOnly) FKH_WeaponStats Stats;

    UPROPERTY(EditDefaultsOnly) TSubclassOf<class UKH_WeaponAnimBase> WeaponAnimInstanceClass;
    UPROPERTY(EditDefaultsOnly) UAnimMontage* DrawMontage;
    UPROPERTY(EditDefaultsOnly) UAnimMontage* HolsterMontage;
    UPROPERTY(EditDefaultsOnly) UAnimMontage* IdleFireMontage;
    UPROPERTY(EditDefaultsOnly) UAnimMontage* ADSFireMontage;
    UPROPERTY(EditDefaultsOnly) UAnimMontage* WeaponFireMontage;
    UPROPERTY(EditDefaultsOnly) UAnimMontage* ReloadMontage;
    UPROPERTY(EditDefaultsOnly) UAnimMontage* ReloadEmptyMontage;
    UPROPERTY(EditDefaultsOnly) UAnimMontage* WeaponReloadMontage;
    UPROPERTY(EditDefaultsOnly) UAnimMontage* WeaponReloadEmptyMontage;
    
    UPROPERTY(EditDefaultsOnly) UParticleSystem* MuzzleFlash;
    UPROPERTY(EditDefaultsOnly) USoundBase* FireSound;
};
