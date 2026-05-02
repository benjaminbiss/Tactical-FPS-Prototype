#include "Animations/KH_AnimInstance_Toon.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UKH_AnimInstance_Toon::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

    if (!Owner)
    {
        Owner = Cast<ACharacter>(TryGetPawnOwner()); 
        if (!Owner)
            return;
    }

    Speed = Owner->GetVelocity().Size();
	Direction = CalculateDirection(Owner->GetVelocity(), Owner->GetActorRotation());
    bIsJumping = Owner->GetCharacterMovement()->IsFalling();
	bIsCrouching = Owner->GetCharacterMovement()->IsCrouching();
}
