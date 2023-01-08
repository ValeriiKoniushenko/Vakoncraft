// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Character/MainCharacter.h"

#include "Core/Character/MainPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "Core/WorldGenerator/WorldGenerator.h"
#include "Kismet/GameplayStatics.h"

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
	SpringArmComponent->AttachToComponent(RootComponent, {EAttachmentRule::KeepRelative, false});

	//	Camera:
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	check(CameraComponent);
	CameraComponent->AttachToComponent(SpringArmComponent, {EAttachmentRule::KeepRelative, false},
	                                   USpringArmComponent::SocketName);
	CameraComponent->FieldOfView = 120.f;
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->Activate();

	//========Setup=========
	//	Movement Component:
	UCharacterMovementComponent* CurrentMovementComponent = GetCharacterMovement();
	check(CurrentMovementComponent);
	CurrentMovementComponent->AirControl = 0.5f;
	CurrentMovementComponent->JumpZVelocity = 480.f;

}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//========Setup=========
	//	WorldGenerator:
	check(WorldGeneratorClass);
	AActor* WhatFound = UGameplayStatics::GetActorOfClass(GetWorld(), WorldGeneratorClass);
	check(WhatFound);
	WorldGenerator = Cast<AWorldGenerator>(WhatFound);
	check(WorldGenerator);
}

FHitResult AMainCharacter::LineTraceFromCamera() const
{
	FHitResult Hit;

	FVector TraceStart = SpringArmComponent->GetRelativeLocation() + GetActorLocation();
	FVector TraceEnd = SpringArmComponent->GetRelativeLocation() + GetActorLocation() + CameraComponent->
		GetForwardVector() * 1000.0f;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_WorldStatic, QueryParams);

	return Hit;
}

void AMainCharacter::LeftMouseAction()
{
	FHitResult Hit = LineTraceFromCamera();

	DrawDebugLine(GetWorld(), Hit.TraceStart, Hit.TraceEnd, Hit.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0,
	              10.0f);
}

void AMainCharacter::RightMouseAction()
{
	FHitResult Hit = LineTraceFromCamera();

	DrawDebugLine(GetWorld(), Hit.TraceStart, Hit.TraceEnd, Hit.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0,
				  10.0f);

		FTransform Transform;
		Transform.SetLocation(Hit.ImpactPoint);
		WorldGenerator->Stone->AddInstance(Transform);
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
	Input->BindAction("LeftMouseAction", IE_Pressed, CurrentController, &AMainPlayerController::LeftMouseAction);
	Input->BindAction("RightMouseAction", IE_Pressed, CurrentController, &AMainPlayerController::RightMouseAction);
}
