// Fill out your copyright notice in the Description page of Project Settings.


#include "HotbarCursorWidget.h"

#include "Animation/UMGSequencePlayer.h"
#include "Components/PanelWidget.h"

void UHotbarCursorWidget::AnimateToNewParent(UPanelWidget* NewParent)
{
	TargetParent = NewParent;

	BeginTransitioning();

	// if new parent != null
	// begin transitioning to hidden
	// set parent to new parent
	// begin transitioning to showing
	// goto first line
}

void UHotbarCursorWidget::OnAnimationFinishedPlaying(UUMGSequencePlayer& Player)
{
	Super::OnAnimationFinishedPlaying(Player);

	if (Player.GetAnimation() == DisappearAnimation)
	{
		TargetParent->AddChild(this);
		TargetParent = nullptr;

		if (IsFirstTransition)
		{
			SetVisibility(VisibilityOnFirstNewParent);
			IsFirstTransition = false;
		}
		
		PlayAnimation(AppearAnimation);
	}
	else if (Player.GetAnimation() == AppearAnimation)
	{
		IsAnimating = false;
		if (TargetParent != nullptr)
			BeginTransitioning();
	}
}

void UHotbarCursorWidget::BeginTransitioning()
{
	if (IsAnimating)
		return;
	
	IsAnimating = true;
	PlayAnimation(DisappearAnimation);
}
