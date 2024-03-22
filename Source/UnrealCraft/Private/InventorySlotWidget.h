// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnButtonClicked();
	
protected:
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UWidget> ItemParent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UButton> ButtonWidget;
};
