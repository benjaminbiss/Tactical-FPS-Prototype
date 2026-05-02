#include "Controllers/FK_PlayerController_FPS.h"

#include "KillHouseCameraManager.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"

AFK_PlayerController_FPS::AFK_PlayerController_FPS()
{
	// set the player camera manager class
	PlayerCameraManagerClass = AKillHouseCameraManager::StaticClass();
}

void AFK_PlayerController_FPS::BeginPlay()
{
	Super::BeginPlay();

}

void AFK_PlayerController_FPS::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}
		}
	}
}