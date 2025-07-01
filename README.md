# UnrealCraft

![GIF of playing game](https://raw.githubusercontent.com/giodestone/unrealcraft/main/Images/GIF1.gif)

A voxel game made in Unreal Engine 5 using C++, aiming to replicate basic Minecraft functionality. More details about implementation can be found in subsequent sections.

#### Features
* Procedurally generated finite voxel world which can be modified by the player.
* Basic inventory system which allows you to create chests, move items into them, and place items from your inventory into the world.
* First person character controller.
* Programmed with C++ exclusively.
* Utilizes DataAssets, UPROPERTY(), Blueprints to make adding gameplay addition easy.

## How To Play

[Download the latest release for your platform here](https://github.com/giodestone/unrealcraft/releases).

### Controls

* W, A, S, D - Move character
* Space - Jump
* E, I - Open/close inventory
* Left Click - Place Block
* Right Click - Remove block
* Mouse Scroll - Change selected block
* Alt+F4 - Exit Game

## Technical Design

![Image of generated landscape.](https://raw.githubusercontent.com/giodestone/unrealcraft/main/Images/Image1.jpg)

The goal of this project is to eventually try to replicate basic Minecraft Survival inventory management, block modification, day/night cycle, sounds using C++ and Unreal Engine features to ease the addition of new gameplay elements, such as new blocks or items. At present, it has world generation, building, and inventory management.

### World Generation.

The `AVoxelWorld` class is the 'manager' class of all chunks (`ABaseChunk` - potentially for future LOD chunks, like the distant horizons mod). A world of set size is generated with `AChunk::GenerateBlocks`. Perlin noise is used to generate hills. Afterwards, the topsoil is placed.

### Chunk Rendering

Chunks use `UProceduralMeshComponent` to render themselves. Vertices, UV, etc. are determined at runtime. Face merging was added to reduce the number of vertices.

When a player modifies the chunk, the mesh is reconstructed. Provisionally, the chunks are 32x32x32 to guarantee good performance upon modification.

The textures are stored inside of a `Texture2DArray` which can have new blocks added easily. `EBlock` is the backing enum for all blocks.

### Inventory System

![Image of generated landscape.](https://raw.githubusercontent.com/giodestone/unrealcraft/main/Images/Image3.jpg)

The inventory system uses `IInventoryInterface` as the base for all inventory classes `UInventory` and `UPlayerInventory` to ensure easy interoperability between inventories and easy addition of new types/logic. These classes derive from `UObject` to allow future interoperability with Blueprints. All inventories are stored inside of `UInventoryDatabase`, a `TMap` backed data structure.

The inventory is visualized using `UInventoryVisualizerWidget`. It is ultimately responsible for displaying the player inventory, and/or the world inventory. This uses a `UInventorySlotWidget` for 'slots', and `UInventoryItemWidget` to represent the items inside of the inventory.

`UItemInfoDatabase` (a `UDataAsset`) contains `FUnrealCraftItemInfo`, which are defined in engine (inside /Content/Data) to add new items.

`UPlayerHotbarWidget` visualises a portion of the player's inventory contents. `UHotbarCursorWidget` gives an animated white box which shows the player's currently selected hotbar slot.

### Use of Unreal Structure (GameState, GUI, GameMode)
The inventory info (`UInventoryDatabase`) and useful references (such as to the `UPlayerInventory`) are stored in `AVoxelGameState`.

The `APlayerHUD` contains all initialization logic and references to core GUI elements, such as the `UInventoryVisualizerWidget`.

The `AVoxelGameMode` contains logic to add a player inventory once the player 'joins' (this is not a multiplayer title).

### Code Practice
The [Epic Games Coding Standard](https://dev.epicgames.com/documentation/en-us/unreal-engine/epic-cplusplus-coding-standard-for-unreal-engine) was used throughout. In assets, Blueprints of C++ classes (excluding widgets) have a `BP_`` prefix.

UUserWidget Blueprints use a `W_` prefix and C++ classes deriving from `UUserWidget` contain a `Widget` suffix.

![Image of generated landscape.](https://raw.githubusercontent.com/giodestone/unrealcraft/main/Images/Image2.jpg)

<!-- ### Future Enhancements * The UInventoryDatabase class has become a monolith. The inventory visualisation should be split into its own class. * The block placement logic should be moved away from the player into another class, which can be used as a 'component'. * Inventory contents are stored in the memory exclusively. This is inefficient for inactive inventories. -->
