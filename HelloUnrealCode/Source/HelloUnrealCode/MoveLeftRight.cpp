// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveLeftRight.h"

// Sets default values
AMoveLeftRight::AMoveLeftRight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	/*
	static ConstructorHelpers::FObjectFinder<UStaticMesh> sm(TEXT("/Script/Engine.StaticMesh'/Game/LevelPrototyping/Meshes/SM_ChamferCube.SM_ChamferCube'"));

	if (sm.Succeeded())
	{
		StaticMesh->SetStaticMesh(sm.Object);
	}
	*/

	SetRootComponent(Root);
	StaticMesh->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	StaticMesh->SetRelativeTransform(FTransform::Identity);

	IsMoveRight = true;
}

// Called when the game starts or when spawned
void AMoveLeftRight::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMoveLeftRight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsMoveRight)
	{
		StaticMesh->AddRelativeLocation(FVector(1, 0, 0));
		if (StaticMesh->GetRelativeLocation().X > 100)
			IsMoveRight = false;
	}
	else
	{
		StaticMesh->AddRelativeLocation(FVector(-1, 0, 0));
		if (StaticMesh->GetRelativeLocation().X < 0)
			IsMoveRight = true;
	}
}

