// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryVisualizerWidget.h"

#include "InventoryInterface.h"
#include "InventoryItemWidget.h"
#include "InventorySlotWidget.h"
#include "PlayerInventory.h"
#include "VoxelGameState.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"

void UInventoryVisualizerWidget::TogglePlayerInventory(TObjectPtr<UPlayerInventory> InPlayerInventory, bool& OutIsMenuDisplayed)
{
	switch (State)
	{
	case Hidden:
		InitPlayerInventoryWidget(InPlayerInventory);
		HideSecondaryInventory();

		
		this->CurrentPlayerInventory = InPlayerInventory;
		OutIsMenuDisplayed = true;

		State = ShowingPlayer;
		this->SetFocus();
		break;
		
	case ShowingBoth: // TODO: Identical to ToggleBothInventories, Hide
		HideSecondaryInventory();
		CurrentOtherInventory = nullptr;
	case ShowingPlayer:
		HidePlayerInventory();
		this->CurrentPlayerInventory = nullptr;
		OutIsMenuDisplayed = false;
		ChangePlayerInputsToWorld();
		State = Hidden;
		break;
		
	}
	// TArray<UWidget*> Children;
	// PlayerInventoryMenuWidget->WidgetTree->GetAllWidgets(Children);
}

void UInventoryVisualizerWidget::ToggleBothInventories(TObjectPtr<UPlayerInventory> InPlayerInventory,
                                                     TScriptInterface<IInventoryInterface> OtherInventory,
                                                     bool& OutIsMenuDisplayed)
{
	switch (State)
	{
	case Hidden:
		InitPlayerInventoryWidget(InPlayerInventory);
		InitOtherInventoryWidget(OtherInventory);

		this->CurrentPlayerInventory = InPlayerInventory;
		this->CurrentOtherInventory = OtherInventory;

		OutIsMenuDisplayed = true;
		State = ShowingBoth;
		this->SetFocus();
		break;

	case ShowingBoth: // TODO: Identical to TogglePlayerInventory, Hide
		HideSecondaryInventory();
		CurrentOtherInventory = nullptr;
	case ShowingPlayer:
		HidePlayerInventory();
		this->CurrentPlayerInventory = nullptr;
		OutIsMenuDisplayed = false;
		ChangePlayerInputsToWorld();
		State = Hidden;
		break;
	}
}

void UInventoryVisualizerWidget::Hide()
{
	switch (State)
	{
	case Hidden:
		break;

	case ShowingBoth: // TODO: Identical to TogglePlayerInventory, ToggleBothInventories
		HideSecondaryInventory();
		CurrentOtherInventory = nullptr;
	case ShowingPlayer:
		HidePlayerInventory();
		this->CurrentPlayerInventory = nullptr;
		ChangePlayerInputsToWorld();
		State = Hidden;
		break;
	}
}

void UInventoryVisualizerWidget::OnSlotButtonClicked(UInventorySlotWidget* Widget)
{
	if (CurrentHeldItem == nullptr && Widget->IsHoldingWidget())
	{
		CurrentHeldItem = Widget->RemoveItemWidget();
		HeldItemParentPanel->AddChild(CurrentHeldItem);
		CurrentHeldItem->SetVisibility(ESlateVisibility::HitTestInvisible);
		CloseButton->SetIsEnabled(false);

		UUnrealCraftItem* RemovedItem; // unused because it is already part of CurrentHeldItem.
		Widget->GetAssociatedInventory()->RemoveFrom(Widget->GetRepresentedInventoryCoord(), RemovedItem);
	}
	else if (CurrentHeldItem != nullptr && !Widget->IsHoldingWidget())
	{
		// actually add to inventory
		Widget->GetAssociatedInventory()->InsertInto(Widget->GetRepresentedInventoryCoord(), CurrentHeldItem->GetRepresentedItem());
		
		CurrentHeldItem->RemoveFromParent();
		CurrentHeldItem->SetRenderTranslation(FVector2D::Zero());
		CurrentHeldItem->SetVisibility(ESlateVisibility::Visible);
		Widget->AddItemWidget(CurrentHeldItem);
		CurrentHeldItem = nullptr;

		CloseButton->SetIsEnabled(true);
	}
}

EInventoryVisualiserState UInventoryVisualizerWidget::GetState() const
{
	return State;
}

void UInventoryVisualizerWidget::ChangePlayerInputsToWorld()
{
	if (GetWorld() == nullptr)
		return;
		
	if(GetWorld()->GetFirstPlayerController() == nullptr)
		return;
	
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
}

void UInventoryVisualizerWidget::SetupComponentsFromName()
{
	if (auto PlayerWidget = GetWidgetFromName(PlayerInventoryMenuWidgetName))
		PlayerInventoryMenuWidget = dynamic_cast<UUserWidget*>(PlayerWidget);
	else
		GLog->Log(ELogVerbosity::Error, TEXT("[UInventoryVisualizerWidget::NativeOnInitialized]: Unable to get player grid widget from name. Check blueprint."));

	if (auto OtherWidget = GetWidgetFromName(OtherInventoryMenuWidgetName))
		OtherInventoryMenuWidget = dynamic_cast<UUserWidget*>(OtherWidget);
	else
		GLog->Log(ELogVerbosity::Error, TEXT("[UInventoryVisualizerWidget::NativeOnInitialized]: Unable to get other grid widget from name. Check blueprint."));
	
	if (auto HeldItemParent = dynamic_cast<UPanelWidget*>(GetWidgetFromName(HeldItemParentWidgetName)))
		HeldItemParentPanel = HeldItemParent;
	else
		GLog->Log(ELogVerbosity::Error, TEXT("[UInventoryVisualizerWidget::NativeOnInitialized]: Unable to find the parent for held item. Check blueprint."));
}

UPanelWidget* UInventoryVisualizerWidget::GetPanelSubWidget(UUserWidget* ParentWidget, const FName& PanelWidgetName)
{
	if (ParentWidget == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, TEXT("[UInventoryVisualizerWidget::GetPanelSubWidget]: Unable to get the panel subwidget as the parent widget is null. Check blueprint as this is related to another error."));
		return nullptr;
	}

	auto PanelSubWidget = ParentWidget->GetWidgetFromName(PanelWidgetName);

	if (PanelSubWidget == nullptr)
	{
		UE_LOG(LogTemp, Error,
			           TEXT("[UInventoryVisualizerWidget::GetPanelSubWidget]: Unable to get the panel subwidget named '%s'. Check blueprint for specified name."),
		           PanelWidgetName);
		return nullptr;
	}
	
	return dynamic_cast<UPanelWidget*>(PanelSubWidget);
}


void UInventoryVisualizerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	SetupComponentsFromName();

	PlayerInventoryMenuWidgetSlotParent = GetPanelSubWidget(PlayerInventoryMenuWidget, InventoryMenuSlotParentName);
	OtherInventoryMenuWidgetSlotParent = GetPanelSubWidget(OtherInventoryMenuWidget, InventoryMenuSlotParentName);
	
	dynamic_cast<AVoxelGameState*>(GetWorld()->GetGameState())->SetInventoryVisualizer(this);
	
	CloseButton->OnClicked.AddDynamic(this, &UInventoryVisualizerWidget::OnCloseButtonClicked);
	
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryVisualizerWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventoryVisualizerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	TrackCurrentlyHeldItem();
}


void UInventoryVisualizerWidget::InitPlayerInventoryWidget(TObjectPtr<UPlayerInventory> InPlayerInventory)
{
	CurrentPlayerInventory = InPlayerInventory;
	SpawnInventoryGrid(InPlayerInventory, PlayerInventoryMenuWidget, PlayerInventoryMenuWidgetSlotParent, InventorySlotBlueprint, InventoryItemBlueprint, FIntVector2(0, -1));

	PlayerInventoryMenuWidget->SetVisibility(ESlateVisibility::Visible);
	this->SetVisibility(DefaultSelfVisibility);
	this->SetFocus();
}

void UInventoryVisualizerWidget::InitOtherInventoryWidget(TScriptInterface<IInventoryInterface> OtherInventory)
{
	CurrentOtherInventory = OtherInventory;
	SpawnInventoryGrid(OtherInventory, OtherInventoryMenuWidget, OtherInventoryMenuWidgetSlotParent, InventorySlotBlueprint, InventoryItemBlueprint);

	OtherInventoryMenuAbsoluteParentWidget->SetVisibility(ESlateVisibility::Visible);
	OtherInventoryMenuWidget->SetVisibility(ESlateVisibility::Visible);
	this->SetVisibility(DefaultSelfVisibility);
}

void UInventoryVisualizerWidget::HideSecondaryInventory()
{
	OtherInventoryMenuWidgetSlotParent->ClearChildren();
	OtherInventoryMenuAbsoluteParentWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryVisualizerWidget::HidePlayerInventory()
{
	PlayerInventoryMenuWidgetSlotParent->ClearChildren();
	this->SetVisibility(DefaultSelfInvisibility);
}

void UInventoryVisualizerWidget::TrackCurrentlyHeldItem()
{
	if (CurrentHeldItem != nullptr)
	{
		if (auto PlayerController = GetWorld()->GetFirstPlayerController()) // false if no player controller
		{
			float X, Y;
			if (PlayerController->GetMousePosition(X, Y)) // false if no mouse.
				CurrentHeldItem->SetRenderTranslation(FVector2D(X, Y));
		}
	}
}

void UInventoryVisualizerWidget::SpawnInventoryGrid(TScriptInterface<IInventoryInterface> Inventory, UUserWidget* GridMenuWidget, UPanelWidget* SlotParent, TSubclassOf<UInventorySlotWidget> SlotBlueprint, TSubclassOf<UInventoryItemWidget> ItemBlueprint, FIntVector2
                                                    SizeOffset)
{
	for (int32 x = 0; x < Inventory->GetSize().X + SizeOffset.X; x++)
	{
		for (int32 y = 0; y < Inventory->GetSize().Y + SizeOffset.Y; y++)
		{
			auto NewSlotWidget = Cast<UInventorySlotWidget>(CreateWidget(GetWorld(), SlotBlueprint));
			NewSlotWidget->InitializeData(FIntVector2(x,y), this, Inventory);
			SlotParent->AddChild(NewSlotWidget);

			UUnrealCraftItem* Item;
			if (Inventory->HasItem(FIntVector2(x, y), Item))
			{
				auto NewItemWidget = Cast<UInventoryItemWidget>(CreateWidget(GetWorld(), ItemBlueprint));
				NewSlotWidget->AddItemWidget(NewItemWidget);
				NewItemWidget->SetRepresentedItem(Item);
			}
		}
	}
}

void UInventoryVisualizerWidget::OnCloseButtonClicked()
{
	if (CurrentHeldItem != nullptr)
		return;
	
	Hide();
}

FReply UInventoryVisualizerWidget::KeyboardShortcutHideInventoryScreen(const FKeyEvent& InKeyEvent)
{
	if (State == Hidden)
		return FReply::Unhandled();
	
	// TODO: Explore getting the keys to check for from the input here specifically. It seems possible for the player input but not here? Who designed this.
	if (InKeyEvent.GetKey() == EKeys::I || InKeyEvent.GetKey() == EKeys::E)
	{
		if (GetWorld() == nullptr)
			return FReply::Unhandled();
		
		if(GetWorld()->GetFirstPlayerController() == nullptr)
			return FReply::Unhandled();

		if (CurrentHeldItem != nullptr)
			return FReply::Unhandled();

		Hide();
		
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

FReply UInventoryVisualizerWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	// TODO: This doesn't work when the component isnt focused. That seems wrong.
	
	return KeyboardShortcutHideInventoryScreen(InKeyEvent);
}
