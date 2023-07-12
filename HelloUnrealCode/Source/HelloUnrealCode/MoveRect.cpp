// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveRect.h"

// Sets default values
AMoveRect::AMoveRect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMoveRect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMoveRect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (MoveType)
	{
	case EN_MoveType::MoveRight:
		{
		}
		break;
	case EN_MoveType::MoveUp:
		{
		}
		break;
	case EN_MoveType::MoveLeft:
		{
		}
		break;
	case EN_MoveType::MoveDown:
		{
		}
		break;
	default:
		break;
	}
}

