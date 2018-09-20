// Fill out your copyright notice in the Description page of Project Settings.

#include "MinableObject.h"
#include "Components/StaticMeshComponent.h"
#include <Components/BoxComponent.h>
#include <Components/SkeletalMeshComponent.h>


// Sets default values
AMinableObject::AMinableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	RootComponent = MeshComponent;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComponent->SetupAttachment(RootComponent);

	Minable = true;
	Resistance = 10;
	ResourceCount = FMath::RoundToInt(FMath::RandRange(50.f, 100.f));
}

void AMinableObject::SetResource(int Amount)
{
	ResourceCount = Amount;
}

void AMinableObject::CheckResource()
{
	if (ResourceCount <= 0)
	{
		Destroy(this);
	}
}

// Called when the game starts or when spawned
void AMinableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMinableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

