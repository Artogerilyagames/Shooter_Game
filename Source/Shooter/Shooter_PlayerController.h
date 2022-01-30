// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Shooter_PlayerController.generated.h"


/**
 * 
 */
UCLASS()
class SHOOTER_API AShooter_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AShooter_PlayerController();
protected:
	
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Widgets, meta = (AllowPrivateAccess = "true"));
	TSubclassOf<class UUserWidget> HUDOverlayClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category =Widgets, meta = (AllowPrivateAccess = "true"));
	UUserWidget* HUDOverlay;
	
};
