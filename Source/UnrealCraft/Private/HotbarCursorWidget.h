// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HotbarCursorWidget.generated.h"

/**
 * Represents the cursor on the hotbar showing where it should be.
 */
UCLASS()
class UHotbarCursorWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* AppearAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> DisappearAnimation;

	UPROPERTY(EditDefaultsOnly)
	ESlateVisibility VisibilityOnFirstNewParent = ESlateVisibility::SelfHitTestInvisible;

private:
	// Current parent can be retrieved using GetParent()
	
	UPROPERTY()
	TObjectPtr<UPanelWidget> TargetParent;

	bool IsAnimating = false;
	bool IsFirstTransition = true;

public:
	/**
	 * Change parents by using animations.
	 * @param NewParent New parent.
	 */
	void AnimateToNewParent(UPanelWidget* NewParent);

protected:
	virtual void OnAnimationFinishedPlaying(UUMGSequencePlayer& Player) override;

private:
	/**
	 * Begin transitioning to parent. Calls initial transition.
	 */
	void BeginTransitioning();
};
