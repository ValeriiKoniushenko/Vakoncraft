// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class VAKONCRAFT_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookAround(float Value);
	void LookUp(float Value);
	void Jump();
	void LeftMouseAction();
	void RightMouseAction();
};
