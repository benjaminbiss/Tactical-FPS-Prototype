#pragma once

#include "Weapons/FKH_WeaponStats.h"

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "KH_WeaponInterface.generated.h"

UINTERFACE(MinimalAPI)
class UKH_WeaponInterface : public UInterface
{
	GENERATED_BODY()
};

class KILLHOUSE_API IKH_WeaponInterface
{
	GENERATED_BODY()

public:
    //** Handle Weapon Fire Logic */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
    void Fire();

    //** Handle Weapon Reload Logic */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
    void Reload();

    //** Handle Weapon Light Toggle Logic */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
    void ToggleLight();

    //** Handle Weapon Laser Toggle Logic */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
    void ToggleLaser();

    //UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
    //void StartADS();

    //UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
    //void StopADS();

    //** Access Weapon Stats within a Weapon Data Asset */
    UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
    FKH_WeaponStats GetWeaponStats();
};
