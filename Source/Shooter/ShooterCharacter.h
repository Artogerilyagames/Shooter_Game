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
	void Turn(float Value);
	void LookUp(float Value);
	void FireWeapon();
	void Punch();
	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation);
	void AimingButtonPressed();
	void AimingButtonReleased();
	void CameraInterpZoom(float DeltaTime);
	void SetLookRates();
	void CalculateCrosshairSpread(float DeltaTime);
	
	void FireButtonPressed();
	void FireButtonReleased();
	void StartFireTimer();
	UFUNCTION()
	void AutoFireReset();

	
	void FinishCrosshairBulletFire();
	void StartCrosshairBulletfire();
	


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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Camera,meta =(AllowPrivateAccess = "true"))
	float HipTurnRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Camera,meta =(AllowPrivateAccess = "true"))
	float HipLookUpRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Camera,meta =(AllowPrivateAccess = "true"))
	float AimingTurnRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Camera,meta =(AllowPrivateAccess = "true"))
	float AimingLookUpRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Camera,meta =(AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseHipTurnRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Camera,meta =(AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseHipLookUpRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Camera,meta =(AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseAimingTurnRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Camera,meta =(AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseAimingLookUpRate;
	



	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat,meta =(AllowPrivateAccess = "true"))
	class USoundCue* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat,meta =(AllowPrivateAccess = "true"))
	class UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat,meta =(AllowPrivateAccess = "true"))
	class UAnimMontage* HipFireMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat,meta =(AllowPrivateAccess = "true"))
	class UAnimMontage* PunchMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat,meta =(AllowPrivateAccess = "true"))
	class USoundCue* PunchSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat,meta =(AllowPrivateAccess = "true"))
	UParticleSystem* ImpactParticles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat,meta =(AllowPrivateAccess = "true"))
	UParticleSystem* BeamParticles;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Combat,meta =(AllowPrivateAccess = "true"))
	bool bAiming;
    // Zoomed Field of View
	float CameraDefaultFOV;
	float CameraZoomedFOV;
	float CameraCurrentFOV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat,meta =(AllowPrivateAccess = "true"))
	float ZoomInterpSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Crosshairs,meta =(AllowPrivateAccess = "true"))
	float CrosshairSpreadMultiplier;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Crosshairs,meta =(AllowPrivateAccess = "true"))
	float CrosshairVelocityFactor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Crosshairs,meta =(AllowPrivateAccess = "true"))
	float CrosshairInAirFactor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Crosshairs,meta =(AllowPrivateAccess = "true"))
	float CrosshairAimFactor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Crosshairs,meta =(AllowPrivateAccess = "true"))
	float CrosshairShootingFactor;

	float ShootTimeDuration;
	bool bFiringBullet;
	FTimerHandle CrosshairShootTimer;

	float TurnScaleFactor;
	float LookUpScaleFactor;

	bool bFireButtonPressed;
	bool bShouldFire;
	float AutomaticFireRate;
	FTimerHandle AutoFireTimer;





public:
	FORCEINLINE USpringArmComponent* GetCameraBoom() const {return CameraBoom;}
	FORCEINLINE UCameraComponent* GetFollowCamera() const {return FollowCamera;}
	FORCEINLINE bool GetAiming() const {return bAiming;}
	UFUNCTION(BlueprintCallable)
	float GetCrosshairSpreadmultiplier() const;
	

};
