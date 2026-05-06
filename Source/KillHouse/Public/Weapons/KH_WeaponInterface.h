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
    //** Set Weapon Can Fire Logic */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
    void SetCanFire(bool Value);
    //** Set Weapon Can Fire Logic */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
    bool GetCanFire();

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

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
    void StartADS();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
    void StopADS();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
    int32 GetAmmoCount();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
    UAnimMontage* GetDrawMontage();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
    UAnimMontage* GetHolsterMontage();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
    UAnimMontage* GetIdleFireMontage();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
    UAnimMontage* GetADSFireMontage();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
    UAnimMontage* GetReloadMontage();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
    UAnimMontage* GetReloadEmptyMontage();

    //** Access Weapon Stats within a Weapon Data Asset */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
    FKH_WeaponStats GetWeaponStats();
};
