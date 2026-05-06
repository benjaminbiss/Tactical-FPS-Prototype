#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KH_PlayerCharacter.generated.h"

class AKH_WeaponBase;
class IKH_WeaponInterface;

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

UCLASS()
class KILLHOUSE_API AKH_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	/** Constructor */
	AKH_PlayerCharacter();

#pragma region Components
protected:
	/** Camera Pivot to allow fps mesh to move with camera on the y rot */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* CameraPivot;

	/** Pawn mesh: first person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* FirstPersonMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* FirstPersonCameraComponent;

	//** Weapon Skeletal Mesh*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;

	//** Weapon Actor */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	AKH_WeaponBase* Weapon;
	//** Weapon Interface */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TScriptInterface<IKH_WeaponInterface> WeaponInterface;
	//** Weapon Spawn Class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TSubclassOf<class AKH_WeaponBase> WeaponToSpawn;
#pragma endregion

#pragma region Input
protected:
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SprintAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* CrouchAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MouseLookAction;
		
	/** Mouse Aim Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AimAction;

	/** Mouse Shoot Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ShootAction;

	/** Reload Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ReloadAction;
#pragma endregion

#pragma region UE Overrides
protected:
	/** Gameplay Tick */
	virtual void Tick(float DeltaSeconds) override;
	/** Gameplay Set Up */
	virtual void BeginPlay() override;
	/** Gameplay Clean Up */
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
#pragma endregion

#pragma region Input Handling
protected:
	/** Called from Input Actions for movement input */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void HandleMoveInput(const FInputActionValue& Value);

	/** Called from Input Actions for looking input */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void HandleLookInput(const FInputActionValue& Value);

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void MoveCharacter(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void LookCharacter(float Yaw, float Pitch);

	/** Handles jump start inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void HandleJumpStart();

	/** Handles jump end inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void HandleJumpEnd();

	/** Handles sprint start inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void HandleSprintStart();

	/** Handles sprint end inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void HandleSprintEnd();

	/** Handles crouch start inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void HandleCrouchStart();

	/** Handles crouch end inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void HandleCrouchEnd();

	/** Handles aim start inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void HandleAimStart();

	/** Handles aim end inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void HandleAimEnd();

	/** Handles shoot start inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void HandleShootStart();

	/** Handles shoot end inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void HandleShootEnd();

	/** Handles reload inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void HandleReload();
#pragma endregion

#pragma region Movement Variables
protected:
	//** Used for Aiming Logic and Animation */
	bool bIsAiming = false;
	//** Used for Sprinting Logic */
	bool bCanSprint = false; 
	//** Used for Animation */
	bool bIsSprinting = false;
	//** Used for Animation */
	bool bWeaponObstructed = false;
	//** Used to Lerp Camera for weapon recoil*/
	FVector TargetCameraRecoilOffset;
#pragma endregion

#pragma region Movement Variables Getters
public:
	//** Accessor for Animation Instance */
	bool GetIsSprinting() { return bIsSprinting; };
	//** Accessor for Animation Instance */
	bool GetIsAiming() { return bIsAiming; };
	float GetADSInTime();
	float GetADSOutTime();
#pragma endregion

#pragma region Animation Notifies
	//** Handles binding to each animation montage notify*/
	void InitAnimations();
	void BindWeaponReadyAnimation(UAnimMontage* Montage);
	/** Handles reload anim notify */
	UFUNCTION()
	void HandleWeaponReadyAnimNotify();
#pragma endregion
};
