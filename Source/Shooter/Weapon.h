// Fill out your copyright notice in the Description page of Project Settings.


#pragma once
#include "CoreMinimal.h"
#include "Item.h"
#include "AmmoType.h"
#include "Engine/DataTable.h"
#include "WeaponType.h"
#include "Weapon.generated.h"


USTRUCT(BlueprintType)
struct FWeaponDataTable : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAmmoType AmmoType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeaponAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MagazineCapacity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundCue* PickupSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* EquipSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* ItemMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* InventoryIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* AmmoIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ClipBoneName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ReloadMontageSection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsMiddle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsButtom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsTop;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AutoFireRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutomatic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeadShotDamage;


	

	
};

/**
 * 
 */
UCLASS()
class SHOOTER_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	AWeapon();
	virtual void Tick(float DeltaTime) override;
	
	
protected:
	
	void StopFalling();

	
	virtual void OnConstruction(const FTransform& Transform) override;
	void FinishMovingSlide();
	void UpdateSlideDisplacement();
    	
private:
	FTimerHandle ThroWeaponTimer;
	float ThrowWeaponTime;
	bool bFalling;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Weapon Properties", meta = (AllowPrivateAccess = "true"));
	int32 Ammo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Weapon Properties", meta = (AllowPrivateAccess = "true"));
	int32 MagazineCapacity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Weapon Properties", meta = (AllowPrivateAccess = "true"));
	EWeaponType WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Weapon Properties", meta = (AllowPrivateAccess = "true"));
	EAmmoType AmmoType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Weapon Properties", meta = (AllowPrivateAccess = "true"));
	FName ReloadMontageSection;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Weapon Properties", meta = (AllowPrivateAccess = "true"));
	bool bMovingClip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Weapon Properties", meta = (AllowPrivateAccess = "true"));
	FName ClipBoneName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= DataTable, meta = (AllowPrivateAccess = "true"));
	UDataTable* WeaponDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= DataTable, meta = (AllowPrivateAccess = "true"));
	UTexture2D* CrosshairsMiddle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= DataTable, meta = (AllowPrivateAccess = "true"));
	UTexture2D* CrosshairsLeft;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= DataTable, meta = (AllowPrivateAccess = "true"));
	UTexture2D* CrosshairsRight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= DataTable, meta = (AllowPrivateAccess = "true"));
	UTexture2D* CrosshairsButtom;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= DataTable, meta = (AllowPrivateAccess = "true"));
	UTexture2D* CrosshairsTop;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= DataTable, meta = (AllowPrivateAccess = "true"));
	float AutoFireRate;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= DataTable, meta = (AllowPrivateAccess = "true"));
	class UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= DataTable, meta = (AllowPrivateAccess = "true"));
	USoundCue* FireSound;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Pistol, meta = (AllowPrivateAccess = "true"));
	float SlideDisplacement;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Pistol, meta = (AllowPrivateAccess = "true"));
	UCurveFloat* SlideDisplacementCurve;
	FTimerHandle SliderTimer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Pistol, meta = (AllowPrivateAccess = "true"));
	float SlideDisplacementTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category= Pistol, meta = (AllowPrivateAccess = "true"));
	bool bMovingSlide;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Pistol, meta = (AllowPrivateAccess = "true"));
	float MaxSlideDisplacement;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Pistol, meta = (AllowPrivateAccess = "true"));
	float MaxRecoilRotation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Pistol, meta = (AllowPrivateAccess = "true"));
	float RecoilRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Pistol, meta = (AllowPrivateAccess = "true"))
	bool bAutomatic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category= "Weapon Properties", meta = (AllowPrivateAccess = "true"));
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category= "Weapon Properties", meta = (AllowPrivateAccess = "true"));
	float HeadShotDamage;




	
	
public:
	void ThrowWeapon();
	FORCEINLINE int32 GetAmmo() const {return Ammo;}
	FORCEINLINE int32 GetMagazineCapacity() const {return MagazineCapacity;}
	void DecrementAmmo();
	FORCEINLINE EWeaponType GetWeaponType() const {return WeaponType;}
	FORCEINLINE EAmmoType GetAmmoType() const {return AmmoType;}
	FORCEINLINE FName GetReloadMontageSection() const {return ReloadMontageSection;}
	FORCEINLINE void SetReloadMontageSection(FName Name){ReloadMontageSection = Name;}
	FORCEINLINE FName GetClipBoneName() const {return ClipBoneName;}
	FORCEINLINE void SetClipBoneName(FName Name) {ClipBoneName = Name;}
	FORCEINLINE float GetAutoFireRate() const {return AutoFireRate;}
	FORCEINLINE UParticleSystem* GetMuzzleFlash() const {return MuzzleFlash;}
	FORCEINLINE USoundCue* GetFireSound() const {return FireSound;}
	void StartSlideTimer();
	FORCEINLINE bool GetAutomatic() const {return bAutomatic;}
	FORCEINLINE float GetDamage() const {return Damage;}
	FORCEINLINE float GetHeadShotDamage() const {return HeadShotDamage;}

	


	void ReloadAmmo(int32 Amount);
	FORCEINLINE void SetMovingClip(const bool Move){bMovingClip = Move;}
	bool ClipIsFull();

	


	
	
};
