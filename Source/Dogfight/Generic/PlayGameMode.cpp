// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#include "PlayGameMode.h"
#include "Generic/PoolingManager.h"

APlayGameMode::APlayGameMode()
{
	
}

void APlayGameMode::BeginPlay()
{
	if (HasAuthority())
	{
		PoolingManager = NewObject<UPoolingManager>(UPoolingManager::StaticClass());
		PoolingManager->SetWorld(GetWorld());
	}
}

UPoolingManager* APlayGameMode::GetPoolingManager()
{
	UPoolingManager* Out = PoolingManager;

	return Out;
}