// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/WorldGenerator/WorldGenerator.h"

#include "Core/WorldGenerator/Perlin.h"
#include "Components/InstancedStaticMeshComponent.h"

#pragma warning(push)
#pragma warning(disable: 4996)

#define CREATE_BLOCK_MODEL(Block) \
	Block = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT(#Block)); \
	check(Block); \
	Block->SetMobility(EComponentMobility::Static); \
	Block->SetCollisionProfileName("Block All"); \
	SetRootComponent(Block);

const uint8 Chunk::Width = 16u;
const uint8 Chunk::Height = 255u;

Chunk::Chunk()
{
	Data.SetNum(Width);
	for (auto& Z : Data)
	{
		Z.SetNum(Height);
		for (auto& Y : Z)
		{
			Y.SetNum(Width);
		}
	}
}


AWorldGenerator::AWorldGenerator()
{
	PrimaryActorTick.bCanEverTick = false;

	CREATE_BLOCK_MODEL(Grass);
	CREATE_BLOCK_MODEL(Dirt);
	CREATE_BLOCK_MODEL(Stone);
	CREATE_BLOCK_MODEL(Bedrock);
}

void AWorldGenerator::Generate()
{
	SetupChunksPositions();

	const FVector2D Size(CountOfChunksX * Chunk::Width, CountOfChunksY * Chunk::Width);

	for (int32 i = 0; i < CountOfChunksY; ++i)
	{
		for (int32 j = 0; j < CountOfChunksX; ++j)
		{
			for (int32 z = 0; z < Chunk::Width; ++z)
			{
				for (int32 x = 0; x < Chunk::Width; ++x)
				{
					float Y = db::perlin(static_cast<float>(x + Chunk::Width * j) / Size.X,
					                     static_cast<float>(z + Chunk::Width * i) / Size.Y) * Amplitude;
					Y /= BlockSize;
					Y = FMath::Floor(Y);
					Y += NormalHeight;

					Chunks[i][j].Data[z][Y][x].Type = Block::EType::Grass;

					float Temp = Y;
					const float DirtHeight = FMath::RandRange(1,4);
					const float BedrockHeight = FMath::RandRange(1,3);
					const float StoneHeight = Temp - DirtHeight - BedrockHeight - 1;
					
					for (int32 k = 0; k < DirtHeight; ++k)
					{
						Chunks[i][j].Data[z][--Temp][x].Type = Block::EType::Dirt;
					}

					for (int32 k = 0; k < StoneHeight; ++k)
					{
						Chunks[i][j].Data[z][--Temp][x].Type = Block::EType::Stone;
					}

					for (int32 k = 0; k < BedrockHeight; ++k)
					{
						Chunks[i][j].Data[z][--Temp][x].Type = Block::EType::Bedrock;
					}
				}
			}
		}
	}
}

void AWorldGenerator::BeginPlay()
{
	Super::BeginPlay();

	Generate();
	RedrawMap();
}

void AWorldGenerator::RedrawMap()
{
	for (int32 i = 0; i < CountOfChunksY; ++i)
	{
		for (int32 j = 0; j < CountOfChunksX; ++j)
		{
			RedrawOneChunk(i, j);
		}
	}
}

void AWorldGenerator::RedrawOneChunk(int32 i, int32 j)
{
	for (int32 z = 0; z < Chunk::Width; ++z)
	{
		for (int32 y = 0; y < Chunk::Height; ++y)
		{
			for (int32 x = 0; x < Chunk::Width; ++x)
			{
				FTransform Transform;
				Transform.SetLocation(FVector(
					(x + Chunk::Width * j) * BlockSize,
					(z + Chunk::Width * i) * BlockSize,
					y * BlockSize
				));

				Chunks[i][j].Data[z][y][x].InstanceIndex = AddBlockInstance(Chunks[i][j].Data[z][y][x].Type, Transform);
			}
		}
	}
}

void AWorldGenerator::SetupChunksPositions()
{
	Chunks.Empty();
	Chunks.SetNum(CountOfChunksY);
	for (int32 i = 0; i < CountOfChunksY; ++i)
	{
		Chunks[i].SetNum(CountOfChunksX);
		for (int32 j = 0; j < CountOfChunksX; ++j)
		{
			Chunks[i][j].Position.X = j;
			Chunks[i][j].Position.Y = i;
		}
	}
}

int32 AWorldGenerator::AddBlockInstance(Block::EType Type, const FTransform& Transform)
{
	switch (Type)
	{
	case Block::EType::Grass:
		return Grass->AddInstance(Transform);
	case Block::EType::Dirt:
		return Dirt->AddInstance(Transform);
	case Block::EType::Stone:
		return Stone->AddInstance(Transform);
	case Block::EType::Bedrock:
		return Bedrock->AddInstance(Transform);
	}
	return 0;
}

#undef CREATE_BLOCK_MODEL
#pragma warning(pop)
