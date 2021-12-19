// Fill out your copyright notice in the Description page of Project Settings.


#include "Shooter_PlayerController.h"
#include "Blueprint/UserWidget.h"

AShooter_PlayerController::AShooter_PlayerController()
{
	
}

void AShooter_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	if(HUDOverlayClass)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayClass);
		if(HUDOverlay)
		{
			HUDOverlay->AddToViewport();
			HUDOverlay->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
