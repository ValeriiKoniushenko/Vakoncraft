#include "Core/Inventory/Inventory.h"

/*
void UInventory::SetSize(int32 Rows, int32 Cols)
{
	Cells.SetNum(Rows);
	for (auto& Cell : Cells)
	{
		Cell.SetNum(Cols);
	}
}

int32 UInventory::GetRows() const
{
	return Cells.Num();
}

int32 UInventory::GetCols() const
{
	if (Cells.Num() > 0)
	{
		return Cells[0].Num();
	}

	return 0;
}

TArray<FCellData>& UInventory::operator[](uint32 RowIndex)
{
	return Cells[RowIndex];
}

const TArray<FCellData>& UInventory::operator[](uint32 RowIndex) const
{
	return Cells[RowIndex];
}

TArray<TArray<FCellData>>& UInventory::Data()
{
	return Cells;
}

const TArray<TArray<FCellData>>& UInventory::Data() const
{
	return Cells;
}
*/
