# UnrealCraft

![GIF of playing game](https://raw.githubusercontent.com/giodestone/unrealcraft/main/Images/GIF1.gif)

A voxel game made in Unreal Engine 5 using C++, aiming to replicate basic Minecraft functionality. More details about implementation can be found in subsequent sections.

Features:
* Procedurally generated finite voxel world which can be modified.
* Basic inventory system which allows you to move items between inventories.
* Character with controls.
* Programmed with C++, excluding scene and blueprint setup.
* Utilizes DataAssets and other Unreal specific features to make adding gameplay easy.

## How To Play

[Download latest release for your platform here](https://github.com/giodestone/unrealcraft/releases).

### Controls

* W, A, S, D - Move character.
* Space - Jump.
* E, I - Open/close inventory
* Left Click - Place Block
* Right Click - Remove block


## Technical Design

![Image of generated landscape.](https://raw.githubusercontent.com/giodestone/unrealcraft/main/Images/Image1.jpg)

The goal of this project is to ultimately try to replicate basic Minecraft Survival inventory management, block modification, day/night cycle, sounds using C++ and Unreal Engine features to ease addition of new gameplay elements, such as new blocks or items.

### World Generation.

The `AVoxelWorld` class is the 'manager' class of all chunks (`ABaseChunk` - potentially for future LOD chunks, like the distant horizons mod). A world of set size is generated with `AChunk::GenerateBlocks`. Perlin noise is used to generate hills. Afterwards, the topsoil is placed.

### Chunk Rendering

Chunks use `UProceduralMeshComponent` to render themselves. Vertices, UV, etc. are determined at runtime. Basic optimization is performed to reduce the number of vertices to 'squares'.

When a player modifies the chunk, the mesh is reconstructed. Provisionally, the chunks are 32x32x32 to guarantee good performance upon modification.

The textures are stored inside of a `Texture2DArray` which can have new blocks added easily. `EBlock` is the backing enum for all blocks.

### Inventory System

![Image of generated landscape.](https://raw.githubusercontent.com/giodestone/unrealcraft/main/Images/Image3.jpg)

The inventory system uses `IInventoryInterface` as the base for all inventory classes `Inventory` and `PlayerInventory` to ensure easy interoperability between inventories and easy addition of new types/logic. These classes are outside of the Unreal managed space and use smart pointers to guarantee safety. All inventories are stored inside of `InventoryDatabase`, a `TMap` backed data structure.

The inventory is visualized using `UInventoryVisualizerWidget`. It is ultimately responsible for displaying the player inventory, and/or the world inventory. This uses a `UInventorySlotWidget` for 'slots', and `UInventoryItemWidget` to represent the items inside of the inventory.

`UItemInfoDatabase` (a `UDataAsset`) contains `FUnrealCraftItemInfo`, which are defined in engine (inside Content/Data) to add new items.

### Use of Unreal Structure (GameState, GUI, GameMode)
The inventory info (`InventoryDatabase`) and useful references (such as to the `PlayerInventory`) are stored in `AVoxelGameState`.

The `APlayerHUD` contains all initialization logic and references to core GUI elements, such as the `UInventoryVisualizerWidget`.

The `AVoxelGameMode` contains logic to add a player inventory once the player 'joins' (this is not a multiplayer title).

### Code Practice
The [Epic Games Coding Standard](https://dev.epicgames.com/documentation/en-us/unreal-engine/epic-cplusplus-coding-standard-for-unreal-engine) was used throughout. In assets, Blueprints of C++ classes (excluding widgets) have a `BP_`` prefix.

Widgets use `W_` prefix and C++ classes contain a `Widget` suffix.

![Image of generated landscape.](https://raw.githubusercontent.com/giodestone/unrealcraft/main/Images/Image2.jpg)