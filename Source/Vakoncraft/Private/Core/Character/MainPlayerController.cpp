// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Character/MainPlayerController.h"

#include "GameFramework/Character.h"
#include "Core/Character/MainCharacter.h"

void AMainPlayerController::MoveForward(float Value)
{
	ACharacter* CurrentCharacter = GetCharacter();
	check(CurrentCharacter);
	CurrentCharacter->AddMovementInput(CurrentCharacter->GetActorForwardVector(), Value, true);
}

void AMainPlayerController::MoveRight(float Value)
{
	ACharacter* CurrentCharacter = GetCharacter();
	check(CurrentCharacter);
	CurrentCharacter->AddMovementInput(CurrentCharacter->GetActorRightVector(), Value, true);
}

void AMainPlayerController::LookAround(float Value)
{
	ACharacter* CurrentCharacter = GetCharacter();
	check(CurrentCharacter);
	CurrentCharacter->AddControllerYawInput(Value);
}

void AMainPlayerController::LookUp(float Value)
{
	ACharacter* CurrentCharacter = GetCharacter();
	check(CurrentCharacter);
	CurrentCharacter->AddControllerPitchInput(Value);
}

void AMainPlayerController::Jump()
{
	ACharacter* CurrentCharacter = GetCharacter();
	check(CurrentCharacter);
	CurrentCharacter->Jump();
}

void AMainPlayerController::LeftMouseAction()
{
	AMainCharacter* CurrentCharacter = Cast<AMainCharacter>(GetCharacter());
	check(CurrentCharacter);
	CurrentCharacter->LeftMouseAction();
}

void AMainPlayerController::RightMouseAction()
{
	AMainCharacter* CurrentCharacter = Cast<AMainCharacter>(GetCharacter());
	check(CurrentCharacter);
	CurrentCharacter->RightMouseAction();
}

void AMainPlayerController::NextToolBarTool(float Value)
{
	AMainCharacter* CurrentCharacter = Cast<AMainCharacter>(GetCharacter());
	check(CurrentCharacter);
	CurrentCharacter->NextToolBarTool(Value);
}
