// Fill out your copyright notice in the Description page of Project Settings.

#include "MineBlitzCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include <Animation/AnimInstance.h>
#include "Engine.h"
#include "MinableObject.h"
#include "Components/StaticMeshComponent.h"

AMineBlitzCharacter::AMineBlitzCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	MyPickAxe = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My PickAxe"));
	MyPickAxe->SetupAttachment(GetMesh(), TEXT("RightHand"));
	//MyPickAxe->AttachRootComponentTo(GetMesh(), "RightHand");
	//MyPickAxe->SetActorRelativeLocation(FVector(-19.200001, 4.700000, 31.830000));
	//MyPickAxe->SetActorRelativeRotation(FRotator(-74.300003, 17.700001, 104.860001));
	MyPickAxe->SetRelativeLocation(FVector(-19.200001, 4.700000, 31.830000));
	MyPickAxe->SetRelativeRotation(FRotator(-74.300003, 17.700001, 104.860001));

	PickAxeCapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PickAxe Capsule Component"));
	PickAxeCapsuleComp->SetupAttachment(MyPickAxe);
	PickAxeCapsuleComp->SetRelativeLocation(FVector(-43.0f, 0.0f, 0.0f));
	PickAxeCapsuleComp->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	PickAxeCapsuleComp->SetRelativeScale3D(FVector(0.699389, -0.148722, 1.086160));
	PickAxeCapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AMineBlitzCharacter::CollisionEnter);
	//PickAxeCapsuleComp->OnComponentEndOverlap.AddDynamic(this, &AMineBlitzCharacter::CollisionExit);


	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	bIsRunning = false;
	bIsMining = false;
	bCanMine = true;
	ObjectCanHit = false;
	MineCost = 12.0f;

	PickAxePower = 5;
	MoveSpeed = 125.0f;
	MaxStamina = 100.0f;
	StaminaRegenRate = 2.5f;
	CurrentStamina = MaxStamina;
	CurrentMoveSpeed = MoveSpeed;
}

void AMineBlitzCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RegenerateStamina(DeltaTime);

	if (ObjectCanHit)
	{
		PickAxeCapsuleComp->bGenerateOverlapEvents = true;
	}
	else if (!ObjectCanHit)
	{
		PickAxeCapsuleComp->bGenerateOverlapEvents = false;
	}

	if (bIsMining)
	{
		Mine();
	}
	if (!bIsRunning)
	{
		CurrentMoveSpeed = MoveSpeed;
	}
	else if (bIsRunning)
	{
		CurrentMoveSpeed = MoveSpeed * 2.5f;
	}
}

void AMineBlitzCharacter::UpgradePickAxePower()
{
	PickAxePower += 5;
	GoldResource -= 3;
	IronResource -= 3;
	CopperResource -= 3;
}

void AMineBlitzCharacter::SetMoveSpeed()
{
	MoveSpeed += 50.0f;
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	CopperResource -= 6;
}

void AMineBlitzCharacter::SetStamina()
{
	MaxStamina = MaxStamina * 1.3f;
	IronResource -= 3;
}

void AMineBlitzCharacter::CashOutOres(int Round)
{
	switch (Round)
	{
	case 2:
		Score += (GoldResource * 4 + IronResource * 2 + CopperResource) * 4;
		GoldResource = 0;
		IronResource = 0;
		CopperResource = 0;
		break;
	case 1:
		Score += (GoldResource * 4 + IronResource * 2 + CopperResource) * 2;
		GoldResource = 0;
		IronResource = 0;
		CopperResource = 0;
		break;
	case 0:
		Score += (GoldResource * 4 + IronResource * 2 + CopperResource);
		GoldResource = 0;
		IronResource = 0;
		CopperResource = 0;
		break;
	}
}

void AMineBlitzCharacter::ResetBools()
{
	bIsRunning = false;
	bIsMining = false;
	bCanMine = true;
	ObjectCanHit = false;
}

void AMineBlitzCharacter::ResetStamina()
{
	CurrentStamina = MaxStamina;
}


//Movement Component
void AMineBlitzCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("LeftClick", IE_Pressed, this, &AMineBlitzCharacter::Mine);
	PlayerInputComponent->BindAction("LeftClick", IE_Released, this, &AMineBlitzCharacter::ResetMine);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AMineBlitzCharacter::ToggleRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AMineBlitzCharacter::ToggleRun);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMineBlitzCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMineBlitzCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookRight", this, &AMineBlitzCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &AMineBlitzCharacter::LookUpAtRate);
}

void AMineBlitzCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMineBlitzCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMineBlitzCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMineBlitzCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AMineBlitzCharacter::ToggleRun()
{
	if (!bIsRunning)
	{
		GetCharacterMovement()->MaxWalkSpeed = MoveSpeed*2.5f;
		bIsRunning = true;
	}
	else if (bIsRunning)
	{
		GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
		bIsRunning = false;
	}
}

void AMineBlitzCharacter::Mine()
{
	if (CurrentStamina > MineCost)
	{
		if (bCanMine)
		{
			CurrentStamina -= MineCost;
			AnimInstance = GetMesh()->GetAnimInstance();
			MiningMontageTime = MiningMontage->GetPlayLength();

			if (AnimInstance != nullptr)
			{
				bCanMine = false;
				bIsMining = true;
				AnimInstance->Montage_Play(MiningMontage);
			}
			GetWorld()->GetTimerManager().SetTimer(MiningDelayHandle, this, &AMineBlitzCharacter::ResetMiningTime, MiningMontageTime, false);
		}
	}
}

void AMineBlitzCharacter::CheckObject()
{
	if (MinableObject != nullptr)
	{
		int CurrentResource = MinableObject->GetResourceCount();
		int ResourceResistance = MinableObject->GetResistance();
		int ResourceType = MinableObject->GetResourceType();
		if (CurrentResource > (PickAxePower - ResourceResistance) && (PickAxePower > ResourceResistance))
		{
			switch(ResourceType)
			{
				case 1:
					CopperResource += (PickAxePower - ResourceResistance);
					break;
				case 2:
					IronResource += (PickAxePower - ResourceResistance);
					break;
				case 3:
					GoldResource += (PickAxePower - ResourceResistance);
					break;
			}
		}
		else
		{
			switch (ResourceType)
			{
			case 1:
				CopperResource += CurrentResource;
				break;
			case 2:
				IronResource += CurrentResource;
				break;
			case 3:
				GoldResource += CurrentResource;
				break;
			}
		}
		MinableObject->SetResource(CurrentResource - (PickAxePower - ResourceResistance));
		MinableObject->CheckResource();
	}
}

void AMineBlitzCharacter::ResetMiningTime()
{
	bCanMine = true;
	ObjectCanHit = false;
	GetWorldTimerManager().ClearTimer(MiningDelayHandle);

	if (MinableObject != nullptr)
	{
		MinableObject->CheckResource();
	}
}

void AMineBlitzCharacter::ResetMine()
{
	bIsMining = false;
}

void AMineBlitzCharacter::RegenerateStamina(float DeltaTime)
{
	if (CurrentStamina <= MaxStamina)
	{
		CurrentStamina += StaminaRegenRate * DeltaTime;
	}
}

void AMineBlitzCharacter::CollisionEnter(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor == Cast<AMinableObject>(OtherActor))
	{
		MinableObject = Cast<AMinableObject>(OtherActor);
		if (ObjectCanHit)
		{
			ObjectCanHit = false;
			if (OreHitSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, OreHitSound, GetActorLocation());
				CheckObject();
			}
		}
	}
}
