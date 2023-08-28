// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "ShootingHUD.h"

AShootingPlayerState::AShootingPlayerState()
{
	CurHp = 100;
	MaxHp = 100;
	CurSh = 100;
	MaxSh = 100;
	Mag = 0;
}

void AShootingPlayerState::BeginPlay()
{
	Super::BeginPlay();

	UpdateBind();
}

void AShootingPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShootingPlayerState, CurHp);
	DOREPLIFETIME(AShootingPlayerState, MaxHp);
	DOREPLIFETIME(AShootingPlayerState, CurSh);
	DOREPLIFETIME(AShootingPlayerState, MaxSh);
	DOREPLIFETIME(AShootingPlayerState, Mag);
	DOREPLIFETIME(AShootingPlayerState, UserName);
	DOREPLIFETIME(AShootingPlayerState, Kill);
	DOREPLIFETIME(AShootingPlayerState, Death);
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

void AShootingPlayerState::OnRep_CurSh()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
		FString::Printf(TEXT("OnRep_CurSh=%f"), CurSh));

	if (Fuc_Dele_UpdateSh.IsBound())
		Fuc_Dele_UpdateSh.Broadcast(CurSh, MaxSh);
}

void AShootingPlayerState::OnRep_MaxSh()
{
}

void AShootingPlayerState::OnRep_Mag()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
		FString::Printf(TEXT("OnRep_Mag=%d"), Mag));

	if (Fuc_Dele_UpdateMag.IsBound())
		Fuc_Dele_UpdateMag.Broadcast(Mag);
}

void AShootingPlayerState::OnRep_Kill()
{
	if (Fuc_Dele_UpdateKillDeath.IsBound())
		Fuc_Dele_UpdateKillDeath.Broadcast(Kill, Death);
}

void AShootingPlayerState::OnRep_Death()
{
	if (Fuc_Dele_UpdateKillDeath.IsBound())
		Fuc_Dele_UpdateKillDeath.Broadcast(Kill, Death);
}

bool AShootingPlayerState::AddDamage(float Damage)
{
	float leftDamage = AddDamageSh(Damage);

	if (leftDamage <= 0)
	{
		return false;
	}

	if (CurHp <= 0)
	{
		return false;
	}

	CurHp = CurHp - leftDamage;
	CurHp = FMath::Clamp(CurHp, 0.0f, MaxHp);

	OnRep_CurHp();
	
	/*
	if (CurHp <= 0)
	{
		return true;
	}

	return false;
	*/
	return (CurHp <= 0) ? true : false;
}

float AShootingPlayerState::AddDamageSh(float Damage)
{
	if (CurSh <= 0)
	{
		return Damage;
	}

	float TempSh = CurSh - Damage;
	
	if (TempSh > 0)
	{
		CurSh = TempSh;
		OnRep_CurSh();
		return 0;
	}

	CurSh = 0;
	OnRep_CurSh();
	return FMath::Abs(TempSh);
}

void AShootingPlayerState::AddMag()
{
	Mag = Mag + 1;

	OnRep_Mag();
}

bool AShootingPlayerState::UseMag()
{
	if (IsCanUseMag() == false)
		return false;

	Mag = Mag - 1;
	OnRep_Mag();

	return true;
}

bool AShootingPlayerState::IsCanUseMag()
{
	if (Mag <= 0)
		return false;

	return true;
}

void AShootingPlayerState::AddHeal(float Heal)
{
	CurHp = CurHp + Heal;
	CurHp = FMath::Clamp(CurHp, 0.0f, MaxHp);

	OnRep_CurHp();
}

void AShootingPlayerState::AddHealSh(float Heal)
{
	CurSh = CurSh + Heal;
	CurSh = FMath::Clamp(CurSh, 0.0f, MaxSh);

	OnRep_CurSh();
}

void AShootingPlayerState::AddKill()
{
	Kill = Kill + 1;

	OnRep_Kill();
}

void AShootingPlayerState::AddDeath()
{
	Death = Death + 1;

	OnRep_Death();
}

void AShootingPlayerState::OnRep_UserName()
{
	GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Yellow,
		FString::Printf(TEXT("(Client)OnRep_UserName UserName=%s"), *UserName));

	if (Func_Dele_UpdateUserName.IsBound())
		Func_Dele_UpdateUserName.Broadcast(UserName);
}

void AShootingPlayerState::SetUserName(const FString& NewName)
{
	UserName = NewName;

	OnRep_UserName();
}

void AShootingPlayerState::UpdateBind()
{
	APlayerController* player0 = GetWorld()->GetFirstPlayerController();
	if (player0)
	{
		AShootingHUD* pHud = Cast<AShootingHUD>(player0->GetHUD());
		if (pHud)
		{
			pHud->BindPlayerState(this);
			return;
		}
	}

	FTimerManager& TimerMgr = GetWorldTimerManager();
	TimerMgr.SetTimer(th_UpdateBind, this, &AShootingPlayerState::UpdateBind, 0.1f, false);
}
