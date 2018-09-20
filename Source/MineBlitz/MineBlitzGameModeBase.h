// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MineBlitzGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MINEBLITZ_API AMineBlitzGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	/** Remove the current menu widget and create a new one from the specified class, if provided. */
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

protected:
	/** Called when the game starts. */
	virtual void BeginPlay() override;

	/** The widget class we will use as our menu when the game starts. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
	TSubclassOf<UUserWidget> StartingWidgetClass;

	/** The widget instance that we are using as our menu. */
	UPROPERTY()
	class UUserWidget* CurrentWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = "Game Timer")
	float GameTimer;

	FTimerHandle MainGameTimerHandle;

	void CountdownGameTimer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Timer")
	class USoundCue* GameTimerSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Timer")
	class USoundCue* RoundEndSound;

public:
	UFUNCTION(BlueprintCallable)
	void ResetGameTimer();

	UFUNCTION(BlueprintCallable)
	void BeginCountdown();

	UFUNCTION(BlueprintImplementableEvent)
	void CheckGameOver();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetGameTimer() const { return GameTimer; }


};