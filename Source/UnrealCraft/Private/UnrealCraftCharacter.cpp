// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealCraftCharacter.h"

#include "ChunkWorld.h"
#include "PlayerHUD.h"
#include "VoxelGameState.h"
#include "VoxelUtils.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealCraft/Public/Inventory.h"

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

	PlayerInputComponent->BindAction("PlayerInventory", IE_Pressed, this, &AUnrealCraftCharacter::PlayerInventory);
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

	HitChunk->ModifyVoxel(VoxelUtils::WorldToLocalBlockPosition(HitResult.Location - HitResult.Normal, HitChunk->GetChunkSize()), EBlock::Air);
}

void AUnrealCraftCharacter::StopHit()
{
}

void AUnrealCraftCharacter::Interact()
{
	FVector Start = FPSCameraComponent->GetComponentLocation();
	FVector End = Start + (FPSCameraComponent->GetForwardVector() * Reach);
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	auto HasHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 15.f);

	if (!HasHit)
		return;

	if (!HitResult.GetActor()->ActorHasTag(AChunkWorld::DefaultChunkTag))
		return;

	ABaseChunk* HitChunk = Cast<ABaseChunk>(HitResult.GetActor());

#if UE_BUILD_DEVELOPMENT
	check(HitChunk != nullptr)
#endif

	DrawDebugSphere(GetWorld(), HitResult.Location, 1.f, 10, FColor::Green, false, 15.f);
	DrawDebugLine(GetWorld(), HitResult.Location, HitResult.Location - (HitResult.Normal*10.f), FColor::Green, false, 15.f);
	switch (auto HitBlock = HitChunk->GetBlock(VoxelUtils::WorldToLocalBlockPosition(HitResult.Location - HitResult.Normal, HitChunk->GetChunkSize())))
	{
	case EBlock::Inventory:
		{
			TSharedPtr<IInventoryInterface> WorldInventory;
			if (GetWorld()->GetGameState<AVoxelGameState>()->GetInventoryDatabase().GetWorldInventory(VoxelUtils::WorldToBlockPosition(HitResult.Location - HitResult.Normal), WorldInventory))
			{
				WorldInventory->Open();
			}
		}
		break;
	
	default:
		PlaceBlock(HitChunk, HitResult.Location - HitResult.Normal, HitResult.Normal, EBlock::Inventory);
		break;
		
	}
}

void AUnrealCraftCharacter::PlayerInventory()
{
	// TODO: if this ever goes multiplayer, this needs to be fixed probably.

	auto GameState = Cast<AVoxelGameState>(GetWorld()->GetGameState());
	
	if (GameState == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, TEXT("[AUnrealCraftCharacter::PlayerInventory]: Unable to get gamestate."));
		return;
	}
		
	// get player inventory (jeez)
	TSharedPtr<IInventoryInterface> PlayerInventory;
	GameState->GetInventoryDatabase().GetEntityInventory(
			"Player", PlayerInventory);
	
	if (PlayerInventory == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, TEXT("[AUnrealCraftCharacter::PlayerInventory]: Unable to get player inventory."));
		return;
	}
	
	auto PlayerHUD = Cast<APlayerHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	
	if (PlayerHUD == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, TEXT("[AUnrealCraftCharacter::PlayerInventory]: Unable to get hud."));
		return;
	}
	
	PlayerHUD->GetInventoryScreenWidget()->ShowPlayerInventory(PlayerInventory);
}

void AUnrealCraftCharacter::PlaceBlock(ABaseChunk* Chunk, const FVector& WorldPos, const FVector& HitNormal, EBlock Block)
{
	switch (Block)
	{
	case EBlock::Inventory:
		GetWorld()->GetGameState<AVoxelGameState>()->GetInventoryDatabase().AddWorldInventory(VoxelUtils::WorldToBlockPosition(WorldPos) + FIntVector(HitNormal), MakeShared<Inventory>());
		// goto default;
	default:
		Chunk->ModifyVoxel(VoxelUtils::WorldToLocalBlockPosition(WorldPos, Chunk->GetChunkSize()) + FIntVector(HitNormal), Block);
		break;
	}
}

