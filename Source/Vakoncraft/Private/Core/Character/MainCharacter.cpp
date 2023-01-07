// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Character/MainCharacter.h"

#include "Core/Character/MainPlayerController.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* Input)
{
	Super::SetupPlayerInputComponent(Input);

	AMainPlayerController* CurrentController = GetController<AMainPlayerController>();
	check(CurrentController);

	Input->BindAxis("MoveForward", CurrentController, &AMainPlayerController::MoveForward);
	Input->BindAxis("MoveRight", CurrentController, &AMainPlayerController::MoveRight);
	Input->BindAxis("LookAround", CurrentController, &AMainPlayerController::LookAround);
	Input->BindAxis("LookUp", CurrentController, &AMainPlayerController::LookUp);
	Input->BindAction("Jump", IE_Pressed, CurrentController, &AMainPlayerController::Jump);
}
