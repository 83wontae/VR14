// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LobbyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAMECODE_API ALobbyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ALobbyPlayerState();

public:
	UFUNCTION()
	void OnRep_UserName();

	UFUNCTION(BlueprintCallable)
	void SetUserName(const FString& NewName);

private:
	UPROPERTY(ReplicatedUsing = OnRep_UserName)
	FString UserName;


};
