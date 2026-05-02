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
    UPROPERTY(EditDefaultsOnly) USkeletalMesh* WeaponMesh;
    UPROPERTY(EditDefaultsOnly) UAnimMontage* FireMontage;
    UPROPERTY(EditDefaultsOnly) UAnimMontage* ReloadMontage;
    UPROPERTY(EditDefaultsOnly) TSubclassOf<UAnimInstance> AnimInstanceClass;
    UPROPERTY(EditDefaultsOnly) UParticleSystem* MuzzleFlash;
    UPROPERTY(EditDefaultsOnly) USoundBase* FireSound;
};
