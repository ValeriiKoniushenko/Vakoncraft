// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

class AWorldGenerator;
class UCameraComponent;
class USpringArmComponent;
class USpringArmComponent;

UCLASS()
class VAKONCRAFT_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
	TSubclassOf<AWorldGenerator> WorldGeneratorClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	AWorldGenerator* WorldGenerator;
	
	UFUNCTION(BlueprintCallable)
	virtual void LeftMouseAction();
	virtual void RightMouseAction();

protected:
	virtual void BeginPlay() override;
	FHitResult LineTraceFromCamera() const;
public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* Input) override;
};
