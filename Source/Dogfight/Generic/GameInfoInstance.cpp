// Fill out your copyright notice in the Description page of Project Settings.

#include "GameInfoInstance.h"
#include "Dogfight.h"
#include "Generic/DogfightSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Generic/PlayerHUD.h"

void UGameInfoInstance::StartGame()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if (PC != nullptr)
	{
		PC->bShowMouseCursor = true;
	}

	PlayerHUD = (APlayerHUD*)PC->GetHUD();

	if (PlayerHUD != nullptr)
	{
		PlayerHUD->ShowMainMenu();
	}

	CheckSavedData();
}

void UGameInfoInstance::CheckSavedData()
{
	UDogfightSaveGame* SaveGameInstance = Cast<UDogfightSaveGame>(UGameplayStatics::CreateSaveGameObject(UDogfightSaveGame::StaticClass()));

	if (SaveGameInstance->IsValidLowLevel())
	{
		if (!UGameplayStatics::DoesSaveGameExist(SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex))
		{
			PlayerHUD->ShowFirstConnectionScreen();
		}
		else
		{
			LoadSavedData();
		}
	}
}

void UGameInfoInstance::LoadSavedData(bool bQuelchMessage)
{
	UDogfightSaveGame* LoadInstance = Cast<UDogfightSaveGame>(UGameplayStatics::CreateSaveGameObject(UDogfightSaveGame::StaticClass()));
	
	if (LoadInstance->IsValidLowLevel())
	{
		if (!UGameplayStatics::DoesSaveGameExist(LoadInstance->SaveSlotName, 0))
		{
			if(!bQuelchMessage)
			{
				UE_LOG(LogLoad, Warning, TEXT("Something strange happened. LoadSavedData was called but no data could be found."));
			}
			
		}
		else
		{
			LoadInstance = Cast<UDogfightSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadInstance->SaveSlotName, LoadInstance->UserIndex));

			PlayerInformation = LoadInstance->PlayerInfo;
		}
	}
}

void UGameInfoInstance::SavePlayerData(FPlayerInformation PlayerInfo)
{
	UDogfightSaveGame* SaveInstance = Cast<UDogfightSaveGame>(UGameplayStatics::CreateSaveGameObject(UDogfightSaveGame::StaticClass()));

	if (SaveInstance->IsValidLowLevel())
	{
		SaveInstance->PlayerInfo = PlayerInfo;

		UGameplayStatics::SaveGameToSlot(SaveInstance, SaveInstance->SaveSlotName, SaveInstance->UserIndex);
	}

	LoadSavedData();
}

FPlayerInformation UGameInfoInstance::GetPlayerData()
{
	LoadSavedData(true);
	return PlayerInformation;
}

void UGameInfoInstance::OnSessionDestroyed()
{

}

void UGameInfoInstance::DestroySession_Implementation()
{

}

void UGameInfoInstance::LaunchLobby_Implementation(int InNumOfPlayers, bool InEnableLan, const FText& InServerName)
{
	MaxPlayers = InNumOfPlayers;
	ServerName = InServerName;
}

void UGameInfoInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Here we list the variables we want to replicate + a condition if wanted
	DOREPLIFETIME(UGameInfoInstance, MaxPlayers);
	DOREPLIFETIME(UGameInfoInstance, ServerName);
}