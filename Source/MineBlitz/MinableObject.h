// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MinableObject.generated.h"

UCLASS()
class MINEBLITZ_API AMinableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMinableObject();

	/**How much resource is left to mine*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MiningStats")
	int ResourceCount;

	/**The resistance level needed to overcome to mine*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MiningStats")
	float Resistance;

	/**Type of Resource*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MiningStats")
	int ResourceType;

	/**Box Component*/
	UPROPERTY(VisibleAnywhere, Category = "MiningStats")
	class UBoxComponent* BoxComponent;

	/**Mesh Component*/
	UPROPERTY(VisibleAnywhere, Category = "MiningStats")
	class UStaticMeshComponent* MeshComponent;

	/**Whether we can mine the resource or not*/
	bool Minable;

	void SetResource(int Amount);

	void CheckResource();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE int GetResourceCount() { return ResourceCount; }
	FORCEINLINE int GetResistance() { return Resistance; }
	FORCEINLINE int GetResourceType() { return ResourceType; }
	
};
