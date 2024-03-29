// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enums.h"
#include "PlayerHUD.h"
#include "UnrealCraftCharacter.generated.h"

class AVoxelGameMode;
class AVoxelGameState;
class APlayerHUD;
class ABaseChunk;
class UUserWidget;
class UCameraComponent;

/**
 * Represents the character that the player controls, with specialised logic for it to interact with the world.
 */
UCLASS()
class AUnrealCraftCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<AVoxelGameState> GameState;

	UPROPERTY()
	TObjectPtr<APlayerHUD> PlayerHUD;
	
public:
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FPSCameraComponent;

	UPROPERTY(EditAnywhere)
	float Reach = 6.f * 100.f;

private:
	int32 CurrentlySelectedHotbarSlot = 0;

public:
	
	AUnrealCraftCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION()
	void MoveForward(float Value);

	UFUNCTION()
	void MoveStrafe(float Value);

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void RepeatJump();
	
	UFUNCTION()
	void StopJump();
	
	UFUNCTION()
	void StartHit();

	UFUNCTION()
	void StopHit();

	UFUNCTION()
	void Interact();

	UFUNCTION()
	void PlayerInventory();

	UFUNCTION()
	void NextSelectedHotbarItem();

	UFUNCTION()
	void PreviousSelectedHotbarItem();

private:
	/**
	 * Place a block in the world.
	 * @param Chunk The chunk to place the block in.
	 * @param WorldPos The world position of the location where to place the block in.
	 * @param HitNormal The normal of the hit.
	 * @param Block The new type of block that that block should be replaced with.
	 * @remark The HitNormal should be inverted if attempting to replace a block with air.
	 */
	void PlaceBlock(ABaseChunk* Chunk, const FVector& WorldPos, const FVector& HitNormal, EBlock Block);

	/**
	 * Update the character controller's interaction mode depending on if something is displayed or not.
	 */
	void UpdateInteractionMode(bool IsInventoryMenuDisplayed) const;

};
