#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KH_PlayerAnim.generated.h"

class AKH_PlayerCharacter;

UCLASS()
class KILLHOUSE_API UKH_PlayerAnim : public UAnimInstance
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

    //** bIsJumping */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bIsJumping;

    //** bIsCrouching */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bIsCrouching;

    //** Gather and update animation parameters from Owner */
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
