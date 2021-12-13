// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "ShooterCharacter.generated.h"


UCLASS()
class SHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();
// Shooter Game
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void FireWeapon();
	void Punch();
	


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward (float Value);
	void MoveRight(float Value);

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Camera, meta =(AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Camera, meta =(AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Camera,meta =(AllowPrivateAccess = "true"))
	float BaseTurnRate;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Camera,meta =(AllowPrivateAccess = "true"))
	float BaseLookUpRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat,meta =(AllowPrivateAccess = "true"))
	class USoundCue* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat,meta =(AllowPrivateAccess = "true"))
	class UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat,meta =(AllowPrivateAccess = "true"))
	class UAnimMontage* HipFireMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat,meta =(AllowPrivateAccess = "true"))
	class UAnimMontage* PunchMontage;


public:
	FORCEINLINE USpringArmComponent* GetCameraBoom() const {return CameraBoom;}
	FORCEINLINE UCameraComponent* GetFollowCamera() const {return FollowCamera;}

};
