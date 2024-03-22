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

private:
	void PlaceBlock(ABaseChunk* Chunk, const FVector& WorldPos, const FVector& HitNormal, EBlock Block);

};
