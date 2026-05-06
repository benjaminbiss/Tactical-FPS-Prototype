#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "WeaponReadyAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNotifiedSignature);

UCLASS()
class KILLHOUSE_API UWeaponReadyAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	FOnNotifiedSignature OnWeaponReadyNotify;
	
};
