#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KH_AnimInstance_Toon.generated.h"

UCLASS()
class KILLHOUSE_API UKH_AnimInstance_Toon : public UAnimInstance
{
	GENERATED_BODY()

protected:
	//** Cached reference to the owning character */
    ACharacter* Owner;

public:
    //** Speed */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float Speed;

    //** Direction */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float Direction;

    //** bIsJumping */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bIsJumping;

    //** bIsCrouching */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bIsCrouching;

    //** Gather and update animation parameters from Owner */
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
