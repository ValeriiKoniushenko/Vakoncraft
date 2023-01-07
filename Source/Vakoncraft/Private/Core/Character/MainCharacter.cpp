// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Character/MainCharacter.h"

#include "Core/Character/MainPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//========Creating=========
	//	SpringArmComponent:
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	check(SpringArmComponent);
	SpringArmComponent->TargetArmLength = 0.f;
	SpringArmComponent->SetRelativeLocation(FVector(0.f, 0.f, 80.f));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->AttachTo(RootComponent);

	//	Camera:
	UCameraComponent* CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	check(CameraComponent);
	CameraComponent->AttachTo(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->FieldOfView = 120.f;
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->Activate();

	//========Setup=========
	//	Movement Component:
	UCharacterMovementComponent* CurrentMovementComponent = GetCharacterMovement();
	check(CurrentMovementComponent);
	CurrentMovementComponent->AirControl = 0.7f;
	CurrentMovementComponent->JumpZVelocity = 480.f;
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
