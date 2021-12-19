// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"


AWeapon::AWeapon() :
ThrowWeaponTime(0.7),
bFalling(false)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(GetItemState()== EItemState::EIS_Falling && bFalling)
	{
		const FRotator MeshRotation{0.f, GetItemMesh()->GetComponentRotation().Yaw, 0.f};
		GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);
	}
}

void AWeapon::ThrowWeapon()
{
	const FRotator MeshRotation{0.f, GetItemMesh()->GetComponentRotation().Yaw, 0.f};
	GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);
	const FVector MeshForward{GetItemMesh()->GetForwardVector()};
	const FVector MeshRight{GetItemMesh()->GetRightVector()};
	FVector ImpulseDirection = MeshRight.RotateAngleAxis(-20.f, MeshForward);
	float RandomRotation{30.f};
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

void AWeapon::StopFalling()
{
	bFalling = true;
	SetItemState(EItemState::EIS_Pickup);
}
