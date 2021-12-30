// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ammo.h"
#include "Weapon.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "AmmoType.h"
#include "ShooterCharacter.generated.h"

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccpied"),
	ECS_FireTimerInProgress UMETA(DisplayName = "FireTimerInProgress"),
	ECS_Reloading UMETA(DisplayName = "Reloading"),

	ECS_MAX UMETA(DisplayName= "DefaultMAX")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquipItemDelegate,int32, CurrentSlotIndex, int32, NewSlotindex);


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
	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation) const;
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
	/** Line Trace for Item under the crosshairs*/
	bool TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation) const;
	void TraceForItems();
	void ReloadingIcon();
	void UnreloadingIcon();

	AWeapon* SpawnDefaultWeapon();
	void EquipWeapon(AWeapon* WeaponToEquip);

	
	void FinishCrosshairBulletFire();
	void StartCrosshairBulletFire();
	void DropWeapon();
	void SelectButtonPressed();
	void SelectButtonReleased();
	
	void MoveForward (float Value);
	void MoveRight(float Value);
	void SwapWeapon(AWeapon* WeaponToSwap);
	void InitializeAmmoMap();
	bool WeaponHasAmmo();
	/**FireWeapon function*/
	void PlayFireSound();
	void SendBullet();
	void PlayGunfireMontage();
	void ReloadButtonPressed();
	void ReloadWeapon();
	bool CarryingAmmo();
	void ShowCrosshairs();
	void HideCrosshairs();
	
	UFUNCTION(BlueprintCallable)
	void GrabClip();
	UFUNCTION(BlueprintCallable)
	void ReleasedClip();
	void CrouchButtonPressed();
	void ProneButtonPressed();
	void InterpCapsuleHalfHeight(float DeltaTime) const;

	virtual void Jump() override;

	void Aim();
	void StopAiming();
	
	void PickupAmmo(class AAmmo* Ammo);

	void ResetPickupSoundTimer();
	void ResetEquipSoundTimer();

	void FKeyPressed();
	void OneKeyPressed();
	void TwoKeyPressed();
	void ThreeKetPressed();
	void FourKeyPressed();
	void FiveKeyPressed();

	void ExchangeInventoryItems(int32 CurrentItemIndex, int32 NewItemIndex );
	


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat,meta =(AllowPrivateAccess = "true"))
	bool bAiming;
	// Zoomed Field of View
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat,meta =(AllowPrivateAccess = "true"))
	float CameraDefaultFOV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat,meta =(AllowPrivateAccess = "true"))
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
	// Screen HUD
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* PickupWidget;

	float ShootTimeDuration;
	bool bFiringBullet;
	FTimerHandle CrosshairShootTimer;

	float TurnScaleFactor;
	float LookUpScaleFactor;

	bool bFireButtonPressed;
	bool bShouldFire;
	float AutomaticFireRate;
	FTimerHandle AutoFireTimer;
	bool bShouldTraceForItems;
	int8 OverlappedItemCount;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Items, meta =(AllowPrivateAccess = "true"));
	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	class AItem* TraceHitItemLastFrame;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Combat, meta =(AllowPrivateAccess = "true"));
	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	AWeapon* EquippedWeapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Combat, meta =(AllowPrivateAccess = "true"));
	TSubclassOf<AWeapon> DefaultWeaponClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Combat, meta =(AllowPrivateAccess = "true"));
	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	AItem* TraceHitItem;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Item, meta =(AllowPrivateAccess = "true"));
	float CameraInterpDistance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Item, meta =(AllowPrivateAccess = "true"));
	float CameraInterpElevation;


	
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Combat, meta =(AllowPrivateAccess = "true"));
	TMap<EAmmoType, int32> AmmoMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat, meta =(AllowPrivateAccess = "true"));
	int32 Starting9mmAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat, meta =(AllowPrivateAccess = "true"));
	int32 StartingARAmmo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Combat, meta =(AllowPrivateAccess = "true"));
	// ReSharper disable once UnrealHeaderToolParserError
	ECombatState CombatState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat, meta = (AllowPrivateAccess = "true"));
	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	UAnimMontage* ReloadMontage;
	
	UFUNCTION(BlueprintCallable)
	void FinishReloading();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Combat, meta = (AllowPrivateAccess = "true"));
	FTransform ClipTransform;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Combat, meta = (AllowPrivateAccess = "true"));
	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	USceneComponent* HandSceneComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat,meta =(AllowPrivateAccess = "true"))
	bool bCrosshairsTriger;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Movement, meta =(AllowPrivateAccess = "true"))
	bool bCrouching;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Movement, meta =(AllowPrivateAccess = "true"))
	bool bProne;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Combat, meta = (AllowPrivateAccess = "true"));
	float BaseMovementSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Combat, meta = (AllowPrivateAccess = "true"));
	float CrouchMovementSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Combat, meta = (AllowPrivateAccess = "true"));
	float ProneMovementSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Movement, meta = (AllowPrivateAccess = "true"));
	float CurrentCapsuleHalfHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Movement, meta = (AllowPrivateAccess = "true"));
	float StandingCapsuleHalfHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Movement, meta = (AllowPrivateAccess = "true"));
	float CrouchingCapsuleHalfHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Movement, meta = (AllowPrivateAccess = "true"));
	float ProneCapsuleHalfHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Movement, meta = (AllowPrivateAccess = "true"));
	float BaseGroundFriction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Movement, meta = (AllowPrivateAccess = "true"));
	float CrouchingGroundFriction;

	bool bAimingButtonPressed;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Combat,meta =(AllowPrivateAccess = "true"))
	bool bReloadingIcon;

	FTimerHandle PickupSoundTimer;
	FTimerHandle EquipSoundTimer;
	bool bShouldPlayPickupSound;
	bool bShouldPlayEquipSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category= Items, meta = (AllowPrivateAccess = "true"))
	float PickupSoundResetTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category= Items, meta = (AllowPrivateAccess = "true"))
	float EquipSoundResetTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Inventory, meta = (AllowPrivateAccess = "true"))
	TArray<AItem*> Inventory;
	
	const int32 INVENTORY_CAPACITY{6};
	
    UPROPERTY(BlueprintAssignable, Category= Delegates,meta = (AllowPrivateAccess = "true"))
	FEquipItemDelegate EquipItemDelegate;



public:
	FORCEINLINE USpringArmComponent* GetCameraBoom() const {return CameraBoom;}
	FORCEINLINE UCameraComponent* GetFollowCamera() const {return FollowCamera;}
	FORCEINLINE bool GetAiming() const {return bAiming;}
	UFUNCTION(BlueprintCallable)
	float GetCrosshairSpreadmultiplier() const;
	FORCEINLINE int8 GetOverlappedItemCount() const {return OverlappedItemCount;}
	void IncrementOverlappedItemCount(int8 Amount);
	FVector GetCameraInterpLocation();

	FORCEINLINE ECombatState GetCombatState() const {return CombatState;}
	FORCEINLINE bool GetCrouching() const {return bCrouching;}
	FORCEINLINE bool GetProne() const {return bProne;}
	FORCEINLINE bool ShouldPlayPickupSound() const {return bShouldPlayPickupSound;}
	FORCEINLINE bool ShouldPlayEquipSound() const {return bShouldPlayEquipSound;}
	void StartPickupSoundTimer();
	void StartEquipSoundTimer();
	
	void GetPickupItem(AItem* Item);
	
	
};