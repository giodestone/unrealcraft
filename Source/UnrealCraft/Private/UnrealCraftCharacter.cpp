// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealCraftCharacter.h"

#include "ChunkWorld.h"
#include "VoxelUtils.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/UserWidget.h"

AUnrealCraftCharacter::AUnrealCraftCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FPSCameraComponent != nullptr);
	FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
	FPSCameraComponent->bUsePawnControlRotation = true;
}

void AUnrealCraftCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr);
	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello World, this is UnrealCraftCharacter!"));
}

void AUnrealCraftCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUnrealCraftCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("Forward", this, &AUnrealCraftCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Strafe", this, &AUnrealCraftCharacter::MoveStrafe);
	PlayerInputComponent->BindAxis("ViewHorizontal", this, &AUnrealCraftCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("ViewVertical", this, &AUnrealCraftCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AUnrealCraftCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Repeat, this, &AUnrealCraftCharacter::RepeatJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AUnrealCraftCharacter::StopJump);

	PlayerInputComponent->BindAction("Hit", IE_Pressed, this, &AUnrealCraftCharacter::StartHit);
	PlayerInputComponent->BindAction("Hit", IE_Released, this, &AUnrealCraftCharacter::StopHit);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AUnrealCraftCharacter::Interact);
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

void AUnrealCraftCharacter::StartJump()
{
	bPressedJump = true;
}

void AUnrealCraftCharacter::RepeatJump()
{
	if (!bWasJumping)
		bPressedJump = true;
}

void AUnrealCraftCharacter::StopJump()
{
	bPressedJump = false;
}

void AUnrealCraftCharacter::StartHit()
{
	FVector Start = FPSCameraComponent->GetComponentLocation();
	FVector End = Start + (FPSCameraComponent->GetForwardVector() * Reach);
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	auto HasHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5.f);

	if (!HasHit)
		return;

	if (!HitResult.GetActor()->ActorHasTag(AChunkWorld::DefaultChunkTag))
		return;

	ABaseChunk* HitChunk = Cast<ABaseChunk>(HitResult.GetActor());

#if UE_BUILD_DEVELOPMENT
	check(HitChunk != nullptr)
#endif

	HitChunk->ModifyVoxel(VoxelUtils::WorldToLocalBlockPosition(HitResult.Location + HitResult.Normal, HitChunk->GetChunkSize()), EBlock::Air);
}

void AUnrealCraftCharacter::StopHit()
{
}

void AUnrealCraftCharacter::Interact()
{
}

