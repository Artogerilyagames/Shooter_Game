// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"


AWeapon::AWeapon() :
ThrowWeaponTime(0.7),
bFalling(false),
Ammo(30),
MagazineCapacity(30),
WeaponType(EWeaponType::EWT_SubmachineGun),
AmmoType(EAmmoType::EAT_9mm),
ReloadMontageSection(FName(TEXT("Reload SMG"))),
ClipBoneName(TEXT("b_gun_mag")),
SlideDisplacement(0.f),
SlideDisplacementTime(0.1f),
bMovingSlide(false),
MaxSlideDisplacement(4.f),
MaxRecoilRotation(20.f),
bAutomatic(true)
{
	/*PrimaryActorTick.bCanEverTick = true;*/
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(GetItemState()== EItemState::EIS_Falling && bFalling)
	{
		const FRotator MeshRotation{0.f, GetItemMesh()->GetComponentRotation().Yaw, 0.f};
		GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);
	}
	UpdateSlideDisplacement();
}

void AWeapon::ThrowWeapon()
{
	const FRotator MeshRotation{0.f, GetItemMesh()->GetComponentRotation().Yaw, 0.f};
	GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);
	const FVector MeshForward{GetItemMesh()->GetForwardVector()};
	const FVector MeshRight{GetItemMesh()->GetRightVector()};
	FVector ImpulseDirection = MeshRight.RotateAngleAxis(-20.f, MeshForward);
	const float RandomRotation{30.f};
	ImpulseDirection = ImpulseDirection.RotateAngleAxis(RandomRotation, FVector(0.f, 0.f, 1.f));
	ImpulseDirection *= 2'000.f;
	GetItemMesh()->AddImpulse(ImpulseDirection);

	bFalling = true;
	GetWorldTimerManager().SetTimer
	(ThroWeaponTimer,
		this,
		&AWeapon::StopFalling,
		ThrowWeaponTime);
	
}
void AWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	const FString WeaponTablePath{TEXT("DataTable'/Game/Game/DataTable/MyWeaponDataTable.MyWeaponDataTable'")};
	UDataTable* WeaponTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *WeaponTablePath));
	if(WeaponTableObject)
	{
		// ReSharper disable once CppDeclaratorNeverUsed
		FWeaponDataTable* WeaponDataRow = nullptr;

		switch (WeaponType)
		{
		case EWeaponType::EWT_SubmachineGun:
			WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(FName("SubMachineGun"), TEXT(""));

			break;
			case EWeaponType::EWT_AssultRiffle:
				WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(FName("AssaultRifle"), TEXT(""));
			
			break;
		case EWeaponType::EWT_Pistol:
			WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(FName("Pistol"), TEXT(""));
			break;
        	default: ;
		}
		if(WeaponDataRow)
		{
			AmmoType = WeaponDataRow->AmmoType;
			Ammo = WeaponDataRow->WeaponAmmo;
			MagazineCapacity = WeaponDataRow->MagazineCapacity;
			SetPickupSound(WeaponDataRow->PickupSound);
			SetEquipSound(WeaponDataRow->EquipSound);
			GetItemMesh()->SetSkeletalMesh(WeaponDataRow->ItemMesh);
			SetItemName(WeaponDataRow->ItemName);
			SetIconItem(WeaponDataRow->InventoryIcon);
			SetAmmoIcon(WeaponDataRow->AmmoIcon);
			SetClipBoneName(WeaponDataRow->ClipBoneName);
			SetReloadMontageSection(WeaponDataRow->ReloadMontageSection);
			GetItemMesh()->SetAnimInstanceClass(WeaponDataRow->AnimBP);
			CrosshairsMiddle = WeaponDataRow->CrosshairsMiddle;
			CrosshairsLeft = WeaponDataRow->CrosshairsLeft;
			CrosshairsRight = WeaponDataRow->CrosshairsRight;
			CrosshairsButtom = WeaponDataRow->CrosshairsButtom;
			CrosshairsTop = WeaponDataRow->CrosshairsTop;
			AutoFireRate = WeaponDataRow->AutoFireRate;
			MuzzleFlash = WeaponDataRow->MuzzleFlash;
			FireSound = WeaponDataRow->FireSound;
			bAutomatic = WeaponDataRow->bAutomatic;
			Damage = WeaponDataRow->Damage;
			HeadShotDamage = WeaponDataRow->HeadShotDamage;
			
		
			
			
		}
	}
	
}

void AWeapon::FinishMovingSlide()
{
	bMovingSlide = false;
}

void AWeapon::UpdateSlideDisplacement()
{
	if(SlideDisplacementCurve && bMovingSlide)
	{
		// ReSharper disable once CppDeclaratorNeverUsed
		const float ElapsedTime{GetWorldTimerManager().GetTimerElapsed(SliderTimer)};
		// ReSharper disable once CppDeclaratorNeverUsed
		const float CurveValue{SlideDisplacementCurve->GetFloatValue(ElapsedTime)};
		SlideDisplacement = CurveValue * MaxSlideDisplacement;
		RecoilRotation = CurveValue * MaxRecoilRotation;
	}
}


void AWeapon::DecrementAmmo()
{
	if(Ammo - 1 <= 0)
	{
		Ammo = 0;
	}
	else
	{
		 --Ammo;
	}
}

void AWeapon::StartSlideTimer()
{
	bMovingSlide = true;
	GetWorldTimerManager().SetTimer
	(SliderTimer, this, &AWeapon::FinishMovingSlide,SlideDisplacementTime);
}

void AWeapon::ReloadAmmo(int32 Amount)
{
	checkf(Ammo + Amount <= MagazineCapacity, TEXT("Attemped to reload with more than magazine capcity"));
	Ammo += Amount;
}

bool AWeapon::ClipIsFull()
{
	return Ammo >= MagazineCapacity;
}

void AWeapon::StopFalling()
{
	bFalling = true;
	SetItemState(EItemState::EIS_Pickup);
}



