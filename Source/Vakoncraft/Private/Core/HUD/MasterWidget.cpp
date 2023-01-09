// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/HUD/MasterWidget.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"

void UMasterWidget::HighlightToolbarCell(int32 Index)
{
	auto Widgets = ToolbarSelectedCells->GetAllChildren();
	check(Index * 2 < Widgets.Num())
	Widgets[Index * 2]->SetVisibility(ESlateVisibility::Visible); // *2 - to skip 'Spacer'
}

void UMasterWidget::UnHighlightToolbarCell(int32 Index)
{
	auto Widgets = ToolbarSelectedCells->GetAllChildren();
	check(Index * 2 < Widgets.Num())
	Widgets[Index * 2]->SetVisibility(ESlateVisibility::Hidden); // *2 - to skip 'Spacer'
}

void UMasterWidget::UnHighlightAllToolbarCells()
{
	for (int32 i = 0; i < 9; ++i)
	{
		UnHighlightToolbarCell(i);
	}
}
