// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MainGameModeBase.h"
#include "Core/Character/MainCharacter.h"
#include "Core/Character/MainPlayerController.h"
#include "Core/HUD/MainHUD.h"
#include "Core/MainPlayerState.h"
#include "Core/MainGameStateBase.h"

AMainGameModeBase::AMainGameModeBase()
{
	GameStateClass = AMainGameStateBase::StaticClass();
	HUDClass = AMainHUD::StaticClass();
	PlayerControllerClass = AMainPlayerController::StaticClass();
	PlayerStateClass = AMainPlayerState::StaticClass();
	DefaultPawnClass = AMainCharacter::StaticClass();
}
