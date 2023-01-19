// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Core/Inventory/Inventory.h"
#include "MainCharacter.generated.h"

class AWorldGenerator;
class UCameraComponent;
class USpringArmComponent;
class USpringArmComponent;
class UMasterWidget;

UCLASS()
class VAKONCRAFT_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();

	virtual void LeftMouseAction();
	virtual void RightMouseAction();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* Input) override;
	virtual void NextToolBarTool(float Value);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=HUD)
	TSubclassOf<UMasterWidget> MasterWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=BlockGrabber)
	float ArmLength = 600.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
	TSubclassOf<AWorldGenerator> WorldGeneratorClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	AWorldGenerator* WorldGenerator;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Toolbar)
	int32 MaxToolbarCellIndex = 8;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Toolbar)
	int32 MinToolbarCellIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Toolbar)
	int32 ToolbarCellIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Toolbar)
	UInventory* Inventory;

protected:
	virtual void BeginPlay() override;
	FHitResult LineTraceFromCamera() const;
	void UpdateToolbarCellsHighlight();

protected:
	UMasterWidget* MasterWidget;
};
