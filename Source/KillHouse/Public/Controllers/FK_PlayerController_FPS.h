#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FK_PlayerController_FPS.generated.h"

class UInputMappingContext;
class UUserWidget;

UCLASS()
class KILLHOUSE_API AFK_PlayerController_FPS : public APlayerController
{
	GENERATED_BODY()
	
public:
	/** Constructor */
	AFK_PlayerController_FPS();

#pragma region Components
protected:
	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category = "Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;
#pragma endregion

#pragma region UE Overrides
protected:
	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;
#pragma endregion

};
