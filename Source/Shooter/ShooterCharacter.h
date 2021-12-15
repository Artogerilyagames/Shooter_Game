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
	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation);
	void AimingButtonPressed();
	void AimingButtonReleased();
	void CameraInterpZoom(float DeltaTime);
	


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





public:
	FORCEINLINE USpringArmComponent* GetCameraBoom() const {return CameraBoom;}
	FORCEINLINE UCameraComponent* GetFollowCamera() const {return FollowCamera;}
	FORCEINLINE bool GetAiming() const {return bAiming;}
	

};
