#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "UnrealCraftItemInfo.generated.h"

/**
 * Info about an item that can be placed into an inventory.
 */
USTRUCT()
struct FUnrealCraftItemInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FString ItemName = "Unnamed Item";

	UPROPERTY(EditAnywhere)
	int8 MaxStackSize = 64;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> ItemThumbnail;

	UPROPERTY(EditAnywhere)
	EBlock BlockType = EBlock::Null;
};
