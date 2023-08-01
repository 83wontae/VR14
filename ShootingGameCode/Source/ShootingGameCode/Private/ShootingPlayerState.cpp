// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingPlayerState.h"
#include "Net/UnrealNetwork.h"

AShootingPlayerState::AShootingPlayerState()
{
	CurHp = 100;
	MaxHp = 100;
	Mag = 0;
}

void AShootingPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShootingPlayerState, CurHp);
	DOREPLIFETIME(AShootingPlayerState, MaxHp);
	DOREPLIFETIME(AShootingPlayerState, Mag);
}

void AShootingPlayerState::OnRep_CurHp()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
		FString::Printf(TEXT("OnRep_CurHp=%f"), CurHp));

	if (Fuc_Dele_UpdateHp.IsBound())
		Fuc_Dele_UpdateHp.Broadcast(CurHp, MaxHp);
}

void AShootingPlayerState::OnRep_MaxHp()
{
}

void AShootingPlayerState::OnRep_Mag()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
		FString::Printf(TEXT("OnRep_Mag=%d"), Mag));

	if (Fuc_Dele_UpdateMag.IsBound())
		Fuc_Dele_UpdateMag.Broadcast(Mag);
}

void AShootingPlayerState::AddDamage(float Damage)
{
	CurHp = CurHp - Damage;

	OnRep_CurHp();
}

void AShootingPlayerState::AddMag()
{
	Mag = Mag + 1;

	OnRep_Mag();
}
