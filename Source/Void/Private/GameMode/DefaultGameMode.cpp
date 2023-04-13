#include "GameMode/DefaultGameMode.h"

void ADefaultGameMode::BeginPlay()
{
	Super::BeginPlay();

	if(APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PlayerController->bShowMouseCursor = true;
	}
}
