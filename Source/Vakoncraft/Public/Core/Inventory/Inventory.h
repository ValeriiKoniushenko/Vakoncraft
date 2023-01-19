#pragma once

#include "CoreMinimal.h"
#include "Core/WorldGenerator/Block.h"
#include "Components/ActorComponent.h"
#include "Inventory.generated.h"

class UTexture2D;

UCLASS()
class UInventory : public UActorComponent
{
	GENERATED_BODY()

public:
	void SetSize(int32 Rows, int32 Cols);
	int32 GetRows() const;
	int32 GetCols() const;
	TArray<FCellData>& operator[](uint32 RowIndex);
	const TArray<FCellData>& operator[](uint32 RowIndex) const;
	TArray<TArray<FCellData>>& Data();
	const TArray<TArray<FCellData>>& Data() const;

protected:
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Textures)
	// TArray<UTexture2D*> Textures;
	
private:
	// TArray<TArray<FCellData>> Cells;
};
