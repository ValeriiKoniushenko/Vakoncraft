// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldGenerator.generated.h"

class UInstancedStaticMeshComponent;


struct Block
{
	enum class EType
	{
		None,
		Dirt,
		Grass,
		Stone,
		Bedrock
	};

	EType Type = EType::None;
};

struct Chunk
{
	Chunk();
	static const uint8 Width;
	static const uint8 Height;
	FVector2D Position;
	TArray<TArray<TArray<Block>>> Data;
};

UCLASS()
class VAKONCRAFT_API AWorldGenerator : public AActor
{
	GENERATED_BODY()

public:
	AWorldGenerator();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Generator)
	int32 CountOfChunksX = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Generator)
	int32 CountOfChunksY = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Generator)
	float Amplitude = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Generator)
	float SubAmplitude = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Generator, Category=Generator)
	int32 BlockSize = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Generator, Category=Generator)
	uint8 MinHeight = 3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Generator, Category=Generator)
	uint8 MaxHeight = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin=3, UIMin=3, ClampMax=200, UIMax=200), Category=Generator)
	uint8 NormalHeight = 20;

	void Generate();

	FVector GetActorSpawnPlace() const;

	void SpawnBlock(Block::EType Type, const FTransform& Transform);

protected:
	virtual void BeginPlay() override;
	void RedrawMap();
	void RedrawOneChunk(int32 i, int32 j);
	void SetupChunksPositions();
	int32 AddBlockInstance(Block::EType Type, const FTransform& Transform);

protected:
	TArray<TArray<Chunk>> Chunks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInstancedStaticMeshComponent* Grass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInstancedStaticMeshComponent* Dirt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInstancedStaticMeshComponent* Stone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInstancedStaticMeshComponent* Bedrock;
};
