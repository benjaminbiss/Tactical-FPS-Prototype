#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KH_PlayerAnim_FPS.generated.h"

class AKH_PlayerCharacter;

UCLASS()
class KILLHOUSE_API UKH_PlayerAnim_FPS : public UAnimInstance
{
	GENERATED_BODY()

protected:
    //** Cached reference to the owning character */
    AKH_PlayerCharacter* Owner;

public:
    //** Speed */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float Speed;

    //** Direction */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float Direction;

    //** bIsAiming */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bIsAiming;

    //** bIsJumping */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bIsJumping;

    //** bIsCrouching */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bIsCrouching;

    //** bIsSprinting */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bIsSprinting;

    //** Gather and update animation parameters from Owner */
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
