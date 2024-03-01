// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealCraftCharacter.h"

// Sets default values
AUnrealCraftCharacter::AUnrealCraftCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AUnrealCraftCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr);
	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello World, this is UnrealCraftCharacter!"));
}

// Called every frame
void AUnrealCraftCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUnrealCraftCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("Forward", this, &AUnrealCraftCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Strafe", this, &AUnrealCraftCharacter::MoveStrafe);
	PlayerInputComponent->BindAxis("ViewHorizontal", this, &AUnrealCraftCharacter::LookHorizontal);
	PlayerInputComponent->BindAxis("ViewVertical", this, &AUnrealCraftCharacter::LookVertical);
}

void AUnrealCraftCharacter::MoveForward(float Value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AUnrealCraftCharacter::MoveStrafe(float Value)
{
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
    AddMovementInput(Direction, Value);
}

void AUnrealCraftCharacter::LookVertical(float Value)
{
}

void AUnrealCraftCharacter::LookHorizontal(float Value)
{
}

