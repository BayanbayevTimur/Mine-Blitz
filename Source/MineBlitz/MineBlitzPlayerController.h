// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MineBlitzPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MINEBLITZ_API AMineBlitzPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void BeginPlay() override;
	
};
