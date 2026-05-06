#pragma once

#include "CoreMinimal.h"
#include "FKH_WeaponStats.generated.h"

USTRUCT(BlueprintType)
struct KILLHOUSE_API FKH_WeaponStats
{
	GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly) float Damage = 0.0f;
    UPROPERTY(EditDefaultsOnly) float FireRate = 0.0f;
    UPROPERTY(EditDefaultsOnly) float Range = 0.0f;
    UPROPERTY(EditDefaultsOnly) int32 MagSize = 0;
    UPROPERTY(EditDefaultsOnly) float ReloadTime = 0.0f;
    UPROPERTY(EditDefaultsOnly) float ADSTimeIn = 0.0f;
    UPROPERTY(EditDefaultsOnly) float ADSTimeOut = 0.0f;
    UPROPERTY(EditDefaultsOnly) FVector ADSShotImpulseVector = FVector(0, 0, 0);
    UPROPERTY(EditDefaultsOnly) FVector IdleShotImpulseVector = FVector(0, 0, 0);
    UPROPERTY(EditDefaultsOnly) FVector ADSOffset = FVector(0,0,0);
};
