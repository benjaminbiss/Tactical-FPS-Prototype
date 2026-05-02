#include "Animations/KH_PlayerAnim_FPS.h"

#include "KismetAnimationLibrary.h"

#include "Characters/KH_PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UKH_PlayerAnim_FPS::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!Owner)
    {
        Owner = Cast<AKH_PlayerCharacter>(TryGetPawnOwner());
        if (!Owner)
            return;
    }

    Speed = Owner->GetVelocity().Size();
    Direction = UKismetAnimationLibrary::CalculateDirection(Owner->GetVelocity(), Owner->GetActorRotation());
    bIsJumping = Owner->GetCharacterMovement()->IsFalling();
    bIsCrouching = Owner->GetCharacterMovement()->IsCrouching();
    bIsSprinting = Owner->GetIsSprinting();
}
