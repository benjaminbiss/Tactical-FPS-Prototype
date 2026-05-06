// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/ReloadAnimNotify.h"

void UReloadAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	OnReloadNotify.Broadcast();
}
