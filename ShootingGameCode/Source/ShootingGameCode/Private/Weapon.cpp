// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	WeaponMesh->SetCollisionProfileName("weapon");
	WeaponMesh->SetSimulatePhysics(true);

	bReplicates = true;
	SetReplicateMovement(true);

	SetRootComponent(WeaponMesh);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::EventTrigger_Implementation()
{
	if (IsValid(ShootMontage) == false)
		return;

	OwnChar->PlayAnimMontage(ShootMontage);
}

void AWeapon::EventReload_Implementation()
{
	if (IsValid(ReloadMontage) == false)
		return;

	OwnChar->PlayAnimMontage(ReloadMontage);
}

void AWeapon::EventShoot_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShootEffect,
		WeaponMesh->GetSocketLocation("muzzle"),
		WeaponMesh->GetSocketRotation("muzzle"), 
		FVector(0.1f, 0.1f, 0.1f));

	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ShootSound,
		WeaponMesh->GetSocketLocation("muzzle"));

	APlayerController* shooter = GetWorld()->GetFirstPlayerController();

	FVector CameraLoc = shooter->PlayerCameraManager->GetCameraLocation();
	FVector CameraForward = shooter->PlayerCameraManager->GetActorForwardVector();
	FVector Start = (CameraForward * GetFireStartLength()) + CameraLoc;
	FVector End = (CameraForward * 5000.0f) + CameraLoc;

	ReqShoot(Start, End);
}

void AWeapon::EventPickUp_Implementation(ACharacter* targetChar)
{
	OwnChar = targetChar;

	WeaponMesh->SetSimulatePhysics(false);

	AttachToComponent(targetChar->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("weapon"));
}

void AWeapon::EventDrop_Implementation(ACharacter* targetChar)
{
	OwnChar = nullptr;

	WeaponMesh->SetSimulatePhysics(true);

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void AWeapon::IsCanPickUp_Implementation(bool& IsCanPickUp)
{
	if (OwnChar != nullptr)
	{
		IsCanPickUp = false;
		return;
	}

	IsCanPickUp = true;
}

void AWeapon::ReqShoot_Implementation(FVector vStart, FVector vEnd)
{
	FHitResult result;
	FCollisionObjectQueryParams collisionObjectQuery;
	collisionObjectQuery.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	collisionObjectQuery.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	collisionObjectQuery.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	collisionObjectQuery.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);
	collisionObjectQuery.AddObjectTypesToQuery(ECollisionChannel::ECC_Vehicle);
	collisionObjectQuery.AddObjectTypesToQuery(ECollisionChannel::ECC_Destructible);

	FCollisionQueryParams collisionQuery;
	collisionQuery.AddIgnoredActor(OwnChar);

	bool isHit = GetWorld()->LineTraceSingleByObjectType(result, vStart, vEnd, collisionObjectQuery, collisionQuery);
	DrawDebugLine(GetWorld(), vStart, vEnd, FColor::Yellow, false, 5.0f);

	if (isHit == false)
		return;

	ACharacter* HitChar = Cast<ACharacter>(result.GetActor());
	if (HitChar == nullptr)
		return;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
		FString::Printf(TEXT("ApplyDamage HitChar=%s"), *HitChar->GetName()));
	
	UGameplayStatics::ApplyDamage(HitChar, 10, OwnChar->GetController(), this, UDamageType::StaticClass());
}

float AWeapon::GetFireStartLength()
{
	if (IsValid(OwnChar) == false)
		return 0.0f;

	USpringArmComponent* arm = Cast<USpringArmComponent>(OwnChar->GetComponentByClass(USpringArmComponent::StaticClass()));
	if (IsValid(arm) == false)
		return 0.0f;

	return arm->TargetArmLength + 100;
}

