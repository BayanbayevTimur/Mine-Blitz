// Fill out your copyright notice in the Description page of Project Settings.

#include "MineBlitzGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine.h"

void AMineBlitzGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	ChangeMenuWidget(StartingWidgetClass);
}

void AMineBlitzGameModeBase::BeginCountdown()
{
	GetWorld()->GetTimerManager().SetTimer(MainGameTimerHandle, this, &AMineBlitzGameModeBase::CountdownGameTimer, 1.0f, true);
}


void AMineBlitzGameModeBase::CountdownGameTimer()
{
	if (GameTimer >= 0.0f)
	{
		GameTimer -= 1.0f;
		CheckGameOver();

		if (GameTimer <= 10.f && GameTimerSound != nullptr)
		{
			UGameplayStatics::PlaySound2D(this, GameTimerSound);
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(MainGameTimerHandle);
		ResetGameTimer();
	}
}

void AMineBlitzGameModeBase::ResetGameTimer()
{
	GameTimer = 60.0f;
}

void AMineBlitzGameModeBase::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}
	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}



