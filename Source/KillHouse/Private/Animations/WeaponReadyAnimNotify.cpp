// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/WeaponReadyAnimNotify.h"

void UWeaponReadyAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	OnWeaponReadyNotify.Broadcast();
}
