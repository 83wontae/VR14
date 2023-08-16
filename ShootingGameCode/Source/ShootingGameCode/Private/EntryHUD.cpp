// Fill out your copyright notice in the Description page of Project Settings.


#include "EntryHUD.h"
#include "Blueprint/UserWidget.h"

void AEntryHUD::BeginPlay()
{
	Super::BeginPlay();

	check(HudWidgetClass);

	HudWidget = CreateWidget<UUserWidget>(GetWorld(), HudWidgetClass);
	HudWidget->AddToViewport();
}