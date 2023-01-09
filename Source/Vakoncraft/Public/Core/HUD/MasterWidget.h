// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MasterWidget.generated.h"

class UImage;
class UHorizontalBox;

/**
 * 
 */
UCLASS()
class VAKONCRAFT_API UMasterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void HighlightToolbarCell(int32 Index);
	void UnHighlightToolbarCell(int32 Index);
	void UnHighlightAllToolbarCells();

protected:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UHorizontalBox* ToolbarSelectedCells;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UHorizontalBox* ToolbarUnselectedCells;
};
